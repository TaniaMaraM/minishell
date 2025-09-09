# Phase 1: REPL + Signal Handling - Architecture Analysis

## Table of Contents
1. [Phase 1 Overview](#phase-1-overview)
2. [Signal Handling Architecture](#signal-handling-architecture)
3. [REPL Loop Implementation](#repl-loop-implementation)
4. [EOF Handling](#eof-handling)
5. [Interactive vs Non-Interactive Modes](#interactive-vs-non-interactive-modes)
6. [Global Variable Management](#global-variable-management)
7. [Memory Management](#memory-management)
8. [Key Learning Points](#key-learning-points)
9. [Code Examples](#code-examples)
10. [Testing Results](#testing-results)

---

## Phase 1 Overview

### What We Implemented
- **Proper signal handling** with global variable approach
- **Interactive vs non-interactive** signal behavior
- **EOF detection and handling** (Ctrl+D)
- **Signal-safe operations** in main loop
- **Proper `extern` declarations** for global variables

### What We Fixed
- **Signal handler integration** with main loop
- **Readline interruption handling**
- **Proper cleanup on exit**
- **Subject compliance** for signal handling

---

## Signal Handling Architecture

### Global Variable Approach (Subject Requirement)

```c
// In main.c (DEFINITION - allocates memory)
volatile sig_atomic_t g_signal = 0;

// In minishell.h (DECLARATION - tells other files it exists)
extern volatile sig_atomic_t g_signal;
```

### Why This Design?

**Subject Compliance**: Uses exactly one global variable as required
**Signal Safety**: `sig_atomic_t` is the only type safe in signal handlers
**Separation of Concerns**: Handler sets flag, main loop handles logic

### Signal Handler Implementation

```c
void signal_handler(int sig)
{
    /* Store signal number in global variable - subject requirement */
    g_signal = sig;
    
    /* For SIGINT in interactive mode, just set the flag */
    /* The main loop will handle the newline and prompt */
}
```

**Key Points:**
- **Minimal logic** in signal handler (signal-safe)
- **Just stores signal number** in global variable
- **Main loop handles** the actual response

### Signal Setup Functions

```c
void signal_setup_interactive(void)
{
    struct sigaction sa;
    
    /* Setup SIGINT (Ctrl+C) handler */
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    
    /* Setup SIGQUIT (Ctrl+\) handler - do nothing in interactive mode */
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}
```

**Signal Flags Explained:**
- **`SA_RESTART`**: Automatically restarts interrupted system calls
- **`SIG_IGN`**: Ignore SIGQUIT in interactive mode (subject requirement)
- **`sigemptyset`**: Clear the signal mask

---

## REPL Loop Implementation

### Main Loop Architecture

```c
int shell_loop(t_shell *shell)
{
    char *line;
    int status;

    if (!shell) return (1);
    
    while (1)
    {
        g_signal = 0;  // Reset signal flag
        
        // Read input line
        line = readline(shell->prompt);
        
        // Check if we received a signal during readline
        if (g_signal == SIGINT)
        {
            // Signal handler already handled the display
            // In non-interactive mode, exit on SIGINT
            if (!shell->is_interactive)
                break;
            // In interactive mode, continue to next iteration
            continue;
        }
        
        if (!line)
        {
            // EOF (Ctrl+D) - exit shell
            if (shell->is_interactive)
                ft_putendl_fd("exit", STDOUT_FILENO);
            break;
        }
        
        // Skip empty lines
        if (ft_strlen(line) == 0)
        {
            free(line);
            continue;
        }
        
        // Add to history if not empty
        add_history(line);
        
        // TODO: Parse and execute command
        status = 0;  // Placeholder
        
        // Update last status
        shell->last_status = status;
        
        // Free line
        free(line);
    }
    
    return (shell->last_status);
}
```

### Loop Flow Analysis

1. **Reset Signal Flag**: Clear `g_signal` at start of each iteration
2. **Read Input**: Use `readline()` to get user input
3. **Check Signal**: If SIGINT was received, handle appropriately
4. **Check EOF**: If `readline()` returned NULL, handle EOF
5. **Process Input**: Skip empty lines, add to history
6. **Execute Command**: TODO - placeholder for future phases
7. **Update Status**: Store command exit status for `$?` expansion
8. **Cleanup**: Free allocated memory

---

## EOF Handling

### What is EOF?

**EOF (End of File)** occurs when:
- **No more data** is available to read
- **Input stream is closed**
- **User presses Ctrl+D** (in interactive terminals)

### EOF Detection

```c
line = readline(shell->prompt);
if (!line)  // This means EOF
{
    /* EOF (Ctrl+D) - exit shell */
    if (shell->is_interactive)
        ft_putendl_fd("exit", STDOUT_FILENO);
    break;
}
```

### EOF Behavior

**Interactive Mode**:
- Print "exit" message (bash compatibility)
- Exit the shell cleanly

**Non-Interactive Mode**:
- Exit silently (no "exit" message)
- Return to calling process

### EOF vs Other Conditions

| Condition | Trigger | Behavior |
|-----------|---------|----------|
| **EOF (Ctrl+D)** | Input stream closes | Exit shell |
| **SIGINT (Ctrl+C)** | Interrupt signal | Continue with new prompt |
| **Empty line** | User presses Enter | Show prompt again |
| **Valid command** | User types command | Execute (TODO) |

---

## Interactive vs Non-Interactive Modes

### Detection

```c
shell->is_interactive = isatty(STDIN_FILENO);
```

**`isatty()`** checks if the file descriptor is connected to a terminal:
- **Returns 1**: Connected to terminal (interactive)
- **Returns 0**: Not connected to terminal (non-interactive)

### Signal Handling Differences

**Interactive Mode**:
```c
if (shell->is_interactive)
{
    // Ctrl+C: Continue with new prompt
    // Ctrl+D: Print "exit" and exit
    // Ctrl+\: Ignored
}
```

**Non-Interactive Mode**:
```c
else
{
    // Ctrl+C: Exit immediately
    // Ctrl+D: Exit silently
    // Ctrl+\: Exit immediately
}
```

### Use Cases

**Interactive Mode**:
- User running shell in terminal
- Expects to continue working after interruption
- Needs visual feedback

**Non-Interactive Mode**:
- Scripts running shell commands
- Should exit cleanly on interruption
- No visual feedback needed

---

## Global Variable Management

### The `extern` Pattern

```c
// In main.c (DEFINITION - allocates memory and initializes)
volatile sig_atomic_t g_signal = 0;

// In minishell.h (DECLARATION - tells other files it exists)
extern volatile sig_atomic_t g_signal;
```

### Why `extern` is Required

**Without `extern`**:
- Every file including the header would try to define the variable
- **Result**: Multiple definition error (or dangerous merging)

**With `extern`**:
- Only one file defines the variable
- Other files just declare it exists
- **Result**: Clean, safe linking

### The One Definition Rule

**C Standard Rule**: A variable can be defined only once, but declared multiple times.

**Our Implementation**:
- **One definition**: `main.c` with initialization
- **Multiple declarations**: Headers with `extern`

---

## Memory Management

### Allocation Ownership

```c
// shell_init() owns shell->envp allocation
shell->envp = ft_strarr_dup(envp);

// shell_loop() owns line allocation
line = readline(shell->prompt);

// shell_cleanup() frees all owned allocations
if (shell->envp)
    ft_strarr_free(shell->envp);
```

### Memory Management Rules

1. **Every allocation has one owner**
2. **Owner must free the allocation**
3. **No double-freeing**
4. **Check for NULL before freeing**
5. **Free in reverse order of allocation**

### Cleanup Strategy

```c
void shell_cleanup(t_shell *shell)
{
    if (!shell) return;
    
    // Restore default signal handlers
    signal_restore_defaults();
    
    // Free environment array
    if (shell->envp)
        ft_strarr_free(shell->envp);
    
    // Clear readline history
    clear_history();
}
```

---

## Key Learning Points

### 1. Signal Handling Philosophy

**The Analogy: Fire Alarm System**
- **Global Variable**: Fire alarm indicator light
- **Signal Handler**: Fire alarm (just rings and sets indicator)
- **Main Loop**: Building manager (sees indicator and decides what to do)

**Why This Design?**
- Signal handlers must be fast and simple
- Complex logic in signal handlers is dangerous
- Main loop can safely handle complex decisions

### 2. EOF Understanding

**The Analogy: Reading a Book**
- **Reading normally**: Getting words from the book
- **EOF**: Reached the last page, no more words
- **Ctrl+D**: Closing the book and saying "I'm done"

### 3. Interactive vs Non-Interactive

**The Analogy: Restaurant vs Takeout**
- **Interactive**: Dining in restaurant (customer present, can handle interruptions)
- **Non-Interactive**: Takeout (no customer, just process order)

### 4. Global Variable Management

**The Analogy: Library Book System**
- **Definition**: The library (owns the book)
- **Declaration**: The library catalog (tells you the book exists)
- **`extern`**: Access through the catalog

---

## Code Examples

### Signal Handler Integration

```c
// Signal handler (signal-safe)
void signal_handler(int sig)
{
    g_signal = sig;  // Just store the signal number
}

// Main loop (can do complex operations)
if (g_signal == SIGINT)
{
    if (!shell->is_interactive)
        break;  // Exit in non-interactive mode
    continue;   // Continue in interactive mode
}
```

### EOF Handling

```c
line = readline(shell->prompt);
if (!line)  // EOF detected
{
    if (shell->is_interactive)
        ft_putendl_fd("exit", STDOUT_FILENO);
    break;
}
```

### Interactive Detection

```c
shell->is_interactive = isatty(STDIN_FILENO);
if (shell->is_interactive)
    signal_setup_interactive();
else
    signal_setup_non_interactive();
```

---

## Testing Results

### What Works

✅ **Signal Handling**: Ctrl+C properly handled in both modes  
✅ **EOF Handling**: Ctrl+D exits cleanly with proper message  
✅ **Interactive Mode**: Continues after interruption  
✅ **Non-Interactive Mode**: Exits on interruption  
✅ **Memory Management**: No leaks detected  
✅ **Build System**: Clean compilation with strict flags  

### Test Commands

```bash
# Basic functionality
echo "exit" | ./minishell

# Signal handling (interactive)
./minishell
# Press Ctrl+C - should show new prompt
# Press Ctrl+D - should exit with "exit" message

# Non-interactive mode
echo "exit" | ./minishell
# Should exit silently
```

---

## Next Steps (Phase 2)

### What's Missing

1. **Command Parsing**: Currently just reads input, doesn't process it
2. **Tokenization**: Need to break input into tokens
3. **Quote Handling**: Single and double quotes
4. **Metacharacter Recognition**: `|`, `<`, `>`, `>>`, `<<`

### Phase 2 Goals

1. **Implement Lexer**: Tokenize input respecting quotes
2. **Handle Metacharacters**: Recognize shell operators
3. **Quote Processing**: Handle single/double quotes correctly
4. **Error Handling**: Invalid syntax detection

---

*This document provides a comprehensive analysis of Phase 1 implementation. Keep it for reference during development and evaluation.*
