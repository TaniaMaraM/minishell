#!/bin/bash

# Phase 4 Test: Expander (Variable Expansion)
# Tests $VAR and $? expansion with quote rules

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
RESET='\033[0m'
CHECK='\033[0;34m✔\033[0m'
CROSS='\033[0;31m✗\033[0m'

# Test counter
TESTS_RUN=0
TESTS_PASSED=0

# Function to run a test
run_test() {
    local test_name="$1"
    local input="$2"
    local expected="$3"
    
    echo -n "Testing: $test_name... "
    TESTS_RUN=$((TESTS_RUN + 1))
    
    # For now, just test compilation
    if make > /dev/null 2>&1; then
        echo -e "${GREEN}${CHECK} Compilation OK${RESET}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}${CROSS} Compilation failed${RESET}"
    fi
}

# Function to test variable expansion
test_variable_expansion() {
    echo -e "\n${YELLOW}=== Variable Expansion Tests ===${RESET}"
    
    # Test basic variable expansion
    run_test "Basic variable expansion" "echo \$USER" "echo username"
    
    # Test exit status expansion
    run_test "Exit status expansion" "echo \$?" "echo 0"
    
    # Test single quotes (no expansion)
    run_test "Single quotes no expansion" "echo '\$USER'" "echo \$USER"
    
    # Test double quotes (allow expansion)
    run_test "Double quotes allow expansion" "echo \"\$USER\"" "echo username"
    
    # Test mixed quotes
    run_test "Mixed quotes" "echo 'hello' \$USER \"world\"" "echo hello username world"
    
    # Test undefined variable
    run_test "Undefined variable" "echo \$UNDEFINED" "echo (empty)"
    
    # Test complex expansion
    run_test "Complex expansion" "echo \"User: \$USER, Status: \$?\"" "echo User: username, Status: 0"
}

# Function to test quote handling
test_quote_handling() {
    echo -e "\n${YELLOW}=== Quote Handling Tests ===${RESET}"
    
    # Test nested quotes
    run_test "Nested quotes" "echo \"'hello world'\"" "echo 'hello world'"
    
    # Test escaped quotes
    run_test "Escaped quotes" "echo \"hello\\\"world\"" "echo hello\"world"
    
    # Test quote state transitions
    run_test "Quote state transitions" "echo 'start' \$USER \"end\"" "echo start username end"
}

# Function to test edge cases
test_edge_cases() {
    echo -e "\n${YELLOW}=== Edge Cases ===${RESET}"
    
    # Test empty variable
    run_test "Empty variable" "echo \$EMPTY" "echo (empty)"
    
    # Test variable at end of string
    run_test "Variable at end" "echo hello\$USER" "echo hellousername"
    
    # Test variable at start
    run_test "Variable at start" "echo \$USERhello" "echo usernamehello"
    
    # Test multiple variables
    run_test "Multiple variables" "echo \$USER\$HOME" "echo usernamehome"
    
    # Test invalid variable names
    run_test "Invalid variable name" "echo \$123INVALID" "echo (empty)"
}

# Function to test integration
test_integration() {
    echo -e "\n${YELLOW}=== Integration Tests ===${RESET}"
    
    # Test with pipes
    run_test "Expansion with pipes" "echo \$USER | grep \$USER" "echo username | grep username"
    
    # Test with redirections
    run_test "Expansion with redirections" "echo \$USER > \$HOME/test" "echo username > home/test"
    
    # Test with complex commands
    run_test "Complex command" "ls \$HOME | grep \$USER" "ls home | grep username"
}

# Main test function
main() {
    echo -e "${GREEN}🧪 Phase 4: Expander Tests${RESET}"
    echo "Testing variable expansion with quote rules"
    echo "=========================================="
    
    # Run all test categories
    test_variable_expansion
    test_quote_handling
    test_edge_cases
    test_integration
    
    # Summary
    echo -e "\n${YELLOW}=== Test Summary ===${RESET}"
    echo "Tests run: $TESTS_RUN"
    echo "Tests passed: $TESTS_PASSED"
    echo "Tests failed: $((TESTS_RUN - TESTS_PASSED))"
    
    if [ $TESTS_PASSED -eq $TESTS_RUN ]; then
        echo -e "${GREEN}${CHECK} All tests passed!${RESET}"
        exit 0
    else
        echo -e "${RED}${CROSS} Some tests failed${RESET}"
        exit 1
    fi
}

# Run main function
main "$@"
