#!/bin/bash

# Minishell Evaluation Tester
# Comprehensive test suite comparing minishell with bash behavior
# Based on the minishell_testing_guide.md

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counters
TESTS_PASSED=0
TESTS_FAILED=0
TOTAL_TESTS=0

# Temporary directory for test files
TEMP_DIR="/tmp/minishell_eval_$$"
mkdir -p "$TEMP_DIR"

# Function to compare minishell output with bash
compare_with_bash() {
    local test_name="$1"
    local command="$2"
    local description="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Get bash output and exit code
    local bash_output
    local bash_exit
    bash_output=$(bash -c "$command" 2>&1)
    bash_exit=$?
    
    # Get minishell output and exit code
    local minishell_output
    local minishell_exit
    minishell_output=$(echo "$command" | ./minishell 2>&1)
    minishell_exit=$?
    
    # Compare results
    local status="PASS"
    local details=""
    
    # Special case for command not found (allow different shell names)
    if echo "$bash_output" | grep -q "command not found" && echo "$minishell_output" | grep -q "command not found" && [ "$bash_exit" = "$minishell_exit" ]; then
        status="PASS"
    # Normal comparison
    elif [ "$bash_output" != "$minishell_output" ] || [ "$bash_exit" != "$minishell_exit" ]; then
        status="FAIL"
        details=" (exit: bash=$bash_exit, minishell=$minishell_exit)"
    fi
    
    # Print result
    printf "%-40s" "Testing $test_name: "
    if [ "$status" = "PASS" ]; then
        echo -e "${GREEN}OK${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}KO${NC}${details}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    
    # Save detailed results if failed
    if [ "$status" = "FAIL" ]; then
        echo "Test: $test_name ($command)" >> evaluation_results.log
        echo "Description: $description" >> evaluation_results.log
        echo "Bash output: $bash_output" >> evaluation_results.log
        echo "Bash exit: $bash_exit" >> evaluation_results.log
        echo "Minishell output: $minishell_output" >> evaluation_results.log
        echo "Minishell exit: $minishell_exit" >> evaluation_results.log
        echo "-----------------------------------" >> evaluation_results.log
    fi
}

# Check if minishell exists
if [ ! -f "./minishell" ]; then
    echo -e "${RED}Error: minishell executable not found. Run 'make' first.${NC}"
    exit 1
fi

# Initialize log file
echo "MINISHELL EVALUATION RESULTS" > evaluation_results.log
echo "==========================" >> evaluation_results.log
echo "Date: $(date)" >> evaluation_results.log
echo "" >> evaluation_results.log

# Start testing
echo -e "${YELLOW}=== MINISHELL EVALUATION TEST ====${NC}"
echo -e "${BLUE}Comparing minishell behavior with bash${NC}"
echo

# 1. Basic Command Execution
echo -e "${YELLOW}--- 1. Basic Command Execution ---${NC}"

# Absolute Paths
compare_with_bash "absolute path ls" "/bin/ls -la" "Should execute ls with absolute path"
compare_with_bash "absolute path pwd" "/bin/pwd" "Should show current directory"
compare_with_bash "absolute path echo" "/bin/echo hello world" "Should echo text"

# Commands with Arguments
compare_with_bash "ls with flags" "ls -l -a" "Should list files with details"
compare_with_bash "echo with multiple args" "echo hello beautiful world" "Should echo multiple words"
compare_with_bash "cat with file" "cat Makefile" "Should display Makefile contents"

# Empty Commands and Whitespace
compare_with_bash "empty command" "" "Should handle empty command"
compare_with_bash "whitespace only" "   " "Should handle whitespace"
compare_with_bash "tabs only" "	" "Should handle tabs"

echo

# 2. Built-in Commands
echo -e "${YELLOW}--- 2. Built-in Commands ---${NC}"

# Echo Command
compare_with_bash "echo without args" "echo" "Should print newline"
compare_with_bash "echo with text" "echo hello world" "Should echo text"
compare_with_bash "echo with quotes" "echo \"hello world\"" "Should echo quoted text"
compare_with_bash "echo with -n flag" "echo -n hello" "Should echo without newline"
compare_with_bash "echo with multiple -n" "echo -n -n hello" "Should handle multiple -n flags"
compare_with_bash "echo with -n in middle" "echo hello -n world" "Should handle -n in middle"

# CD Command
compare_with_bash "cd to home" "cd && pwd" "Should change to home directory"
compare_with_bash "cd to /tmp" "cd /tmp && pwd" "Should change to /tmp"
compare_with_bash "cd with relative path" "cd .. && pwd" "Should change to parent directory"
compare_with_bash "cd with nonexistent dir" "cd /nonexistent 2>&1" "Should show error for nonexistent directory"

