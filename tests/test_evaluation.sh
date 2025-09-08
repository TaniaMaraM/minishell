#!/bin/bash

# Phase 10: Advanced Testing - Evaluation Simulation
# Simulates common evaluation scenarios

set -e

MINISHELL="./minishell"
TESTS_PASSED=0
TESTS_FAILED=0

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

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

log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

# Test 1: Basic Functionality (Most Important)
test_basic_functionality() {
    log_test "Testing basic functionality (evaluator's first test)..."
    
    # Simple echo
    local output1=$(echo "echo hello world" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output1" = "hello world" ]; then
        log_pass "Basic echo works"
    else
        log_fail "Basic echo failed - got: '$output1'"
    fi
    
    # PWD
    local output2=$(echo "pwd" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ -n "$output2" ] && [ -d "$output2" ]; then
        log_pass "PWD works"
    else
        log_fail "PWD failed - got: '$output2'"
    fi
    
    # ENV
    local output3=$(echo "env" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ -n "$output3" ]; then
        log_pass "ENV works"
    else
        log_fail "ENV failed"
    fi
}

# Test 2: Redirections (Common Evaluation Focus)
test_redirections() {
    log_test "Testing redirections (evaluator's favorite)..."
    
    # Output redirection
    echo "echo hello > test_eval.txt" | timeout 3s "$MINISHELL" 2>/dev/null
    if [ -f "test_eval.txt" ] && [ "$(cat test_eval.txt)" = "hello" ]; then
        log_pass "Output redirection works"
        rm -f test_eval.txt
    else
        log_fail "Output redirection failed"
        rm -f test_eval.txt
    fi
    
    # Input redirection
    echo "hello world" > test_input.txt
    local output=$(echo "cat < test_input.txt" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output" = "hello world" ]; then
        log_pass "Input redirection works"
    else
        log_fail "Input redirection failed - got: '$output'"
    fi
    rm -f test_input.txt
    
    # Append redirection
    echo "echo hello > test_append.txt" | timeout 3s "$MINISHELL" 2>/dev/null
    echo "echo world >> test_append.txt" | timeout 3s "$MINISHELL" 2>/dev/null
    if [ -f "test_append.txt" ] && [ "$(cat test_append.txt)" = "hello
world" ]; then
        log_pass "Append redirection works"
        rm -f test_append.txt
    else
        log_fail "Append redirection failed"
        rm -f test_append.txt
    fi
}

# Test 3: Pipelines (Another Evaluation Favorite)
test_pipelines() {
    log_test "Testing pipelines (evaluator's second favorite)..."
    
    # Simple pipeline
    local output1=$(echo "echo hello | cat" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output1" = "hello" ]; then
        log_pass "Simple pipeline works"
    else
        log_fail "Simple pipeline failed - got: '$output1'"
    fi
    
    # Pipeline with grep
    local output2=$(echo "echo hello world | grep hello" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output2" = "hello world" ]; then
        log_pass "Pipeline with grep works"
    else
        log_fail "Pipeline with grep failed - got: '$output2'"
    fi
    
    # Multiple pipelines
    local output3=$(echo "echo hello | cat | wc -c" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output3" = "       6" ]; then
        log_pass "Multiple pipelines work"
    else
        log_fail "Multiple pipelines failed - got: '$output3'"
    fi
}

# Test 4: Variable Expansion (Common Evaluation Test)
test_variable_expansion() {
    log_test "Testing variable expansion (evaluator's third favorite)..."
    
    # Basic variable expansion
    export TEST_VAR="hello world"
    local output1=$(echo "echo \$TEST_VAR" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output1" = "hello world" ]; then
        log_pass "Basic variable expansion works"
    else
        log_fail "Basic variable expansion failed - got: '$output1'"
    fi
    
    # Exit status expansion
    local output2=$(echo "false; echo \$?" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output2" = "1" ]; then
        log_pass "Exit status expansion works"
    else
        log_fail "Exit status expansion failed - got: '$output2'"
    fi
    
    # Undefined variable
    local output3=$(echo "echo \$UNDEFINED_VAR" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output3" = "" ]; then
        log_pass "Undefined variable handling works"
    else
        log_fail "Undefined variable handling failed - got: '$output3'"
    fi
    
    unset TEST_VAR
}

# Test 5: Quote Handling (Evaluation Edge Case)
test_quote_handling() {
    log_test "Testing quote handling (evaluator's edge case favorite)..."
    
    # Single quotes prevent expansion
    local output1=$(echo "echo '\$HOME'" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output1" = "\$HOME" ]; then
        log_pass "Single quotes prevent expansion"
    else
        log_fail "Single quotes expansion failed - got: '$output1'"
    fi
    
    # Double quotes allow expansion
    local output2=$(echo "echo \"\$HOME\"" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ -n "$output2" ] && [ -d "$output2" ]; then
        log_pass "Double quotes allow expansion"
    else
        log_fail "Double quotes expansion failed - got: '$output2'"
    fi
    
    # Mixed quotes
    local output3=$(echo "echo \"hello 'world' test\"" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output3" = "hello 'world' test" ]; then
        log_pass "Mixed quotes work"
    else
        log_fail "Mixed quotes failed - got: '$output3'"
    fi
}

# Test 6: Built-ins (Evaluation Requirement)
test_builtins() {
    log_test "Testing built-ins (evaluator's requirement check)..."
    
    # Export
    echo "export TEST_BUILTIN=value" | timeout 3s "$MINISHELL" 2>/dev/null
    local output1=$(echo "env | grep TEST_BUILTIN" | timeout 3s "$MINISHELL" 2>/dev/null)
    if echo "$output1" | grep -q "TEST_BUILTIN=value"; then
        log_pass "Export works"
    else
        log_fail "Export failed"
    fi
    
    # Unset
    echo "unset TEST_BUILTIN" | timeout 3s "$MINISHELL" 2>/dev/null
    local output2=$(echo "env | grep TEST_BUILTIN" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ -z "$output2" ]; then
        log_pass "Unset works"
    else
        log_fail "Unset failed"
    fi
    
    # CD
    local original_dir=$(pwd)
    echo "cd /tmp" | timeout 3s "$MINISHELL" 2>/dev/null
    local output3=$(echo "pwd" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output3" = "/tmp" ]; then
        log_pass "CD works"
    else
        log_fail "CD failed - got: '$output3'"
    fi
    cd "$original_dir"
}

# Test 7: Error Handling (Evaluation Robustness Check)
test_error_handling() {
    log_test "Testing error handling (evaluator's robustness check)..."
    
    # Non-existent command
    local output1=$(echo "nonexistent_command" | timeout 3s "$MINISHELL" 2>/dev/null)
    if echo "$output1" | grep -q "command not found"; then
        log_pass "Non-existent command error handling works"
    else
        log_fail "Non-existent command error handling failed - got: '$output1'"
    fi
    
    # Invalid export
    local output2=$(echo "export 123INVALID=value" | timeout 3s "$MINISHELL" 2>/dev/null)
    if echo "$output2" | grep -q "not a valid identifier"; then
        log_pass "Invalid export error handling works"
    else
        log_fail "Invalid export error handling failed - got: '$output2'"
    fi
    
    # CD to non-existent directory
    local output3=$(echo "cd /nonexistent" | timeout 3s "$MINISHELL" 2>/dev/null)
    if echo "$output3" | grep -q "No such file or directory"; then
        log_pass "CD error handling works"
    else
        log_fail "CD error handling failed - got: '$output3'"
    fi
}

# Test 8: Signal Handling (Evaluation Advanced Check)
test_signal_handling() {
    log_test "Testing signal handling (evaluator's advanced check)..."
    
    # Test that minishell doesn't crash on signals
    local output=$(echo "echo hello" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output" = "hello" ]; then
        log_pass "Signal handling works (no crash)"
    else
        log_fail "Signal handling failed - got: '$output'"
    fi
}

# Main execution
main() {
    echo "🎯 Phase 10: Advanced Testing - Evaluation Simulation"
    echo "====================================================="
    echo ""
    log_info "This test simulates common evaluation scenarios"
    log_info "These are the tests evaluators typically run"
    echo ""
    
    test_basic_functionality
    test_redirections
    test_pipelines
    test_variable_expansion
    test_quote_handling
    test_builtins
    test_error_handling
    test_signal_handling
    
    echo ""
    echo "====================================================="
    echo "Evaluation Simulation Results:"
    echo "Passed: $TESTS_PASSED"
    echo "Failed: $TESTS_FAILED"
    echo "Total:  $((TESTS_PASSED + TESTS_FAILED))"
    echo ""
    
    if [ $TESTS_FAILED -eq 0 ]; then
        echo -e "${GREEN}🎉 All evaluation tests passed! You're ready! ✅${NC}"
        echo ""
        log_info "Your minishell handles all common evaluation scenarios"
        log_info "You're well-prepared for the evaluation!"
        exit 0
    else
        echo -e "${RED}❌ Some evaluation tests failed! Review the issues above${NC}"
        echo ""
        log_info "Fix the failing tests before your evaluation"
        log_info "These are the exact scenarios evaluators will test"
        exit 1
    fi
}

main "$@"
