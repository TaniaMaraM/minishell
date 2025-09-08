#!/bin/bash

# Phase 5: Executor Tests
# Tests command execution, built-ins, external commands, pipelines, and redirections

set -e

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counters
TESTS_PASSED=0
TESTS_FAILED=0
TOTAL_TESTS=0

# Function to run a test
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_exit="$3"
    local description="$4"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -n "Testing $test_name: "
    
    # Run the command and capture exit code
    local actual_exit
    actual_exit=$(echo "$command" | timeout 5s ./minishell 2>/dev/null >/dev/null; echo $?)
    
    if [ "$actual_exit" = "$expected_exit" ]; then
        echo -e "${GREEN}PASS${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}FAIL${NC} (expected: $expected_exit, got: $actual_exit)"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
}

# Function to run output test
run_output_test() {
    local test_name="$1"
    local command="$2"
    local expected_output="$3"
    local description="$4"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -n "Testing $test_name: "
    
    # Run the command and capture output
    local actual_output
    actual_output=$(echo "$command" | timeout 5s ./minishell 2>/dev/null | head -1)
    
    if [ "$actual_output" = "$expected_output" ]; then
        echo -e "${GREEN}PASS${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}FAIL${NC} (expected: '$expected_output', got: '$actual_output')"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
}

echo -e "${YELLOW}=== Phase 5: Executor Tests ===${NC}"
echo

# Check if minishell exists
if [ ! -f "./minishell" ]; then
    echo -e "${RED}Error: minishell executable not found. Run 'make' first.${NC}"
    exit 1
fi

echo -e "${YELLOW}--- Built-in Commands ---${NC}"

# Test echo command
run_output_test "echo basic" "echo hello" "hello"
run_output_test "echo with -n" "echo -n hello" "hello"
run_output_test "echo multiple args" "echo hello world" "hello world"

# Test pwd command
run_test "pwd command" "pwd" "0"

# Test env command
run_test "env command" "env" "0"

# Test cd command
run_test "cd to home" "cd" "0"
run_test "cd to /tmp" "cd /tmp" "0"
run_test "cd invalid" "cd /nonexistent" "1"

# Test export command
run_test "export new var" "export TEST_VAR=hello" "0"
run_test "export invalid name" "export 123INVALID=value" "1"

# Test unset command
run_test "unset var" "unset TEST_VAR" "0"
run_test "unset invalid name" "unset 123INVALID" "1"

# Test exit command (these will exit the shell, so we test differently)
echo -n "Testing exit with code 0: "
if echo "exit 0" | timeout 2s ./minishell 2>/dev/null | grep -q "exit"; then
    echo -e "${GREEN}PASS${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${RED}FAIL${NC}"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

echo -n "Testing exit with code 42: "
if echo "exit 42" | timeout 2s ./minishell 2>/dev/null | grep -q "exit"; then
    echo -e "${GREEN}PASS${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${RED}FAIL${NC}"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

echo
echo -e "${YELLOW}--- External Commands ---${NC}"

# Test external commands
run_test "ls command" "ls" "0"
run_test "which command" "which ls" "0"
run_test "date command" "date" "0"

# Test command not found
run_test "nonexistent command" "nonexistent_command" "127"

echo
echo -e "${YELLOW}--- Pipelines ---${NC}"

# Test simple pipeline
run_output_test "echo | grep" "echo hello world | grep hello" "hello world"

# Test ls | wc (count files)
echo -n "Testing ls | wc: "
file_count=$(echo "ls | wc -l" | timeout 5s ./minishell 2>/dev/null | tr -d ' ')
if [ "$file_count" -gt 0 ]; then
    echo -e "${GREEN}PASS${NC} (counted $file_count files)"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${RED}FAIL${NC}"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

# Test complex pipeline
echo -n "Testing echo | grep | wc: "
result=$(echo "echo hello world | grep hello | wc -l" | timeout 5s ./minishell 2>/dev/null | tr -d ' ')
if [ "$result" = "1" ]; then
    echo -e "${GREEN}PASS${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${RED}FAIL${NC} (expected: '1', got: '$result')"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

echo
echo -e "${YELLOW}--- Redirections ---${NC}"

# Test output redirection
run_test "output redirection" "echo hello > test_output.txt" "0"
if [ -f "test_output.txt" ] && [ "$(cat test_output.txt)" = "hello" ]; then
    echo -e "Testing output redirection file: ${GREEN}PASS${NC}"
    rm -f test_output.txt
else
    echo -e "Testing output redirection file: ${RED}FAIL${NC}"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

# Test append redirection
echo "world" > test_append.txt
run_test "append redirection" "echo hello >> test_append.txt" "0"
if [ -f "test_append.txt" ] && [ "$(cat test_append.txt)" = "world
hello" ]; then
    echo -e "Testing append redirection file: ${GREEN}PASS${NC}"
    rm -f test_append.txt
else
    echo -e "Testing append redirection file: ${RED}FAIL${NC}"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

# Test input redirection
echo "hello world" > test_input.txt
run_output_test "input redirection" "cat < test_input.txt" "hello world"
rm -f test_input.txt

echo
echo -e "${YELLOW}--- Error Handling ---${NC}"

# Test error handling
run_test "cd to invalid dir" "cd /nonexistent" "1"
run_test "export invalid syntax" "export 123=value" "1"
run_test "unset invalid syntax" "unset 123" "1"

echo
echo -e "${YELLOW}--- Variable Expansion Integration ---${NC}"

# Test variable expansion with execution
run_output_test "echo with $USER" "echo \$USER" "$USER"
run_output_test "echo with $HOME" "echo \$HOME" "$HOME"

echo
echo -e "${YELLOW}=== Test Results ===${NC}"
echo "Total tests: $TOTAL_TESTS"
echo -e "Passed: ${GREEN}$TESTS_PASSED${NC}"
echo -e "Failed: ${RED}$TESTS_FAILED${NC}"

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed! 🎉${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed. Please check the implementation.${NC}"
    exit 1
fi
