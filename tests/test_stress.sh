#!/bin/bash

# Phase 10: Advanced Testing - Stress Tests
# Tests stress scenarios that evaluators might use

set -e

MINISHELL="./minishell"
TESTS_PASSED=0
TESTS_FAILED=0

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
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

# Test 1: Memory Stress Tests
test_memory_stress() {
    log_test "Testing memory stress..."
    
    # Multiple commands in sequence
    local output=$(printf "echo hello\necho world\necho test\n" | timeout 5s "$MINISHELL" 2>/dev/null)
    local expected="hello
world
test"
    if [ "$output" = "$expected" ]; then
        log_pass "Multiple commands work"
    else
        log_fail "Multiple commands failed - got: '$output'"
    fi
    
    # Long command line
    local long_cmd="echo "
    for i in {1..50}; do
        long_cmd="${long_cmd}word$i "
    done
    local output2=$(echo "$long_cmd" | timeout 5s "$MINISHELL" 2>/dev/null)
    if [ -n "$output2" ]; then
        log_pass "Long command line works"
    else
        log_fail "Long command line failed"
    fi
}

# Test 2: Pipeline Stress Tests
test_pipeline_stress() {
    log_test "Testing pipeline stress..."
    
    # Deep pipeline
    local output=$(echo "echo hello | cat | cat | cat" | timeout 5s "$MINISHELL" 2>/dev/null)
    if [ "$output" = "hello" ]; then
        log_pass "Deep pipeline works"
    else
        log_fail "Deep pipeline failed - got: '$output'"
    fi
    
    # Pipeline with redirections
    echo "echo hello | cat > test_stress.txt" | timeout 5s "$MINISHELL" 2>/dev/null
    if [ -f "test_stress.txt" ] && [ "$(cat test_stress.txt)" = "hello" ]; then
        log_pass "Pipeline with redirection works"
        rm -f test_stress.txt
    else
        log_fail "Pipeline with redirection failed"
        rm -f test_stress.txt
    fi
}

# Test 3: Redirection Stress Tests
test_redirection_stress() {
    log_test "Testing redirection stress..."
    
    # Multiple redirections
    echo "echo hello > test1.txt" | timeout 3s "$MINISHELL" 2>/dev/null
    echo "echo world > test2.txt" | timeout 3s "$MINISHELL" 2>/dev/null
    echo "echo test > test3.txt" | timeout 3s "$MINISHELL" 2>/dev/null
    
    if [ -f "test1.txt" ] && [ -f "test2.txt" ] && [ -f "test3.txt" ]; then
        log_pass "Multiple redirections work"
        rm -f test1.txt test2.txt test3.txt
    else
        log_fail "Multiple redirections failed"
        rm -f test1.txt test2.txt test3.txt
    fi
    
    # Large heredoc
    local heredoc_output=$(echo "cat << EOF
hello
world
test
EOF" | timeout 5s "$MINISHELL" 2>/dev/null)
    local expected="hello
world
test"
    if [ "$heredoc_output" = "$expected" ]; then
        log_pass "Large heredoc works"
    else
        log_fail "Large heredoc failed - got: '$heredoc_output'"
    fi
}

# Test 4: Variable Expansion Stress Tests
test_variable_stress() {
    log_test "Testing variable expansion stress..."
    
    # Multiple variables
    export VAR1="hello"
    export VAR2="world"
    export VAR3="test"
    
    local output=$(echo "echo \$VAR1 \$VAR2 \$VAR3" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output" = "hello world test" ]; then
        log_pass "Multiple variables work"
    else
        log_fail "Multiple variables failed - got: '$output'"
    fi
    
    unset VAR1 VAR2 VAR3
    
    # Complex variable expansion
    export COMPLEX_VAR="hello world test"
    local output2=$(echo "echo \"\$COMPLEX_VAR\"" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ "$output2" = "hello world test" ]; then
        log_pass "Complex variable expansion works"
    else
        log_fail "Complex variable expansion failed - got: '$output2'"
    fi
    unset COMPLEX_VAR
}

# Test 5: Built-in Stress Tests
test_builtin_stress() {
    log_test "Testing built-in stress..."
    
    # Multiple exports
    echo "export VAR1=value1" | timeout 3s "$MINISHELL" 2>/dev/null
    echo "export VAR2=value2" | timeout 3s "$MINISHELL" 2>/dev/null
    echo "export VAR3=value3" | timeout 3s "$MINISHELL" 2>/dev/null
    
    local output=$(echo "env | grep VAR" | timeout 3s "$MINISHELL" 2>/dev/null)
    if echo "$output" | grep -q "VAR1=value1" && echo "$output" | grep -q "VAR2=value2" && echo "$output" | grep -q "VAR3=value3"; then
        log_pass "Multiple exports work"
    else
        log_fail "Multiple exports failed"
    fi
    
    # Multiple unsets
    echo "unset VAR1 VAR2 VAR3" | timeout 3s "$MINISHELL" 2>/dev/null
    local output2=$(echo "env | grep VAR" | timeout 3s "$MINISHELL" 2>/dev/null)
    if [ -z "$output2" ]; then
        log_pass "Multiple unsets work"
    else
        log_fail "Multiple unsets failed"
    fi
}

# Test 6: Error Recovery Stress Tests
test_error_recovery_stress() {
    log_test "Testing error recovery stress..."
    
    # Commands that fail followed by commands that work
    local output=$(printf "false\necho hello\nfalse\necho world\n" | timeout 5s "$MINISHELL" 2>/dev/null)
    local expected="hello
world"
    if [ "$output" = "$expected" ]; then
        log_pass "Error recovery works"
    else
        log_fail "Error recovery failed - got: '$output'"
    fi
    
    # Invalid commands followed by valid commands
    local output2=$(printf "nonexistent_command\necho hello\ninvalid_cmd\necho world\n" | timeout 5s "$MINISHELL" 2>/dev/null)
    local expected2="hello
world"
    if [ "$output2" = "$expected2" ]; then
        log_pass "Invalid command recovery works"
    else
        log_fail "Invalid command recovery failed - got: '$output2'"
    fi
}

# Main execution
main() {
    echo "💪 Phase 10: Advanced Testing - Stress Tests"
    echo "============================================="
    
    test_memory_stress
    test_pipeline_stress
    test_redirection_stress
    test_variable_stress
    test_builtin_stress
    test_error_recovery_stress
    
    echo "============================================="
    echo "Stress Test Results:"
    echo "Passed: $TESTS_PASSED"
    echo "Failed: $TESTS_FAILED"
    echo "Total:  $((TESTS_PASSED + TESTS_FAILED))"
    
    if [ $TESTS_FAILED -eq 0 ]; then
        echo -e "${GREEN}All stress tests passed! ✅${NC}"
        exit 0
    else
        echo -e "${RED}Some stress tests failed! ❌${NC}"
        exit 1
    fi
}

main "$@"