# PWD Command
compare_with_bash "pwd command" "pwd" "Should show current directory"

# Export Command
compare_with_bash "export variable" "export TESTVAR=hello && echo \$TESTVAR" "Should set and display environment variable"
compare_with_bash "export with quotes" "export TESTVAR=\"hello world\" && echo \$TESTVAR" "Should handle quoted values"
compare_with_bash "export invalid name" "export 123VAR=test 2>&1" "Should handle invalid variable names"

# Unset Command
compare_with_bash "unset variable" "export TESTVAR=hello && echo \$TESTVAR && unset TESTVAR && echo \$TESTVAR" "Should unset variable"

# Env Command
compare_with_bash "env command" "env | grep PATH" "Should show environment variables"

# Exit Command
compare_with_bash "exit with code" "exit 42 2>/dev/null || echo \$?" "Should exit with specified code"
compare_with_bash "exit with invalid arg" "exit abc 2>&1" "Should handle invalid exit argument"

echo

# 3. Environment Variables
echo -e "${YELLOW}--- 3. Environment Variables ---${NC}"

# Basic Variable Expansion
compare_with_bash "HOME variable" "echo \$HOME" "Should show HOME path"
compare_with_bash "USER variable" "echo \$USER" "Should show username"
compare_with_bash "PATH variable" "echo \$PATH" "Should show PATH"
compare_with_bash "undefined variable" "echo \$UNDEFINED_VAR" "Should handle undefined variables"

# Exit Status Testing
compare_with_bash "success exit status" "/bin/true && echo \$?" "Should show 0 after successful command"
compare_with_bash "failure exit status" "/bin/false && echo \$?" "Should show non-zero after failed command"
compare_with_bash "command not found status" "nosuchcommand 2>/dev/null; echo \$?" "Should show status after command not found"

# Complex Variable Usage
compare_with_bash "variable in double quotes" "export VAR=hello && echo \"\$VAR\"" "Should expand in double quotes"
compare_with_bash "variable in single quotes" "export VAR=hello && echo '\$VAR'" "Should not expand in single quotes"
compare_with_bash "variable concatenation" "export VAR=hello && echo \$VAR\$VAR" "Should concatenate variable values"
compare_with_bash "variable with text" "export VAR=hello && echo \${VAR}world" "Should handle variable with text"

echo

# 4. Quoting Tests
echo -e "${YELLOW}--- 4. Quoting Behavior ---${NC}"

# Single Quotes
compare_with_bash "text in single quotes" "echo 'hello world'" "Should preserve text exactly"
compare_with_bash "variables in single quotes" "echo '\$USER'" "Should not expand variables in single quotes"
compare_with_bash "special chars in single quotes" "echo 'hello | world > file'" "Should preserve special characters"

# Double Quotes
compare_with_bash "text in double quotes" "echo \"hello world\"" "Should preserve spaces"
compare_with_bash "variables in double quotes" "echo \"\$USER\"" "Should expand variables in double quotes"
compare_with_bash "special chars in double quotes" "echo \"hello | world > file\"" "Should preserve special characters"

# Mixed Quotes
compare_with_bash "single quotes in double quotes" "echo \"hello 'world'\"" "Should preserve nested quotes"
compare_with_bash "double quotes in single quotes" "echo 'hello \"world\"'" "Should preserve nested quotes"
compare_with_bash "adjacent quotes" "echo \"hello\"'world'" "Should concatenate quoted strings"

# Quote Edge Cases
compare_with_bash "command name with quotes" "ech'o' hello" "Should handle quotes in command name"
compare_with_bash "empty quotes" "echo \"\" ''" "Should handle empty quotes"
compare_with_bash "multiple empty quotes" "echo \"\"\"\"" "Should handle multiple empty quotes"

echo

# 5. Redirections
echo -e "${YELLOW}--- 5. Redirections ---${NC}"

# Input Redirection (<)
echo "test content" > "$TEMP_DIR/input.txt"
compare_with_bash "input from file" "cat < $TEMP_DIR/input.txt" "Should read from file"
compare_with_bash "input from nonexistent file" "cat < $TEMP_DIR/nonexistent.txt 2>&1" "Should handle nonexistent input file"

# Output Redirection (>)
compare_with_bash "output to file" "echo hello > $TEMP_DIR/output.txt && cat $TEMP_DIR/output.txt" "Should write to file"
compare_with_bash "output overwrite" "echo hello > $TEMP_DIR/overwrite.txt && echo world > $TEMP_DIR/overwrite.txt && cat $TEMP_DIR/overwrite.txt" "Should overwrite file"

