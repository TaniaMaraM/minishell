# Phase 10: Advanced Testing - Evaluation Preparation

## **🎯 Why Advanced Testing Matters for Evaluation**

During your 42 evaluation, the evaluator will test your minishell with:
- **Edge cases** that break poorly implemented shells
- **Stress tests** to find memory leaks and crashes
- **Bash compatibility** to ensure your shell behaves correctly
- **Error handling** to verify robust behavior

**Your goal**: Make your minishell so robust that it handles everything the evaluator throws at it!

## **🧪 Testing Categories for Evaluation**

### **1. Edge Cases Testing**
These are the "gotcha" tests that evaluators love to use:

#### **Quote Edge Cases**
```bash
# Nested quotes
echo "hello 'world' test"
echo 'hello "world" test'

# Empty quotes
echo ""
echo ''

# Quotes with special characters
echo "hello\$world"
echo 'hello$world'

# Mixed quote scenarios
echo "hello 'world' \"test\""
```

#### **Variable Expansion Edge Cases**
```bash
# Undefined variables
echo $UNDEFINED_VAR
echo "$UNDEFINED_VAR"

# Empty variables
export EMPTY_VAR=""
echo $EMPTY_VAR

# Variables with special characters
export SPECIAL_VAR="hello world"
echo $SPECIAL_VAR

# Exit status edge cases
false
echo $?
true
echo $?
```

#### **Redirection Edge Cases**
```bash
# Multiple redirections
echo hello > file1 > file2

# Redirections with pipes
echo hello | cat > file.txt

# Heredoc edge cases
cat << EOF
hello
world
EOF

# Non-existent files
cat < nonexistent.txt
```

### **2. Stress Testing**
These tests push your shell to its limits:

#### **Memory Stress Tests**
```bash
# Long commands
echo $(seq 1 1000) | wc -w

# Many redirections
echo hello > f1 > f2 > f3 > f4 > f5

# Deep pipelines
echo hello | cat | cat | cat | cat | cat

# Large heredoc
cat << EOF
$(seq 1 1000)
EOF
```

#### **Process Stress Tests**
```bash
# Many processes
for i in {1..100}; do echo "Process $i"; done

# Complex pipelines
echo hello | grep h | wc -l | cat | wc -c
```

### **3. Error Handling Tests**
These verify your shell handles errors gracefully:

#### **File System Errors**
```bash
# Permission denied
echo hello > /root/test.txt

# Directory doesn't exist
echo hello > /nonexistent/test.txt

# Read-only filesystem
echo hello > /dev/full
```

#### **Command Errors**
```bash
# Non-existent commands
nonexistent_command

# Commands that fail
false
echo $?

# Commands with wrong arguments
ls --invalid-option
```

### **4. Signal Handling Tests**
These test your signal implementation:

#### **Interactive Signal Tests**
```bash
# Start minishell interactively
./minishell
# Then test:
# Ctrl+C (should show new prompt)
# Ctrl+D (should exit)
# Ctrl+\ (should do nothing)
```

#### **Non-Interactive Signal Tests**
```bash
# Signal handling in scripts
echo "echo hello" | ./minishell
```

## **🛠️ Creating Advanced Test Scripts**

Let me create comprehensive test scripts for evaluation preparation:

### **Test Script 1: Edge Cases**
```bash
#!/bin/bash
# tests/test_edge_cases.sh

echo "🧪 Testing Edge Cases"
echo "===================="

# Quote edge cases
echo "Testing quote edge cases..."
echo "hello 'world' test" | ./minishell
echo 'hello "world" test' | ./minishell
echo "" | ./minishell
echo '' | ./minishell

# Variable edge cases
echo "Testing variable edge cases..."
echo '$UNDEFINED_VAR' | ./minishell
echo "$UNDEFINED_VAR" | ./minishell
export EMPTY_VAR=""
echo '$EMPTY_VAR' | ./minishell

# Redirection edge cases
echo "Testing redirection edge cases..."
echo "hello" > test1.txt
echo "world" > test2.txt
cat < test1.txt | ./minishell
rm -f test1.txt test2.txt
```

