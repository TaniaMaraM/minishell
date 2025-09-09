# Minishell Testing Strategy

## Table of Contents
1. [Testing Philosophy](#testing-philosophy)
2. [Test Structure](#test-structure)
3. [Phase-Specific Tests](#phase-specific-tests)
4. [Unit Testing](#unit-testing)
5. [Integration Testing](#integration-testing)
6. [Key Learnings](#key-learnings)
7. [Testing Best Practices](#testing-best-practices)

---

## Testing Philosophy

### Why Testing is Crucial

**The Analogy: Building a House**
Think of testing like inspecting a house during construction:
- **Foundation Tests** (Phase 0): Check if the foundation is solid
- **Structure Tests** (Phase 1): Verify walls and framework
- **System Tests** (Phase 2+): Test plumbing, electrical, etc.
- **Final Inspection**: Everything works together

### Testing Pyramid

```
        🔺 Integration Tests (Few, Complex)
       🔺🔺 System Tests (Some, Medium)
      🔺🔺🔺 Unit Tests (Many, Simple)
```

**Unit Tests**: Test individual functions in isolation
**System Tests**: Test complete features end-to-end
**Integration Tests**: Test how components work together

---

## Test Structure

### Directory Organization

```
tests/
├── test_phase0.sh      # Bootstrap and basic REPL tests
├── test_phase1.sh      # Signal handling and EOF tests
├── test_phase2.sh      # Lexer tokenization tests
├── test_lexer_unit.c   # Unit tests for lexer functions
└── run_all_tests.sh    # Comprehensive test runner
```

### Test Categories

1. **Functional Tests**: Does the feature work as expected?
2. **Error Tests**: Does it handle errors gracefully?
3. **Edge Case Tests**: Does it handle unusual inputs?
4. **Performance Tests**: Does it work efficiently?
5. **Memory Tests**: Are there memory leaks?

---

## Phase-Specific Tests

### Phase 0: Bootstrap Tests

**What We Test:**
- ✅ Compilation with strict flags
- ✅ Basic REPL functionality
- ✅ Signal handling setup
- ✅ Memory cleanup

**Key Test Cases:**
```bash
# Compilation test
make clean && make

# Basic REPL test
echo "exit" | ./minishell

# Signal handling test
timeout 3s ./minishell  # Should handle SIGTERM gracefully
```

**Why These Tests Matter:**
- **Compilation**: Ensures code follows standards
- **REPL**: Verifies basic shell loop works
- **Signals**: Critical for user experience
- **Memory**: Prevents crashes and leaks

### Phase 1: Signal Handling Tests

**What We Test:**
- ✅ EOF handling (Ctrl+D)
- ✅ Interactive signal handling (Ctrl+C)
- ✅ Non-interactive signal handling
- ✅ History management
- ✅ Environment variable handling

**Key Test Cases:**
```bash
# EOF test
printf "echo hello\n\004" | ./minishell  # Should print "exit"

# Signal test
{ sleep 1; kill -INT $$; } | ./minishell  # Should handle gracefully

# History test
printf "\n\necho hello\n" | ./minishell  # Should skip empty lines
```

**Why These Tests Matter:**
- **EOF**: Users expect clean exit on Ctrl+D
- **Signals**: Shell must respond to interruptions
- **History**: Empty lines shouldn't pollute history
- **Environment**: Shell needs proper environment setup

### Phase 2: Lexer Tests

**What We Test:**
- ✅ Basic tokenization
- ✅ Quote handling (single and double)
- ✅ Metacharacter recognition
- ✅ Error detection
- ✅ Complex commands
- ✅ Whitespace handling

**Key Test Cases:**
```bash
# Basic tokenization
echo "echo hello world" | ./minishell  # Should produce 3 tokens

# Quote handling
echo "echo 'hello world'" | ./minishell  # Should produce 2 tokens

# Metacharacters
echo "echo hello | grep hello" | ./minishell  # Should produce 5 tokens

# Error detection
echo "echo 'unterminated" | ./minishell  # Should detect error
```

**Why These Tests Matter:**
- **Tokenization**: Foundation for all command processing
- **Quotes**: Critical for handling arguments with spaces
- **Metacharacters**: Essential for pipes and redirections
- **Errors**: Must catch syntax errors early

---

## Unit Testing

### Lexer Unit Tests

**What We Test:**
- Individual lexer functions
- Token creation and destruction
- Edge cases in isolation
- Memory management

**Test Structure:**
```c
static int test_lexer_basic(void)
{
    t_lexer *lexer;
    t_token *token;
    int passed;
    
    // Setup
    lexer = lexer_init("echo hello world");
    
    // Test
    token = lexer_next_token(lexer);
    passed = (token && token->type == TOKEN_WORD);
    
    // Cleanup
    token_destroy(token);
    lexer_destroy(lexer);
    
    return passed;
}
```

**Benefits of Unit Tests:**
- **Fast**: Run quickly during development
- **Isolated**: Test one function at a time
- **Precise**: Pinpoint exact failures
- **Reliable**: Consistent results

---

## Integration Testing

### End-to-End Tests

**What We Test:**
- Complete command processing pipeline
- Multiple commands in sequence
- Complex shell interactions
- Real-world usage scenarios

**Example Test:**
```bash
# Complex command test
echo "echo 'hello world' | grep hello > output.txt" | ./minishell
# Should tokenize correctly and handle all components
```

**Benefits of Integration Tests:**
- **Realistic**: Test actual usage patterns
- **Comprehensive**: Cover entire system
- **User-Focused**: Test what users actually do
- **Confidence**: Verify system works as intended

---

## Key Learnings

### 1. Test-Driven Development (TDD)

**The Process:**
1. **Write Test First**: Define expected behavior
2. **Run Test**: Should fail (red)
3. **Write Code**: Implement to pass test
4. **Run Test**: Should pass (green)
5. **Refactor**: Improve code while keeping tests green

**Benefits:**
- **Clear Requirements**: Tests define what code should do
- **Better Design**: Forces you to think about interfaces
- **Regression Prevention**: Catch bugs when refactoring
- **Documentation**: Tests serve as living documentation

### 2. Testing Strategy

**The Analogy: Quality Control**
Think of testing like quality control in manufacturing:
- **Incoming Inspection**: Test each component (unit tests)
- **In-Process Inspection**: Test during assembly (integration tests)
- **Final Inspection**: Test complete product (system tests)
- **Field Testing**: Test in real conditions (user acceptance tests)

### 3. Error Testing

**Why Test Errors:**
- **Real-World**: Users make mistakes
- **Security**: Prevent crashes and exploits
- **Reliability**: System must handle unexpected input
- **User Experience**: Graceful error handling

**Error Test Categories:**
- **Invalid Input**: Malformed commands
- **Resource Limits**: Memory, file descriptors
- **System Errors**: Permission denied, file not found
- **Edge Cases**: Empty input, very long input

### 4. Memory Testing

**Why Memory Tests Matter:**
- **Stability**: Prevent crashes from memory issues
- **Performance**: Avoid memory leaks that slow system
- **Security**: Prevent buffer overflows
- **Professionalism**: Clean code is expected

**Memory Test Tools:**
- **Valgrind**: Detects memory leaks and errors
- **AddressSanitizer**: Catches memory corruption
- **Custom Tests**: Verify proper cleanup

### 5. Signal Testing

**Why Signal Tests Are Critical:**
- **User Experience**: Shell must respond to Ctrl+C
- **System Integration**: Must work with terminal
- **Reliability**: Handle interruptions gracefully
- **Standards Compliance**: Follow shell conventions

**Signal Test Challenges:**
- **Timing**: Signals are asynchronous
- **Environment**: Different behavior in different contexts
- **Platform**: Signal handling varies by OS
- **Complexity**: Multiple signals can interact

---

## Testing Best Practices

### 1. Test Organization

**File Naming:**
- `test_phase0.sh` - Phase-specific tests
- `test_lexer_unit.c` - Unit tests for specific module
- `run_all_tests.sh` - Comprehensive test runner

**Test Structure:**
```bash
# Setup
setup_test_environment

# Test
run_test_case

# Verify
check_results

# Cleanup
cleanup_test_environment
```

### 2. Test Data Management

**Use Temporary Files:**
```bash
TEMP_DIR="/tmp/minishell_test_$$"
mkdir -p "$TEMP_DIR"
# ... use temp directory ...
rm -rf "$TEMP_DIR"
```

**Avoid Side Effects:**
- Don't modify system files
- Use temporary directories
- Clean up after tests
- Don't depend on external state

### 3. Test Output

**Clear Reporting:**
```bash
log_test() {
    echo -e "${YELLOW}[TEST]${NC} $1"
}

log_pass() {
    echo -e "${GREEN}[PASS]${NC} $1"
}

log_fail() {
    echo -e "${RED}[FAIL]${NC} $1"
}
```

**Color Coding:**
- **Green**: Success
- **Red**: Failure
- **Yellow**: Test in progress
- **Blue**: Section headers

### 4. Test Maintenance

**Keep Tests Simple:**
- One test per concept
- Clear test names
- Minimal setup
- Easy to understand

**Update Tests with Code:**
- When adding features, add tests
- When fixing bugs, add regression tests
- When refactoring, update tests
- When removing features, remove tests

### 5. Continuous Testing

**Run Tests Frequently:**
- Before committing code
- After making changes
- During development
- Before releases

**Automate Testing:**
- Make tests runnable with simple commands
- Provide clear pass/fail results
- Include in build process
- Set up CI/CD if possible

---

## Running Tests

### Individual Phase Tests
```bash
./tests/test_phase0.sh    # Test Phase 0
./tests/test_phase1.sh    # Test Phase 1
./tests/test_phase2.sh    # Test Phase 2
```

### Unit Tests
```bash
# Compile and run unit tests
cc -Wall -Wextra -Werror -g -Iinclude -Ilib/libft -Ilib/get_next_line \
   tests/test_lexer_unit.c src/lexeme/*.c lib/libft/libft.a -o test_lexer_unit
./test_lexer_unit
```

### All Tests
```bash
./tests/run_all_tests.sh  # Run comprehensive test suite
```

### Makefile Integration
```makefile
test: $(NAME)
	@./tests/run_all_tests.sh

test-phase0: $(NAME)
	@./tests/test_phase0.sh

test-phase1: $(NAME)
	@./tests/test_phase1.sh

test-phase2: $(NAME)
	@./tests/test_phase2.sh
```

---

*This testing strategy ensures your minishell is robust, reliable, and ready for evaluation. Regular testing during development prevents bugs and builds confidence in your implementation.*
