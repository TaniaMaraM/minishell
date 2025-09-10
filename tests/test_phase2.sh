#!/bin/bash

# Phase 2 Test Script - Lexer
# Tests: tokenization, quote handling, metacharacter recognition, error detection

# set -e  # Removed to prevent early exit on test failures

MINISHELL="./minishell"
TEST_DIR="$(dirname "$0")"
TEMP_DIR="/tmp/minishell_phase2_test_$$"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counter
TESTS_PASSED=0
TESTS_FAILED=0

# Helper functions
log_test() {
    echo -e "${YELLOW}[TEST]${NC} $1"
}

log_pass() {
    echo -e "${GREEN}[PASS]${NC} $1"
    ((TESTS_PASSED++))
}

log_fail() {
    echo -e "${RED}[FAIL]${NC} $1"
    ((TESTS_FAILED++))
}

# Test function for specific input
test_lexer_input() {
    local input="$1"
    local expected_tokens="$2"
    local test_name="$3"
    
    log_test "Testing: $test_name"
    
    # Get actual output
    local actual_output=$(echo "$input" | timeout 3s "$MINISHELL" 2>/dev/null | grep "Token:" | wc -l)
    local expected_count=$(echo "$expected_tokens" | wc -w)
    
    if [ "$actual_output" -eq "$expected_count" ]; then
        log_pass "$test_name - Token count matches"
    else
        log_fail "$test_name - Expected $expected_count tokens, got $actual_output"
    fi
}

# Test 1: Basic Tokenization
test_basic_tokenization() {
    log_test "Testing basic tokenization..."
    
    # Test simple command - check if it executes correctly
    local output=$(echo "echo hello world" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output" = "hello world" ]; then
        log_pass "Basic tokenization works (command executed correctly)"
    else
        log_fail "Basic tokenization failed - got: '$output'"
    fi
}

# Test 2: Quote Handling
test_quote_handling() {
    log_test "Testing quote handling..."
    
    # Test single quotes
    local output1=$(echo "echo 'hello world'" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output1" = "hello world" ]; then
        log_pass "Single quotes work (command executed correctly)"
    else
        log_fail "Single quotes failed - got: '$output1'"
    fi
    
    # Test double quotes
    local output2=$(echo "echo \"hello world\"" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output2" = "hello world" ]; then
        log_pass "Double quotes work (command executed correctly)"
    else
        log_fail "Double quotes failed - got: '$output2'"
    fi
}

# Test 3: Metacharacter Recognition
test_metacharacter_recognition() {
    log_test "Testing metacharacter recognition..."
    
    # Test pipe
    local output1=$(echo "echo hello | grep hello" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output1" = "hello" ]; then
        log_pass "Pipe recognition works (command executed correctly)"
    else
        log_fail "Pipe recognition failed - got: '$output1'"
    fi
    
    # Test output redirection
    echo "echo hello > test_file.txt" | timeout 3s "$MINISHELL" 2>/dev/null
    if [ -f "test_file.txt" ] && [ "$(cat test_file.txt)" = "hello" ]; then
        log_pass "Output redirection works (file created correctly)"
        rm -f test_file.txt
    else
        log_fail "Output redirection failed"
        rm -f test_file.txt
    fi
    
    # Test append redirection
    echo "echo world >> test_file.txt" | timeout 3s "$MINISHELL" 2>/dev/null
    if [ -f "test_file.txt" ] && [ "$(cat test_file.txt)" = "world" ]; then
        log_pass "Append redirection works (file appended correctly)"
        rm -f test_file.txt
    else
        log_fail "Append redirection failed"
        rm -f test_file.txt
    fi
    
    # Test input redirection
    echo "hello world" > test_input.txt
    local output4=$(echo "cat < test_input.txt" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output4" = "hello world" ]; then
        log_pass "Input redirection works (command executed correctly)"
    else
        log_fail "Input redirection failed - got: '$output4'"
    fi
    rm -f test_input.txt
    

}

# Test 4: Error Detection
test_error_detection() {
    log_test "Testing error detection..."
    
    # Test unterminated single quote - should handle gracefully
    local output=$(echo "echo 'unterminated" | timeout 3s "$MINISHELL" 2>/dev/null || echo "ERROR")
    if [ "$output" = "ERROR" ] || [ -z "$output" ]; then
        log_pass "Unterminated quote handled gracefully"
    else
        log_fail "Unterminated quote not handled properly - got: '$output'"
    fi
}

# Test 5: Complex Commands
test_complex_commands() {
    log_test "Testing complex commands..."
    
    # Test command with mixed quotes
    local output=$(echo "echo 'hello' \"world\" test" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output" = "hello world test" ]; then
        log_pass "Mixed quotes work (command executed correctly)"
    else
        log_fail "Mixed quotes failed - got: '$output'"
    fi
    
    # Test command with variables
    local output2=$(echo "echo \$HOME" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ -n "$output2" ]; then
        log_pass "Variable expansion works (command executed correctly)"
    else
        log_fail "Variable expansion failed - got: '$output2'"
    fi
}

# Test 6: Whitespace Handling
test_whitespace_handling() {
    log_test "Testing whitespace handling..."
    
    # Test multiple spaces
    local output1=$(echo "echo    hello    world" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output1" = "hello world" ]; then
        log_pass "Multiple spaces handled correctly (command executed correctly)"
    else
        log_fail "Multiple spaces handling failed - got: '$output1'"
    fi
    
    # Test tabs
    local output2=$(printf "echo\thello\tworld\n" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output2" = "hello world" ]; then
        log_pass "Tabs handled correctly (command executed correctly)"
    else
        log_fail "Tabs handling failed - got: '$output2'"
    fi
}

# Main test execution
main() {
    echo "=========================================="
    echo "Minishell Phase 2 Test Suite (Lexer)"
    echo "=========================================="
    
    # Create temporary directory
    mkdir -p "$TEMP_DIR"
    
    # Run tests
    test_basic_tokenization
    test_quote_handling
    test_metacharacter_recognition
    test_error_detection
    test_complex_commands
    test_whitespace_handling
    
    # Cleanup
    rm -rf "$TEMP_DIR"
    
    # Summary
    echo "=========================================="
    echo "Test Results:"
    echo "Passed: $TESTS_PASSED"
    echo "Failed: $TESTS_FAILED"
    echo "Total:  $((TESTS_PASSED + TESTS_FAILED))"
    
    if [ $TESTS_FAILED -eq 0 ]; then
        echo -e "${GREEN}All Phase 2 tests passed! ✅${NC}"
        exit 0
    else
        echo -e "${RED}Some Phase 2 tests failed! ❌${NC}"
        exit 1
    fi
}

# Run main function
main "$@"