# Append Redirection (>>)
compare_with_bash "append to file" "echo line1 > $TEMP_DIR/append.txt && echo line2 >> $TEMP_DIR/append.txt && cat $TEMP_DIR/append.txt" "Should append to file"

# Here Document (<<)
compare_with_bash "basic heredoc" "cat << EOF
hello world
testing
EOF" "Should handle heredoc input"

# Multiple Redirections
compare_with_bash "multiple output files" "echo test > $TEMP_DIR/file1.txt > $TEMP_DIR/file2.txt && cat $TEMP_DIR/file1.txt $TEMP_DIR/file2.txt" "Should handle multiple redirections"
compare_with_bash "input and output" "echo test > $TEMP_DIR/inout.txt && cat < $TEMP_DIR/inout.txt > $TEMP_DIR/result.txt && cat $TEMP_DIR/result.txt" "Should handle input and output redirections"

echo

# 6. Pipes
echo -e "${YELLOW}--- 6. Pipes ---${NC}"

# Simple Pipes
compare_with_bash "echo to cat" "echo hello | cat" "Should pipe output to cat"
compare_with_bash "echo to grep" "echo hello world | grep hello" "Should pipe output to grep"

# Multiple Pipes
compare_with_bash "triple pipe" "echo hello | cat | grep hello" "Should handle multiple pipes"
compare_with_bash "complex pipeline" "echo -e \"hello\\nworld\\ntest\" | grep o | wc -l" "Should handle complex pipeline"

# Pipes with Redirections
compare_with_bash "pipe with output redirection" "echo hello | cat > $TEMP_DIR/pipe_out.txt && cat $TEMP_DIR/pipe_out.txt" "Should handle pipe with redirection"
compare_with_bash "pipe with input redirection" "echo hello > $TEMP_DIR/pipe_in.txt && cat < $TEMP_DIR/pipe_in.txt | grep hello" "Should handle redirection with pipe"

# Pipes with Errors
compare_with_bash "command not found in pipe" "echo hello | nonexistentcmd 2>&1 | cat" "Should handle command not found in pipe"
compare_with_bash "failed command in pipe" "ls /nonexistent 2>&1 | grep No" "Should handle failed command in pipe"

echo

# 7. Error Handling
echo -e "${YELLOW}--- 7. Error Handling ---${NC}"

# Non-existent Commands
compare_with_bash "command not found" "nosuchcommand 2>&1" "Should handle command not found"
compare_with_bash "nonexistent file execution" "./nonexistent 2>&1" "Should handle nonexistent file execution"

# Non-existent Files
compare_with_bash "cat nonexistent file" "cat nonexistent.txt 2>&1" "Should handle nonexistent file"
compare_with_bash "ls nonexistent dir" "ls nonexistent_dir 2>&1" "Should handle nonexistent directory"

# Permission Errors
compare_with_bash "access restricted area" "cd /root 2>&1" "Should handle permission denied"

echo

# 8. Complex Command Testing
echo -e "${YELLOW}--- 8. Complex Commands ---${NC}"

# Long Commands
compare_with_bash "long command" "echo this is a very long command with many arguments and words to test the parsing capabilities" "Should handle long commands"

# Mixed Features
compare_with_bash "variable with pipe" "export VAR=\"hello world\" && echo \$VAR | grep hello" "Should handle variables with pipes"
compare_with_bash "redirection with grep" "echo hello > $TEMP_DIR/complex.txt && cat < $TEMP_DIR/complex.txt | grep hello > $TEMP_DIR/result.txt && cat $TEMP_DIR/result.txt" "Should handle redirections with pipes"

# Edge Cases
compare_with_bash "multiple pipes" "cat | cat | ls" "Should handle multiple pipes"
compare_with_bash "empty pipe input" "echo \"\" | cat" "Should handle empty pipe input"

# Clean up temporary files
rm -rf "$TEMP_DIR"

# Print test summary
echo
echo -e "${YELLOW}=== Test Results ===${NC}"
echo "Total tests: $TOTAL_TESTS"
echo -e "Passed: ${GREEN}$TESTS_PASSED${NC}"
echo -e "Failed: ${RED}$TESTS_FAILED${NC}"

# Calculate success rate
if [ $TOTAL_TESTS -gt 0 ]; then
    SUCCESS_RATE=$((TESTS_PASSED * 100 / TOTAL_TESTS))
    echo -e "Success rate: ${SUCCESS_RATE}%"
fi

echo
if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed! Your minishell is working correctly. ✅${NC}"
    echo -e "${GREEN}You're ready for evaluation! 🎉${NC}"
else
    echo -e "${RED}Some tests failed. Check evaluation_results.log for details. ❌${NC}"
fi

echo
echo "Detailed results saved to: evaluation_results.log"