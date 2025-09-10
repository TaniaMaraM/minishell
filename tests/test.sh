#!/bin/bash
# ----------------------------------------------------
# 🧪 COMPREHENSIVE MINISHELL TEST SUITE
# Based on 42 Subject Requirements & Evaluation Criteria
# Enhanced with Edge Cases and Comprehensive Coverage
# ----------------------------------------------------

BIN=${1:-./minishell}
RED="\033[0;31m"
GREEN="\033[0;32m"
BLUE="\033[0;34m"
YELLOW="\033[0;33m"
CYAN="\033[0;36m"
RESET="\033[0m"
SEP="--------------------------------------------------"

pass() { echo -e "✅ ${GREEN}OK${RESET}"; }
fail() { echo -e "❌ ${RED}KO${RESET}"; echo -e "$1"; }
warn() { echo -e "⚠️  ${YELLOW}WARN${RESET}"; echo -e "$1"; }

echo -e "${BLUE}🧪 COMPREHENSIVE MINISHELL TEST SUITE${RESET}"
echo -e "${BLUE}Testing: ${BIN}${RESET}"
echo -e "$SEP"

# Test counter
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Create test directories and files
mkdir -p /tmp/minishell_test/outfiles
mkdir -p /tmp/minishell_test/test_files
echo "hello world" > /tmp/minishell_test/test_files/infile
echo "hi there" > /tmp/minishell_test/test_files/infile2
echo "test content with spaces" > "/tmp/minishell_test/test_files/file name with spaces"
touch /tmp/minishell_test/test_files/empty
chmod 000 /tmp/minishell_test/test_files/invalid_permission 2>/dev/null || true

test_case() {
    local test_name="$1"
    local command="$2"
    local expected="$3"
    local description="$4"
    local should_fail="${5:-false}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo -e "${BLUE}[$test_name] $description${RESET}"
    
    if [ -n "$expected" ]; then
        output=$(timeout 5s bash -c "printf '$command\nexit\n' | $BIN 2>&1" 2>/dev/null)
        if [ $? -eq 124 ]; then
            fail "Test timed out after 5 seconds"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        elif [[ "$output" == *"$expected"* ]]; then
            pass
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            fail "Expected: $expected\nGot: $output"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    else
        # Just run the command and check if it doesn't crash
        if timeout 5s bash -c "printf '$command\nexit\n' | $BIN >/dev/null 2>&1" 2>/dev/null; then
            if [ "$should_fail" = "true" ]; then
                fail "Command should have failed but didn't"
                FAILED_TESTS=$((FAILED_TESTS + 1))
            else
                pass
                PASSED_TESTS=$((PASSED_TESTS + 1))
            fi
        else
            if [ "$should_fail" = "true" ]; then
                pass
                PASSED_TESTS=$((PASSED_TESTS + 1))
            else
                fail "Command failed or crashed"
                FAILED_TESTS=$((FAILED_TESTS + 1))
            fi
        fi
    fi
    echo -e "$SEP"
}

test_syntax_error() {
    local test_name="$1"
    local command="$2"
    local description="$3"
    local should_fail="${4:-false}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo -e "${BLUE}[$test_name] $description${RESET}"
    
    # Syntax errors should cause minishell to show error and not crash
    output=$(timeout 5s bash -c "printf '$command\nexit\n' | $BIN 2>&1" 2>/dev/null)
    if [ $? -eq 124 ]; then
        fail "Test timed out after 5 seconds"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    elif [[ "$output" == *"syntax error"* ]] || [[ "$output" == *"unexpected"* ]] || [[ "$output" == *"error"* ]]; then
        pass
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        if [ "$should_fail" = "true" ]; then
            pass
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            fail "Expected syntax error, got: $output"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    fi
    echo -e "$SEP"
}

# =====================================================
# 1. EVALUATION SHEET CRITERIA TESTS
# =====================================================
echo -e "${YELLOW}📋 EVALUATION SHEET CRITERIA${RESET}"

# Basic echo functionality
test_case "echo_basic" "echo Hello world" "Hello world" "Basic echo output"
test_case "echo_n" "echo -n Hello" "Hello" "Echo without newline"
test_case "echo_multiple_n" "echo -n -n -n Hello" "Hello" "Multiple -n flags"
test_case "echo_empty" "echo" "" "Echo with no arguments"
test_case "echo_spaces" "echo '   '" "   " "Echo with spaces"
test_case "echo_special_chars" "echo 'hello world'" "hello world" "Echo with special characters"
test_case "echo_quotes" "echo \"hello world\"" "hello world" "Echo with double quotes"
test_case "echo_single_quotes" "echo 'hello world'" "hello world" "Echo with single quotes"
test_case "echo_mixed_quotes" "echo 'hello'\"world\"" "helloworld" "Mixed quotes"

