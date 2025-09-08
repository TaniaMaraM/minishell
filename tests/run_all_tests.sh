#!/bin/bash

# Comprehensive Test Runner for Minishell
# Runs all phase tests and provides summary

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test results
TOTAL_PHASES=0
PASSED_PHASES=0

# Helper functions
log_header() {
    echo -e "${BLUE}==========================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}==========================================${NC}"
}

log_phase() {
    echo -e "${YELLOW}Running $1...${NC}"
}

log_success() {
    echo -e "${GREEN}✅ $1 PASSED${NC}"
}

log_failure() {
    echo -e "${RED}❌ $1 FAILED${NC}"
}

# Test Phase 0
test_phase0() {
    log_phase "Phase 0 Tests (Bootstrap + Basic REPL)"
    if ./tests/test_phase0.sh; then
        log_success "Phase 0"
        return 0
    else
        log_failure "Phase 0"
        return 1
    fi
}

# Test Phase 1
test_phase1() {
    log_phase "Phase 1 Tests (REPL + Signal Handling)"
    if ./tests/test_phase1.sh; then
        log_success "Phase 1"
        return 0
    else
        log_failure "Phase 1"
        return 1
    fi
}

# Test Phase 2
test_phase2() {
    log_phase "Phase 2 Tests (Lexer)"
    if ./tests/test_phase2.sh; then
        log_success "Phase 2"
        return 0
    else
        log_failure "Phase 2"
        return 1
    fi
}

# Test Lexer Unit Tests
test_lexer_unit() {
    log_phase "Lexer Unit Tests"
    if cc -Wall -Wextra -Werror -g -Iinclude -Ilib/libft -Ilib/get_next_line \
        tests/test_lexer_unit.c src/lexeme/*.c src/utils/error.c lib/libft/libft.a -o test_lexer_unit && \
        ./test_lexer_unit; then
        log_success "Lexer Unit Tests"
        rm -f test_lexer_unit
        return 0
    else
        log_failure "Lexer Unit Tests"
        rm -f test_lexer_unit
        return 1
    fi
}

# Main test execution
main() {
    log_header "Minishell Comprehensive Test Suite"
    
    # Run all tests
    TOTAL_PHASES=4
    
    if test_phase0; then
        ((PASSED_PHASES++))
    fi
    
    if test_phase1; then
        ((PASSED_PHASES++))
    fi
    
    if test_phase2; then
        ((PASSED_PHASES++))
    fi
    
    if test_lexer_unit; then
        ((PASSED_PHASES++))
    fi
    
    # Final summary
    log_header "Final Test Results"
    echo "Phases Passed: $PASSED_PHASES/$TOTAL_PHASES"
    
    if [ $PASSED_PHASES -eq $TOTAL_PHASES ]; then
        echo -e "${GREEN}🎉 ALL TESTS PASSED! 🎉${NC}"
        echo -e "${GREEN}Your minishell is working perfectly!${NC}"
        exit 0
    else
        echo -e "${RED}❌ Some tests failed. Check the output above.${NC}"
        exit 1
    fi
}

# Run main function
main "$@"