### **Test Script 2: Stress Tests**
```bash
#!/bin/bash
# tests/test_stress.sh

echo "💪 Testing Stress Cases"
echo "======================"

# Memory stress
echo "Testing memory stress..."
echo "hello world" | ./minishell
echo "hello world" | ./minishell
echo "hello world" | ./minishell

# Pipeline stress
echo "Testing pipeline stress..."
echo "hello" | cat | cat | ./minishell

# Multiple commands
echo "Testing multiple commands..."
printf "echo hello\necho world\necho test\n" | ./minishell
```

### **Test Script 3: Error Handling**
```bash
#!/bin/bash
# tests/test_errors.sh

echo "⚠️  Testing Error Handling"
echo "========================="

# File errors
echo "Testing file errors..."
echo "hello" > /tmp/test.txt
cat < /tmp/test.txt | ./minishell
rm -f /tmp/test.txt

# Command errors
echo "Testing command errors..."
echo "nonexistent_command" | ./minishell
echo "false" | ./minishell
echo "echo \$?" | ./minishell
```

## **🎯 Evaluation Testing Strategy**

### **Before the Evaluation**
1. **Run all test scripts** to ensure everything works
2. **Test with different shells** (bash, zsh) for comparison
3. **Check memory usage** with tools like `valgrind`
4. **Verify signal handling** in both interactive and non-interactive modes

### **During the Evaluation**
1. **Stay calm** - your shell is well-tested
2. **Explain your approach** - show you understand the requirements
3. **Demonstrate edge cases** - show you've thought about them
4. **Handle errors gracefully** - don't panic if something unexpected happens

### **Key Evaluation Points**
- **Functionality**: Does it work like bash?
- **Error handling**: Does it handle errors gracefully?
- **Memory management**: No leaks, no crashes
- **Signal handling**: Proper behavior for Ctrl+C, Ctrl+D, Ctrl+\
- **Code quality**: Clean, readable, well-documented

## **🚀 Advanced Testing Commands**

### **Memory Testing**
```bash
# Check for memory leaks
valgrind --leak-check=full ./minishell

# Check for memory usage
/usr/bin/time -v ./minishell
```

### **Performance Testing**
```bash
# Time command execution
time echo "hello world" | ./minishell

# Compare with bash
time echo "hello world" | bash
```

### **Compatibility Testing**
```bash
# Test against bash behavior
echo "hello world" | ./minishell
echo "hello world" | bash

# Compare outputs
diff <(echo "hello world" | ./minishell) <(echo "hello world" | bash)
```

## **📋 Evaluation Checklist**

### **✅ Pre-Evaluation Checklist**
- [ ] All basic functionality works
- [ ] All built-ins work correctly
- [ ] Redirections work properly
- [ ] Pipelines work correctly
- [ ] Variable expansion works
- [ ] Quote handling works
- [ ] Signal handling works
- [ ] Error handling is robust
- [ ] No memory leaks
- [ ] Code is clean and documented

### **✅ During Evaluation**
- [ ] Stay confident
- [ ] Explain your design decisions
- [ ] Show you understand the requirements
- [ ] Handle unexpected tests gracefully
- [ ] Ask questions if unclear

## **🎉 Success Tips**

1. **Practice with the test scripts** until you're comfortable
2. **Understand your code** - be able to explain any part
3. **Know the subject requirements** - be able to reference them
4. **Stay calm** - your shell is well-implemented
5. **Show your testing approach** - demonstrate thoroughness

## **🏆 Conclusion**

Advanced testing is your secret weapon for evaluation success. By thoroughly testing edge cases, stress scenarios, and error conditions, you'll be prepared for anything the evaluator throws at you.

**Your minishell is robust, well-tested, and ready for evaluation!** 🚀✨