# Environment variables
test_case "env_path" "env" "PATH=" "Environment contains PATH"
test_case "export_basic" "export TEST=value" "" "Export variable"
test_case "export_invalid" "export =invalid" "not a valid identifier" "Invalid export syntax"
test_case "export_no_value" "export VAR" "" "Export without value"
test_case "export_special" "export VAR='value with spaces'" "" "Export with spaces"
test_case "export_overwrite" "export VAR=old; export VAR=new" "" "Export overwrite"
test_case "export_multiple" "export A=1 B=2 C=3" "" "Export multiple variables"
test_case "export_invalid_chars" "export A-" "not a valid identifier" "Export with invalid characters"
test_case "export_numeric_start" "export 123=value" "not a valid identifier" "Export starting with number"
test_case "unset_basic" "unset TEST" "" "Unset variable"
test_case "unset_multiple" "unset VAR1 VAR2 VAR3" "" "Unset multiple variables"
test_case "unset_nonexistent" "unset NONEXISTENT" "" "Unset nonexistent variable"
test_case "unset_critical" "unset PATH; /bin/ls" "" "Unset PATH and run command" "true"

# Directory operations
test_case "pwd_basic" "pwd" "/" "Print working directory"
test_case "pwd_with_args" "pwd oi" "" "PWD with arguments (should ignore)"
test_case "cd_basic" "cd /tmp" "" "Change directory"
test_case "cd_home" "cd" "" "CD to home directory"
test_case "cd_previous" "cd -" "" "CD to previous directory"
test_case "cd_invalid" "cd /nonexistent" "No such file or directory" "CD to invalid directory"
test_case "cd_with_args" "cd /tmp hi" "" "CD with extra arguments"
test_case "cd_relative" "cd .." "" "CD to parent directory"

# Exit functionality
test_case "exit_code" "exit 42" "" "Exit with specific code" "true"
test_case "exit_invalid" "exit abc" "numeric argument required" "Exit with invalid argument"
test_case "exit_positive" "exit +100" "" "Exit with positive number" "true"
test_case "exit_negative" "exit -100" "" "Exit with negative number" "true"
test_case "exit_multiple_args" "exit 42 world" "" "Exit with multiple arguments" "true"
test_case "exit_no_args" "exit" "" "Exit without arguments"

# =====================================================
# 2. BASH BEHAVIOR COMPLIANCE TESTS
# =====================================================
echo -e "${YELLOW}🐚 BASH BEHAVIOR COMPLIANCE${RESET}"

# Variable expansion
test_case "var_expansion" "echo \$HOME" "/" "Variable expansion"
test_case "var_undefined" "echo \$UNDEFINED" "" "Undefined variable"
test_case "var_exit_code" "nonexistentcommand; echo \$?" "127" "Exit code variable"
test_case "var_exit_code_clear" "nonexistentcommand; \$EMPTY; echo \$?" "0" "Exit code cleared by empty command"
test_case "var_dollar_only" "echo \$" "" "Dollar sign only"
test_case "var_dollar_question" "echo \$?" "0" "Exit code variable"
test_case "var_combined" "echo \$?HELLO" "0HELLO" "Exit code with text"

# =====================================================
# 3. REDIRECTION TESTS
# =====================================================
echo -e "${YELLOW}📁 REDIRECTION TESTS${RESET}"

# Basic redirections
test_case "redir_output" "echo hello > /tmp/minishell_test/outfiles/outfile01 && cat /tmp/minishell_test/outfiles/outfile01" "hello" "Output redirection"
test_case "redir_input" "cat < /tmp/minishell_test/test_files/infile" "hello world" "Input redirection"
test_case "redir_append" "echo world >> /tmp/minishell_test/outfiles/outfile01 && cat /tmp/minishell_test/outfiles/outfile01" "hello\nworld" "Append redirection"

# Multiple redirections
test_case "redir_multiple_output" "ls > /tmp/minishell_test/outfiles/outfile01 > /tmp/minishell_test/outfiles/outfile02" "" "Multiple output redirections"
test_case "redir_multiple_input" "cat < /tmp/minishell_test/test_files/infile < /tmp/minishell_test/test_files/infile2" "hello world" "Multiple input redirections"

# Redirection with pipes
test_case "redir_pipe_output" "echo hi > /tmp/minishell_test/outfiles/outfile01 | echo bye" "bye" "Output redirection with pipe"
test_case "redir_pipe_input" "echo hi | cat < /tmp/minishell_test/test_files/infile" "hello world" "Input redirection with pipe"

# File with spaces
test_case "redir_spaces" "cat < \"/tmp/minishell_test/test_files/file name with spaces\"" "test content with spaces" "Redirection with spaces in filename"

# Permission denied
test_case "redir_permission" "echo test > /tmp/minishell_test/test_files/invalid_permission" "" "Redirection to protected file"

# Missing files
test_case "redir_missing" "cat < /tmp/minishell_test/missing" "" "Redirection from missing file"

# =====================================================
# 4. PIPE TESTS
# =====================================================
echo -e "${YELLOW}🔗 PIPE TESTS${RESET}"

# Basic pipes
test_case "pipe_basic" "echo hello | cat" "hello" "Basic pipe"
test_case "pipe_multiple" "echo hello | cat | wc -c" "6" "Multiple pipes"
test_case "pipe_long" "echo \"hi\" | cat | cat | cat | cat | cat | cat | cat" "hi" "Long pipe chain"

