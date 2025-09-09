# Phase 4 Analysis: Expander - Variable Expansion System

## Table of Contents
1. [What is Variable Expansion?](#what-is-variable-expansion)
2. [The Analogy: Mail Merge System](#the-analogy-mail-merge-system)
3. [Architecture Overview](#architecture-overview)
4. [Key Components](#key-components)
5. [Quote Handling Rules](#quote-handling-rules)
6. [Variable Resolution](#variable-resolution)
7. [Integration with Parser](#integration-with-parser)
8. [Memory Management](#memory-management)
9. [Error Handling](#error-handling)
10. [Testing Strategy](#testing-strategy)
11. [Key Learnings](#key-learnings)

---

## What is Variable Expansion?

**Variable expansion** is the process of replacing variable references (like `$USER` or `$?`) with their actual values from the environment or shell state.

### The Analogy: Mail Merge System
Think of variable expansion like a **mail merge system**:

- **Template**: `"Hello $USER, your status is $?"`
- **Variables**: `USER=john`, `?=0` (exit status)
- **Result**: `"Hello john, your status is 0"`

**In Our Shell:**
- **Input**: `echo "Welcome $USER, last command status: $?"`
- **Expansion**: `echo "Welcome john, last command status: 0"`
- **Output**: `Welcome john, last command status: 0`

---

## Architecture Overview

### The Expansion Pipeline
```
User Input → Lexer → Parser → Expander → Executor
                ↓
            Tokens → Commands → Expanded Commands → Execution
```

### Key Design Principles
1. **Quote-aware expansion**: Different rules for different quote types
2. **Context preservation**: Maintain original structure while expanding
3. **Error resilience**: Handle undefined variables gracefully
4. **Memory safety**: Proper allocation and cleanup

---

## Key Components

### 1. Expander Structure (`t_expander`)
```c
typedef struct s_expander {
    char            *input;         // Original input string
    char            *result;        // Expanded result string
    size_t          input_pos;      // Current position in input
    size_t          result_pos;     // Current position in result
    size_t          result_capacity; // Capacity of result buffer
    t_quote_state   quote_state;    // Current quote context
    t_shell         *shell;         // Shell state for variable access
} t_expander;
```

**What it does**: Tracks the expansion process, maintaining state and context.

### 2. Quote State Management
```c
typedef enum e_quote_state {
    QUOTE_NONE,     // No quotes active
    QUOTE_SINGLE,   // Inside single quotes
    QUOTE_DOUBLE    // Inside double quotes
} t_quote_state;
```

**What it does**: Tracks which type of quotes are currently active.

---

## Quote Handling Rules

### The Analogy: Different Types of Protection
Think of quotes like different types of **protective barriers**:

- **Single quotes (`'`)**: **Bulletproof glass** - nothing gets through
- **Double quotes (`"`)**: **Screen door** - allows some things through
- **No quotes**: **Open window** - everything gets through

### Expansion Rules

| Context | `$VAR` Expansion | `$?` Expansion | Example |
|---------|------------------|----------------|---------|
| **No quotes** | ✅ Yes | ✅ Yes | `echo $USER` → `echo john` |
| **Single quotes** | ❌ No | ❌ No | `echo '$USER'` → `echo $USER` |
| **Double quotes** | ✅ Yes | ✅ Yes | `echo "$USER"` → `echo john` |

### Real Examples
```bash
# No quotes - full expansion
echo $USER$HOME          # → echo john/home/user

# Single quotes - no expansion
echo '$USER$HOME'        # → echo $USER$HOME

# Double quotes - expansion allowed
echo "$USER $HOME"       # → echo john /home/user

# Mixed quotes - selective expansion
echo 'Hello' $USER "world"  # → echo Hello john world
```

---

## Variable Resolution

### 1. Environment Variables (`$VAR`)
```c
char *get_env_var(const char *name, t_shell *shell)
{
    // Search through shell->envp array
    // Look for "NAME=value" format
    // Return the value part
}
```

**What it does**: Looks up environment variables in the shell's environment array.

### 2. Exit Status (`$?`)
```c
char *resolve_exit_status(t_shell *shell)
{
    // Convert shell->last_status to string
    // Return "0", "1", "127", etc.
}
```

**What it does**: Converts the last command's exit status to a string.

### 3. Variable Name Parsing
```c
char *parse_variable_name(const char *input, size_t *pos)
{
    // Extract variable name after $
    // Handle valid characters: alphanumeric + underscore
    // Update position pointer
}
```

**What it does**: Extracts the variable name from the input string.

---

## Integration with Parser

### The Expansion Flow
```
1. Parser builds command structures
2. Expander processes each command
3. Variables are expanded in place
4. Commands are ready for execution
```

### Integration Points
```c
// In parser_integration.c
static int parse_execute(t_parser *parser)
{
    t_cmd *cmd_list = parser_parse(parser);
    if (parser->error || !cmd_list)
        return (1);
    
    // NEW: Expand variables in commands
    if (expand_command_list(cmd_list, parser->shell))
    {
        print_error("expander", "Variable expansion failed");
        return (1);
    }
    
    print_cmd_list(cmd_list);
    return (0);
}
```

**What it does**: Integrates expansion into the parsing pipeline.

---

## Memory Management

### The Analogy: Construction Site Cleanup
Think of memory management like a **construction site**:

- **Allocation**: Ordering materials (malloc)
- **Usage**: Building with materials (expansion)
- **Cleanup**: Cleaning up after work (free)

### Allocation Strategy
```c
// 1. Allocate expander structure
t_expander *expander = expander_init(input, shell);

// 2. Allocate result buffer (2x input size for safety)
expander->result_capacity = ft_strlen(input) * 2;
expander->result = malloc(expander->result_capacity + 1);

// 3. Expand and get result
char *result = ft_strdup(expander->result);

// 4. Clean up
expander_destroy(expander);
```

### Memory Safety
- **Every malloc has a corresponding free**
- **Error checking on every allocation**
- **Graceful failure handling**
- **No memory leaks**

---

## Error Handling

### The Analogy: Safety Net
Think of error handling like a **safety net**:

- **Undefined variables**: Return empty string (graceful degradation)
- **Memory allocation failure**: Clean up and return error
- **Invalid syntax**: Report error and continue

### Error Scenarios
```c
// Undefined variable
echo $UNDEFINED_VAR    // → echo (empty string)

// Memory allocation failure
// → Clean up and return error

// Invalid variable name
echo $123INVALID       // → echo (empty string)
```

### Error Recovery
- **Continue processing** when possible
- **Clean up resources** on failure
- **Report errors clearly** to user
- **Maintain shell stability**

---

## Testing Strategy

### Test Categories

#### 1. Basic Variable Expansion
```bash
# Test basic environment variables
echo $USER
echo $HOME
echo $PATH
```

#### 2. Exit Status Expansion
```bash
# Test exit status
echo $?
false
echo $?
```

#### 3. Quote Handling
```bash
# Test single quotes (no expansion)
echo '$USER'

# Test double quotes (allow expansion)
echo "$USER"

# Test mixed quotes
echo 'Hello' $USER "world"
```

#### 4. Edge Cases
```bash
# Test undefined variables
echo $UNDEFINED

# Test variable at boundaries
echo $USERhello
echo hello$USER

# Test multiple variables
echo $USER$HOME
```

#### 5. Integration Tests
```bash
# Test with pipes
echo $USER | grep $USER

# Test with redirections
echo $USER > $HOME/test

# Test complex commands
ls $HOME | grep $USER
```

---

## Key Learnings

### 1. Quote Context Awareness
**What it is**: Different quote types have different expansion rules.

**The Analogy**: Different types of protective barriers allow different things through.

**Why it matters**: This is fundamental to shell behavior - users expect quotes to work like bash.

### 2. State Machine Design
**What it is**: The expander uses a state machine to track quote context.

**The Analogy**: Like a traffic light system that changes based on context.

**Why it matters**: Ensures correct expansion behavior in complex quote scenarios.

### 3. Memory Management
**What it is**: Careful allocation and cleanup of expansion buffers.

**The Analogy**: Like a construction site that must be cleaned up after work.

**Why it matters**: Prevents memory leaks and ensures system stability.

### 4. Integration Design
**What it is**: The expander integrates seamlessly with the parser.

**The Analogy**: Like a production line where each station does its job.

**Why it matters**: Maintains clean separation of concerns while enabling functionality.

### 5. Error Resilience
**What it is**: The system continues working even when variables are undefined.

**The Analogy**: Like a car that keeps running even if one component fails.

**Why it matters**: Provides a robust user experience.

---

## Bash Behavior Concepts

### 1. Variable Expansion
**What it is**: Replacing `$VAR` with its value.

**The Analogy**: Like a find-and-replace operation in a text editor.

**Why it matters**: Essential for dynamic command construction.

### 2. Quote Removal
**What it is**: Removing quotes after expansion (but not during).

**The Analogy**: Like removing protective packaging after delivery.

**Why it matters**: Ensures correct argument passing to commands.

### 3. Word Splitting
**What it is**: Breaking expanded strings into separate arguments.

**The Analogy**: Like cutting a string into individual pieces.

**Why it matters**: Enables complex command construction.

### 4. Pathname Expansion
**What it is**: Expanding wildcards like `*` and `?` (not implemented in Phase 4).

**The Analogy**: Like a search that finds all matching files.

**Why it matters**: Enables file operations with patterns.

---

## Integration with Shell Loop

### The Complete Flow
```
1. User types: echo "Hello $USER"
2. REPL loop reads input
3. Lexer tokenizes: [echo, "Hello $USER"]
4. Parser builds command structure
5. Expander processes: "Hello $USER" → "Hello john"
6. Command ready for execution (Phase 5)
```

### Data Flow
```
Input String → Tokens → Command Structure → Expanded Commands → Execution
```

---

## Conclusion

The Phase 4 expander provides a robust variable expansion system that:

- **Respects quote rules** like bash
- **Handles edge cases** gracefully
- **Integrates seamlessly** with the parser
- **Manages memory safely**
- **Provides clear error handling**

This foundation enables dynamic command construction and sets the stage for the executor in Phase 5.

---

*This analysis provides a comprehensive understanding of the variable expansion system, from high-level concepts to low-level implementation details.*
