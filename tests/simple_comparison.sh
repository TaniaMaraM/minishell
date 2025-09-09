g#!/bin/bash

# ╔════════════════════════════════════════════════════════════════════════════╗
# ║                                                                            ║
# ║                    MINISHELL vs BASH COMPARISON TESTER                     ║
# ║                                                                            ║
# ║  Simple OK/KO comparison between bash and minishell                        ║
# ║                                                                            ║
# ╚════════════════════════════════════════════════════════════════════════════╝

# Configuration
MINISHELL_PATH="./minishell"
TEMP_DIR="/tmp/minishell_comparison_$$"
RESULTS_FILE="comparison_results.log"

# Colors
GREEN="\033[1;32m"
RED="\033[1;31m"
BLUE="\033[1;34m"
YELLOW="\033[1;33m"
RESET="\033[0m"
BOLD="\033[1m"

# Create temp directory
mkdir -p "$TEMP_DIR"

# Test cases organized by category
declare -A TEST_CASES

# Basic Command Execution
TEST_CASES["BASIC_COMMANDS"]="
/bin/ls -la
/bin/pwd
/bin/echo hello world
ls -l
echo hello world
cat Makefile
"

# Builtins
TEST_CASES["BUILTINS"]="
echo
echo hello world
echo -n hello
cd /tmp && pwd
cd .. && pwd
pwd
export TESTVAR=hello && echo \$TESTVAR
unset TESTVAR && echo \$TESTVAR
env | grep PATH
exit 0 || echo \$?
"

# Environment Variables
TEST_CASES["ENVIRONMENT"]="
echo \$HOME
echo \$USER
echo \$UNDEFINED_VAR
/bin/true && echo \$?
/bin/false && echo \$?
export VAR=\"hello world\" && echo \"\$VAR\"
export VAR=hello && echo \$VAR\$VAR
"

# Quotes
TEST_CASES["QUOTES"]="
echo 'hello world'
echo '\$USER'
echo \"hello world\"
echo \"\$USER\"
echo \"hello 'world'\"
echo 'hello \"world\"'
echo \"hello\"'world'
"

# Redirections
TEST_CASES["REDIRECTIONS"]="
echo test > $TEMP_DIR/test.txt && cat $TEMP_DIR/test.txt
echo hello > $TEMP_DIR/append.txt && echo world >> $TEMP_DIR/append.txt && cat $TEMP_DIR/append.txt
cat < $TEMP_DIR/test.txt
"

# Pipes
TEST_CASES["PIPES"]="
echo hello | cat
echo hello world | grep hello
echo hello | cat | grep hello
ls -l | grep Makefile
"

# Error Handling
TEST_CASES["ERRORS"]="
nonexistentcmd 2>&1 | grep -o 'not found'
ls /nonexistent 2>&1 | grep -o 'No such file'
"

# Complex Cases
TEST_CASES["COMPLEX"]="
export VAR=world && echo \"hello \$VAR\" | grep world
echo hello > $TEMP_DIR/complex.txt && cat < $TEMP_DIR/complex.txt | grep hello
"

# Function to run a test and compare outputs
run_test() {
    local command="$1"
    local category="$2"
    
    # Skip empty lines
    if [ -z "$command" ]; then
        return
    fi
    
    # Get bash output and exit code
    local bash_output
    local bash_exit
    bash_output=$(bash -c "$command" 2>&1)
    bash_exit=$?
    
    # Get minishell output and exit code
    local minishell_output
    local minishell_exit
    minishell_output=$(echo "$command" | "$MINISHELL_PATH" 2>&1)
    minishell_exit=$?
    
    # Compare results (allowing for command not found message differences)
    local result="KO"
    if [ "$bash_output" = "$minishell_output" ] && [ "$bash_exit" = "$minishell_exit" ]; then
        result="OK"
    elif echo "$bash_output" | grep -q "command not found" && echo "$minishell_output" | grep -q "command not found" && [ "$bash_exit" = "$minishell_exit" ]; then
        result="OK"
    fi
    
    # Print result
    if [ "$result" = "OK" ]; then
        echo -e "${BLUE}[$category]${RESET} ${command} ${GREEN}OK${RESET}"
        echo "[$category] $command: OK" >> "$RESULTS_FILE"
    else
        echo -e "${BLUE}[$category]${RESET} ${command} ${RED}KO${RESET}"
        echo "[$category] $command: KO" >> "$RESULTS_FILE"
        echo "  bash ($bash_exit): $bash_output" >> "$RESULTS_FILE"
        echo "  minishell ($minishell_exit): $minishell_output" >> "$RESULTS_FILE"
    fi
    
    return $([[ "$result" = "OK" ]] && echo 0 || echo 1)
}

# Main function
main() {
    # Check if minishell exists
    if [ ! -f "$MINISHELL_PATH" ]; then
        echo -e "${RED}Error: Minishell not found at $MINISHELL_PATH${RESET}"
        echo -e "Running ${BOLD}make${RESET} to build it..."
        make
        
        if [ ! -f "$MINISHELL_PATH" ]; then
            echo -e "${RED}Error: Failed to build minishell. Exiting.${RESET}"
            exit 1
        fi
    fi
    
    # Initialize results file
    echo "MINISHELL vs BASH COMPARISON RESULTS" > "$RESULTS_FILE"
    echo "==================================" >> "$RESULTS_FILE"
    echo "Date: $(date)" >> "$RESULTS_FILE"
    echo "" >> "$RESULTS_FILE"
    
    # Track statistics
    local total=0
    local passed=0
    local failed=0
    
    # Run all test categories
    for category in "${!TEST_CASES[@]}"; do
        echo -e "\n${YELLOW}${BOLD}Testing $category${RESET}"
        echo "----------------------------------------"
        
        while IFS= read -r cmd; do
            if [ -n "$cmd" ]; then
                ((total++))
                run_test "$cmd" "$category"
                if [ $? -eq 0 ]; then
                    ((passed++))
                else
                    ((failed++))
                fi
            fi
        done <<< "${TEST_CASES[$category]}"
    done
    
    # Print summary
    echo -e "\n${BOLD}TEST SUMMARY${RESET}"
    echo "----------------------------------------"
    echo -e "Total tests:  $total"
    echo -e "Passed:       ${GREEN}$passed${RESET}"
    echo -e "Failed:       ${RED}$failed${RESET}"
    
    # Calculate percentage
    if [ $total -gt 0 ]; then
        local pass_percent=$((passed * 100 / total))
        echo -e "Success rate: $pass_percent%"
        
        # Add summary to results file
        echo "" >> "$RESULTS_FILE"
        echo "SUMMARY" >> "$RESULTS_FILE"
        echo "=======" >> "$RESULTS_FILE"
        echo "Total tests: $total" >> "$RESULTS_FILE"
        echo "Passed: $passed" >> "$RESULTS_FILE"
        echo "Failed: $failed" >> "$RESULTS_FILE"
        echo "Success rate: $pass_percent%" >> "$RESULTS_FILE"
        
        # Final verdict
        if [ $failed -eq 0 ]; then
            echo -e "\n${GREEN}${BOLD}All tests passed! Your minishell is working correctly.${RESET}"
        else
            echo -e "\n${RED}${BOLD}Some tests failed. Check the results file for details.${RESET}"
        fi
    fi
    
    # Clean up
    rm -rf "$TEMP_DIR"
    
    echo -e "\nResults saved to: $RESULTS_FILE"
}

# Run the main function
main "$@"
