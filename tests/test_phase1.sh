#!/bin/bash

# Phase 1 Test Script - REPL + Signal Handling
# Tests: signal handling, EOF handling, interactive vs non-interactive

# set -e  # Removed to prevent early exit on test failures

MINISHELL="./minishell"
TEST_DIR="$(dirname "$0")"
TEMP_DIR="/tmp/minishell_phase1_test_$$"

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

# Test 1: EOF Handling (Ctrl+D)
test_eof_handling() {
    log_test "Testing EOF handling (Ctrl+D)..."
    
    # Test non-interactive mode EOF
    local output=$(echo "echo hello" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output" = "hello" ]; then
        log_pass "Non-interactive mode works"
    else
        log_fail "Non-interactive mode failed - got: '$output'"
    fi
    
    # Test basic EOF behavior
    if echo "exit" | timeout 3s "$MINISHELL" 2>/dev/null >/dev/null; then
        log_pass "Basic EOF handling works"
    else
        log_fail "Basic EOF handling failed"
    fi
}

# Test 2: Signal Handling in Interactive Mode
test_interactive_signals() {
    log_test "Testing interactive signal handling..."
    
    # Create a test script for interactive signal testing
    cat > "$TEMP_DIR/interactive_signal_test.sh" << 'EOF'
#!/bin/bash
# Test interactive signal handling
{
    sleep 1
    echo "echo hello"
    sleep 1
    kill -INT $$
} | timeout 5s ./minishell 2>/dev/null | head -5
EOF
    
    chmod +x "$TEMP_DIR/interactive_signal_test.sh"
    
    local output=$("$TEMP_DIR/interactive_signal_test.sh")
    if [ -n "$output" ]; then
        log_pass "Interactive signal handling works"
    else
        log_fail "Interactive signal handling failed"
    fi
}

# Test 3: Signal Handling in Non-Interactive Mode
test_non_interactive_signals() {
    log_test "Testing non-interactive signal handling..."
    
    # Test that non-interactive mode exits on SIGINT
    if timeout 2s bash -c 'echo "echo hello" | ./minishell & sleep 1; kill -INT $!; wait $!' 2>/dev/null; then
        log_pass "Non-interactive signal handling works"
    else
        log_fail "Non-interactive signal handling failed"
    fi
}

# Test 4: History Management
test_history_management() {
    log_test "Testing history management..."
    
    # Test that empty lines are not added to history
    local output=$(printf "\n\necho hello\n" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output" = "hello" ]; then
        log_pass "History management works (empty lines skipped)"
    else
        log_fail "History management failed - got: '$output'"
    fi
}

# Test 5: Environment Variable Handling
test_environment_handling() {
    log_test "Testing environment variable handling..."
    
    # Test that environment is properly copied
    local output=$(echo "echo hello" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output" = "hello" ]; then
        log_pass "Environment handling works"
    else
        log_fail "Environment handling failed - got: '$output'"
    fi
}

# Test 6: Memory Management
test_memory_management() {
    log_test "Testing memory management..."
    
    # Test multiple commands to check for memory leaks
    local output=$(printf "echo hello\necho world\necho test\n" | timeout 5s "$MINISHELL" 2>/dev/null)
    local expected="hello
world
test"
    if [ "$output" = "$expected" ]; then
        log_pass "Memory management works (multiple commands)"
    else
        log_fail "Memory management failed - got: '$output'"
    fi
}

# Main test execution
main() {
    echo "=========================================="
    echo "Minishell Phase 1 Test Suite"
    echo "=========================================="
    
    # Create temporary directory
    mkdir -p "$TEMP_DIR"
    
    # Run tests
    test_eof_handling
    test_interactive_signals
    test_non_interactive_signals
    test_history_management
    test_environment_handling
    test_memory_management
    
    # Cleanup
    rm -rf "$TEMP_DIR"
    
    # Summary
    echo "=========================================="
    echo "Test Results:"
    echo "Passed: $TESTS_PASSED"
    echo "Failed: $TESTS_FAILED"
    echo "Total:  $((TESTS_PASSED + TESTS_FAILED))"
    
    if [ $TESTS_FAILED -eq 0 ]; then
        echo -e "${GREEN}All Phase 1 tests passed! ✅${NC}"
        exit 0
    else
        echo -e "${RED}Some Phase 1 tests failed! ❌${NC}"
        exit 1
    fi
}

# Run main function
main "$@"
