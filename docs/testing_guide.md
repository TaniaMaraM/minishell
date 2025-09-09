# Minishell Testing Guide

## Table of Contents
1. [Quick Start](#quick-start)
2. [Makefile Test Rules](#makefile-test-rules)
3. [Unit Testing](#unit-testing)
4. [Phase Testing](#phase-testing)
5. [Manual Testing](#manual-testing)
6. [Test Development](#test-development)
7. [Troubleshooting](#troubleshooting)

---

## Quick Start

### Run All Tests
```bash
make test                    # Run comprehensive test suite
```

### Run Specific Tests
```bash
make test-unit              # Run unit tests only
make test-phase0            # Run Phase 0 tests
make test-phase1            # Run Phase 1 tests
make test-phase2            # Run Phase 2 tests
```

### Run Tests Manually
```bash
./tests/run_all_tests.sh    # Run all tests
./tests/test_phase0.sh      # Run Phase 0 tests
./tests/test_phase1.sh      # Run Phase 1 tests
./tests/test_phase2.sh      # Run Phase 2 tests
```

---

## Makefile Test Rules

### Available Test Commands

| Command | Description | What It Tests |
|---------|-------------|---------------|
| `make test` | Comprehensive test suite | All phases + unit tests |
| `make test-unit` | Unit tests only | Individual function testing |
| `make test-phase0` | Phase 0 tests | Bootstrap + basic REPL |
| `make test-phase1` | Phase 1 tests | Signal handling + EOF |
| `make test-phase2` | Phase 2 tests | Lexer tokenization |
| `make test-lexer` | Lexer unit tests | Lexer-specific unit tests |

### How the Makefile Rules Work

```makefile
# Comprehensive test suite
test: $(NAME)
	@echo "$(GREEN)[Running comprehensive test suite]$(RESET)"
	@./tests/run_all_tests.sh

# Unit tests (no dependency on main executable)
test-unit:
	@echo "$(GREEN)[Running unit tests]$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) tests/test_lexer_unit.c src/lexeme/*.c src/utils/error.c $(LIBFT) -o test_lexer_unit
	@./test_lexer_unit
	@rm -f test_lexer_unit

# Phase-specific tests (no dependency on main executable)
test-phase0:
	@echo "$(GREEN)[Running Phase 0 tests]$(RESET)"
	@./tests/test_phase0.sh
```

**Key Points:**
- **`test`** depends on `$(NAME)` - ensures minishell is built
- **`test-unit`** and **`test-phase*`** are independent - can run without building main executable
- **Automatic cleanup** - temporary test files are removed
- **Color output** - green for success, red for failure

---

## Unit Testing

### What Are Unit Tests?

**The Analogy: Testing Individual Components**
Think of unit tests like testing individual car parts:
- **Engine test**: Does the engine start?
- **Brake test**: Do the brakes work?
- **Light test**: Do the headlights turn on?

In our case:
- **Lexer test**: Does tokenization work?
- **Quote test**: Are quotes handled correctly?
- **Error test**: Are errors detected?

### Running Unit Tests

```bash
# Using Makefile (recommended)
make test-unit

# Manual compilation and execution
cc -Wall -Wextra -Werror -g -Iinclude -Ilib/libft -Ilib/get_next_line \
   tests/test_lexer_unit.c src/lexeme/*.c src/utils/error.c lib/libft/libft.a -o test_lexer_unit
./test_lexer_unit
rm -f test_lexer_unit
```

### Unit Test Structure

```c
static int test_lexer_basic(void)
{
    t_lexer *lexer;
    t_token *token;
    int passed;
    
    // Setup
    lexer = lexer_init("echo hello world");
    if (!lexer)
        return (0);
    
    // Test
    passed = 1;
    token = lexer_next_token(lexer);
    if (!token || token->type != TOKEN_WORD || ft_strcmp(token->value, "echo"))
        passed = 0;
    
    // Cleanup
    token_destroy(token);
    lexer_destroy(lexer);
    
    return (passed);
}
```

**Test Pattern:**
1. **Setup**: Initialize test data
2. **Test**: Execute function and check result
3. **Cleanup**: Free allocated memory
4. **Return**: Pass (1) or fail (0)

### Unit Test Benefits

✅ **Fast**: Run in milliseconds  
✅ **Isolated**: Test one function at a time  
✅ **Precise**: Pinpoint exact failures  
✅ **Reliable**: Consistent results  
✅ **Development**: Run during coding  

---

## Phase Testing

### What Are Phase Tests?

**The Analogy: Testing Complete Systems**
Think of phase tests like testing complete car systems:
- **Engine system**: Engine + fuel + ignition
- **Braking system**: Brakes + hydraulics + sensors
- **Lighting system**: Headlights + taillights + switches

In our case:
- **Phase 0**: Basic shell + compilation + signals
- **Phase 1**: REPL + signal handling + EOF
- **Phase 2**: Lexer + tokenization + error handling

### Running Phase Tests

```bash
# Individual phase tests
make test-phase0    # Bootstrap and basic functionality
make test-phase1    # Signal handling and EOF
make test-phase2    # Lexer and tokenization

# Or run manually
./tests/test_phase0.sh
./tests/test_phase1.sh
./tests/test_phase2.sh
```

### Phase Test Structure

```bash
# Example from test_phase2.sh
test_basic_tokenization() {
    log_test "Testing basic tokenization..."
    
    if echo "echo hello world" | timeout 3s "$MINISHELL" 2>/dev/null | grep -c "Token:" | grep -q "3"; then
        log_pass "Basic tokenization works (3 tokens)"
    else
        log_fail "Basic tokenization failed"
    fi
}
```

**Test Pattern:**
1. **Setup**: Prepare test environment
2. **Execute**: Run command with test input
3. **Verify**: Check output matches expected result
4. **Report**: Log pass/fail with details

### Phase Test Benefits

✅ **Realistic**: Test actual usage scenarios  
✅ **Comprehensive**: Cover entire features  
✅ **Integration**: Test how components work together  
✅ **User-Focused**: Test what users actually do  
✅ **Confidence**: Verify system works as intended  

---

## Manual Testing

### Interactive Testing

```bash
# Start minishell interactively
./minishell

# Test commands
echo hello world
echo 'hello world'
echo "hello world"
echo hello | grep hello
echo hello > file.txt
echo hello >> file.txt
cat < file.txt
cat << EOF
exit
```

### Non-Interactive Testing

```bash
# Test with echo
echo "echo hello world" | ./minishell

# Test with printf
printf "echo hello\necho world\nexit\n" | ./minishell

# Test with here document
./minishell << EOF
echo hello
echo world
exit
EOF
```

### Signal Testing

```bash
# Test Ctrl+C (interactive)
./minishell
# Type some text, then press Ctrl+C
# Should show new prompt

# Test Ctrl+D (interactive)
./minishell
# Press Ctrl+D
# Should exit with "exit" message
```

### Error Testing

```bash
# Test unterminated quotes
echo "echo 'unterminated" | ./minishell

# Test invalid syntax
echo "echo |" | ./minishell

# Test empty input
echo "" | ./minishell
```

---

## Test Development

### Creating New Unit Tests

1. **Create test file**: `tests/test_new_feature.c`
2. **Add test functions**: Follow the pattern above
3. **Add to Makefile**: Create new test rule
4. **Test thoroughly**: Cover all cases

```c
// Example new unit test
static int test_new_feature(void)
{
    // Setup
    // Test
    // Cleanup
    return (passed);
}

int main(void)
{
    int passed = 0;
    int total = 1;
    
    if (test_new_feature())
    {
        printf("✓ New feature test passed\n");
        passed++;
    }
    else
        printf("✗ New feature test failed\n");
    
    printf("\nResults: %d/%d tests passed\n", passed, total);
    return (passed == total ? 0 : 1);
}
```

### Creating New Phase Tests

1. **Create test file**: `tests/test_phase3.sh`
2. **Add test functions**: Follow the pattern above
3. **Add to Makefile**: Create new test rule
4. **Add to run_all_tests.sh**: Include in comprehensive suite

```bash
# Example new phase test
test_new_phase() {
    log_test "Testing new phase feature..."
    
    if echo "test input" | timeout 3s "$MINISHELL" 2>/dev/null | grep -q "expected output"; then
        log_pass "New phase feature works"
    else
        log_fail "New phase feature failed"
    fi
}
```

### Test Best Practices

**Good Test Names:**
```bash
test_basic_tokenization()     # Clear and specific
test_quote_handling()         # Describes what it tests
test_error_detection()        # Indicates test type
```

**Bad Test Names:**
```bash
test1()                       # Not descriptive
test_lexer()                  # Too vague
test_stuff()                  # Unclear purpose
```

**Test Structure:**
```bash
# Good: Clear setup, test, verification
test_feature() {
    log_test "Testing feature..."
    
    # Setup
    local input="test input"
    local expected="expected output"
    
    # Test
    local actual=$(echo "$input" | ./minishell)
    
    # Verify
    if [ "$actual" = "$expected" ]; then
        log_pass "Feature works"
    else
        log_fail "Feature failed"
    fi
}
```

---

## Troubleshooting

### Common Issues

**1. Tests Not Running**
```bash
# Check if scripts are executable
chmod +x tests/*.sh

# Check if minishell exists
ls -la minishell

# Check if libft is built
ls -la lib/libft/libft.a
```

**2. Compilation Errors**
```bash
# Clean and rebuild
make clean
make

# Check for missing dependencies
make -n test-unit  # See what commands would run
```

**3. Test Failures**
```bash
# Run individual tests to isolate issues
./tests/test_phase0.sh
./tests/test_phase1.sh
./tests/test_phase2.sh

# Check test output for specific errors
make test-phase2 2>&1 | tee test_output.log
```

**4. Memory Issues**
```bash
# Run with valgrind (if available)
valgrind --leak-check=full ./minishell

# Check for memory leaks
echo "echo hello" | valgrind --leak-check=full ./minishell
```

### Debug Mode

**Enable Debug Output:**
```bash
# Add debug prints to your code
printf("DEBUG: Token type: %d, value: %s\n", token->type, token->value);

# Run with debug output
echo "echo hello" | ./minishell 2>&1 | grep DEBUG
```

**Verbose Test Output:**
```bash
# Run tests with verbose output
bash -x ./tests/test_phase2.sh

# Check what commands are being executed
set -x
make test-unit
set +x
```

### Performance Testing

**Time Tests:**
```bash
# Time individual tests
time make test-unit

# Time comprehensive tests
time make test

# Profile with gprof (if compiled with -pg)
gcc -pg -o minishell_debug src/*.c
./minishell_debug
gprof minishell_debug
```

---

## Summary

### Quick Reference

| Task | Command |
|------|---------|
| Run all tests | `make test` |
| Run unit tests | `make test-unit` |
| Run Phase 0 tests | `make test-phase0` |
| Run Phase 1 tests | `make test-phase1` |
| Run Phase 2 tests | `make test-phase2` |
| Run lexer tests | `make test-lexer` |
| Manual testing | `./minishell` |
| Debug mode | `bash -x ./tests/test_phase2.sh` |

### Test Philosophy

**Test Early, Test Often:**
- Run tests after every change
- Add tests for new features
- Fix failing tests immediately
- Use tests to guide development

**Test Quality:**
- Write clear, descriptive test names
- Test both success and failure cases
- Keep tests simple and focused
- Clean up after tests

**Test Coverage:**
- Unit tests for individual functions
- Integration tests for features
- System tests for complete workflows
- Error tests for edge cases

---

*This testing guide ensures you can effectively test your minishell implementation and catch issues early in development.*