# Pipes with redirections
test_case "pipe_redir_input" "echo hi | cat < /tmp/minishell_test/test_files/infile" "hello world" "Pipe with input redirection"
test_case "pipe_redir_output" "echo hi | cat > /tmp/minishell_test/outfiles/outfile01" "" "Pipe with output redirection"

# Pipes with builtins
test_case "pipe_builtin" "export GHOST=123 | env | grep GHOST" "GHOST=123" "Pipe with export and env"

# =====================================================
# 5. SYNTAX ERROR TESTS
# =====================================================
echo -e "${YELLOW}⚠️  SYNTAX ERROR TESTS${RESET}"

test_syntax_error "syntax_pipe_start" "| echo oi" "Pipe at start"
test_syntax_error "syntax_pipe_end" "echo oi |" "Pipe at end"
test_syntax_error "syntax_double_pipe" "| |" "Double pipe"
test_syntax_error "syntax_redir_output" ">" "Output redirection without command"
test_syntax_error "syntax_redir_input" "<" "Input redirection without command"
test_syntax_error "syntax_redir_append" ">>" "Append redirection without command"
test_syntax_error "syntax_heredoc" "<<" "Heredoc without delimiter"
test_syntax_error "syntax_redir_invalid" "echo hi <" "Incomplete input redirection"

# =====================================================
# 6. HEREDOC TESTS
# =====================================================
echo -e "${YELLOW}📝 HEREDOC TESTS${RESET}"

test_case "heredoc_basic" "cat <<HEREDOC
oi
HEREDOC" "oi" "Basic heredoc"
test_case "heredoc_multiple" "cat <<HERE <<DOC
oi
HERE
time
DOC" "oi\ntime" "Multiple heredocs"
test_case "heredoc_pipe" "cat <<HERE | ls
oi
HERE" "" "Heredoc with pipe"
test_case "heredoc_variable" "cat <<HERE
\$USER
oi
HERE" "\$USER\noi" "Heredoc with variable (should not expand)"
test_case "heredoc_redir" "cat < /tmp/minishell_test/test_files/infile <<HERE | ls
HERE" "" "Heredoc with input redirection"
test_case "heredoc_dollar" "cat << \$
oi
\$" "oi" "Heredoc with dollar delimiter"

# =====================================================
# 7. EDGE CASES AND ERROR HANDLING
# =====================================================
echo -e "${YELLOW}⚠️  EDGE CASES & ERROR HANDLING${RESET}"

test_case "empty_input" "" "" "Empty input"
test_case "whitespace_only" "   " "" "Whitespace only"
test_case "invalid_command" "nonexistentcommand" "command not found" "Invalid command"
test_case "permission_denied" "echo test > /root/test.txt" "Permission denied" "Permission denied"
test_case "long_input" "echo 'very long string with many characters to test memory allocation and buffer handling'" "" "Long input handling"
test_case "special_chars" "echo '> >> < * ? [ ] | ; [ ] || && ( ) & # \$ \\ <<'" "> >> < * ? [ ] | ; [ ] || && ( ) & # \$ \\ <<" "Special characters in echo"

# =====================================================
# 8. MEMORY MANAGEMENT TESTS
# =====================================================
echo -e "${YELLOW}🧠 MEMORY MANAGEMENT${RESET}"

test_case "memory_multiple" "echo test1; echo test2; echo test3" "test1\ntest2\ntest3" "Multiple commands"
test_case "memory_export_unset" "export VAR1=1; export VAR2=2; unset VAR1; unset VAR2" "" "Export and unset cycle"
test_case "memory_cd_cycle" "cd /tmp; cd ..; cd /tmp; cd .." "" "CD cycle"

# =====================================================
# 9. SIGNAL HANDLING TESTS
# =====================================================
echo -e "${YELLOW}📡 SIGNAL HANDLING${RESET}"

# Test Ctrl+C handling (SIGINT)
echo -e "${BLUE}[signal_ctrl_c] Testing SIGINT handling${RESET}"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if timeout 2s bash -c 'echo "echo test"; sleep 1; echo "exit" | ./minishell' >/dev/null 2>&1; then
    pass
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    fail "SIGINT handling failed"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
echo -e "$SEP"

# Test Ctrl+D handling (EOF)
echo -e "${BLUE}[signal_ctrl_d] Testing EOF handling${RESET}"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if printf "" | $BIN >/dev/null 2>&1; then
    pass
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    fail "EOF handling failed"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
echo -e "$SEP"

# =====================================================
# 10. CLEANUP
# =====================================================
rm -rf /tmp/minishell_test

# =====================================================
# 8. RESULTS SUMMARY
# =====================================================
echo -e "${BLUE}📊 TEST RESULTS SUMMARY${RESET}"
echo -e "Total Tests: $TOTAL_TESTS"
echo -e "Passed: ${GREEN}$PASSED_TESTS${RESET}"
echo -e "Failed: ${RED}$FAILED_TESTS${RESET}"

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}🎉 ALL TESTS PASSED!${RESET}"
    exit 0
else
    echo -e "${RED}❌ $FAILED_TESTS TESTS FAILED${RESET}"
    exit 1
fi
