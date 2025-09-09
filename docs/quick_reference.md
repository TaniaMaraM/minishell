# Quick Reference: Quote Processing & Exit Behavior

## 🎯 Key Concepts

### Quote Processing Order
```
Input → Quote Processor → Lexer → Parser → Executor
```
**Why**: Quote context is lost during tokenization, so process quotes FIRST.

### Quote Rules
- **Single Quotes (`'`)**: No expansion, everything literal
- **Double Quotes (`"`)**: Variables expand, some escaping
- **No Quotes**: Full expansion and word splitting

### Exit Behavior
- **Interactive Mode**: Print "exit" on Ctrl+D
- **Non-Interactive Mode**: No "exit" printed (clean output)

---

## 🔧 Implementation Details

### Quote Processing Function
```c
char *process_quotes(char *str, t_shell *sh)
{
    // Track quote states
    int in_single = 0, in_double = 0;
    
    // Process character by character
    while (str[i]) {
        if (str[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (str[i] == '"' && !in_single)
            in_double = !in_double;
        else if (str[i] == '$' && !in_single)
            expand_variable();
        else
            copy_character();
    }
}
```

### Exit Behavior Fix
```c
if (!line) {
    if (sh->is_interactive)  // Only in interactive mode
        write(STDOUT_FILENO, "exit\n", 5);
    sh->should_exit = 1;
    break;
}
```

---

## 📋 Test Cases

### Quote Processing
```bash
# Single quotes - no expansion
echo 'hello $USER' → hello $USER

# Double quotes - expansion
echo "hello $USER" → hello taninha

# Nested quotes
echo "echo 'hello \$USER'" → echo 'hello $USER'
```

### Exit Behavior
```bash
# Non-interactive (piped) - no "exit"
echo "test" | ./minishell → test

# Interactive - shows "exit"
./minishell
# Ctrl+D → exit
```

---

## 🎓 Why This Approach?

### Pre-Tokenization Quote Processing
- ✅ **Correctness**: Matches bash behavior
- ✅ **Simplicity**: Clean separation of concerns
- ✅ **Performance**: Single pass, O(n) complexity
- ✅ **Maintainability**: Easy to understand and modify

### Mode-Aware Exit Behavior
- ✅ **User Experience**: Appropriate feedback for each mode
- ✅ **POSIX Compliance**: Standard shell behavior
- ✅ **Bash Compatibility**: Identical behavior to bash

---

## 🚨 Common Pitfalls

1. **Don't process quotes after tokenization** - context is lost
2. **Don't always print "exit"** - check interactive mode first
3. **Don't forget nested quotes** - handle quote state properly
4. **Don't ignore unclosed quotes** - check syntax first

---

## 📚 Files Modified

- `src/utils/quote_processing.c` - Quote processing functions
- `src/app/input_handler.c` - Integration with processing pipeline
- `src/app/loop.c` - Exit behavior fix
- `include/minishell.h` - Function prototypes

---

## 🎯 Success Criteria

- ✅ Quote handling matches bash exactly
- ✅ No unwanted "exit" in non-interactive mode
- ✅ Clean, maintainable code
- ✅ Proper error handling
- ✅ Memory management (no leaks)