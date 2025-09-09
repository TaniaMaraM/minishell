# Signal Handling Complete Guide - Minishell

## Table of Contents
1. [Signal Concepts](#signal-concepts)
2. [Complete Flow for Each Signal](#complete-flow-for-each-signal)
3. [Main Function Integration](#main-function-integration)
4. [Learning Concepts](#learning-concepts)
5. [Code Architecture](#code-architecture)
6. [Debugging and Testing](#debugging-and-testing)

---

## Signal Concepts

### What are Signals?
Signals are **software interrupts** sent to a process by the operating system or other processes. They're like "emergency notifications" that can interrupt normal program execution.

### Key Signal Types in Minishell:

| Signal | Trigger | Purpose | Behavior |
|--------|---------|---------|----------|
| **SIGINT** | `Ctrl+C` | Interrupt current operation | Cancel input, show new prompt |
| **SIGQUIT** | `Ctrl+\` | Quit signal | Ignored in interactive mode |
| **EOF** | `Ctrl+D` | End of file | Exit shell gracefully |

### Subject Requirements:
- **One global variable only**: `volatile sig_atomic_t g_signal`
- **Bash-like behavior**: Ctrl+C shows new prompt, Ctrl+D exits
- **Signal-safe operations**: Only use signal-safe functions in handlers

---

## Complete Flow for Each Signal

### 1. SIGINT (Ctrl+C) - Complete Flow

```
User presses Ctrl+C
    ↓
Kernel sends SIGINT to minishell process
    ↓
signal_handler(int sig) is called
    ↓
┌─────────────────────────────────────┐
│ void signal_handler(int sig) {      │
│     g_signal = SIGINT;              │ ← Set global flag
│     write(STDOUT_FILENO, "\n", 1);  │ ← Print newline
│     rl_on_new_line();               │ ← Move cursor to new line
│     rl_replace_line("", 0);         │ ← Clear readline buffer
│     rl_redisplay();                 │ ← Show fresh prompt
│ }                                   │
└─────────────────────────────────────┘
    ↓
Main loop continues execution
    ↓
handle_signal_interrupt() checks g_signal
    ↓
┌─────────────────────────────────────┐
│ if (g_signal == SIGINT) {           │
│     sh->last_status = 130;          │ ← Set exit status (bash standard)
│     g_signal = 0;                   │ ← Reset flag
│     free(line);                     │ ← Clean up memory
│     return (1);                     │ ← Continue loop
│ }                                   │
└─────────────────────────────────────┘
    ↓
Loop continues with fresh prompt
```

### 2. SIGQUIT (Ctrl+\) - Complete Flow

```
User presses Ctrl+\
    ↓
Kernel sends SIGQUIT to minishell process
    ↓
Signal handler is SIG_IGN (ignored)
    ↓
Nothing happens (subject requirement: "does nothing")
```

### 3. EOF (Ctrl+D) - Complete Flow

```
User presses Ctrl+D
    ↓
readline() receives EOF
    ↓
readline() returns NULL
    ↓
Main loop detects NULL return
    ↓
┌─────────────────────────────────────┐
│ if (!line) {                        │
│     if (sh->is_interactive)         │
│         write(STDOUT_FILENO,        │
│               "exit\n", 5);         │ ← Print "exit" in interactive
│     sh->should_exit = 1;            │ ← Set exit flag
│     break;                          │ ← Exit loop
│ }                                   │
└─────────────────────────────────────┘
    ↓
Shell exits gracefully
```

---

## Main Function Integration

### How Signals are Called by Main:

```c
int main(int argc, char **argv, char **envp)
{
    t_shell shell;
    
    // 1. Initialize shell structure
    shell_init(&shell, envp);
    //    ↓
    //    Calls signal_setup_interactive() or signal_setup_non_interactive()
    //    ↓
    //    Sets up signal handlers with sigaction()
    
    // 2. Start main loop
    shell_loop(&shell);
    //    ↓
    //    Calls readline() in loop
    //    ↓
    //    Checks g_signal after each readline() call
    //    ↓
    //    Handles signals appropriately
    
    // 3. Cleanup
    shell_cleanup(&shell);
    //    ↓
    //    Calls signal_restore_defaults()
    //    ↓
    //    Restores original signal handlers
    
    return (shell.last_status);
}
```

### Signal Setup Flow:

```c
shell_init() → signal_setup_interactive() → sigaction() → signal_handler()
```

### Signal Handling Flow:

```c
readline() → signal_handler() → g_signal = SIGINT → handle_signal_interrupt() → continue
```

---

## Learning Concepts

### 1. Signal Safety
**Why `volatile sig_atomic_t`?**
- `volatile`: Prevents compiler optimization (signals can change it anytime)
- `sig_atomic_t`: Only type guaranteed to be atomic in signal handlers
- **Rule**: Only use signal-safe functions in signal handlers

### 2. Global Variable Pattern
**Why one global variable?**
- Subject requirement: "at most one global variable"
- Communication bridge between signal handler and main loop
- Signal handlers can't pass parameters to main code

### 3. Readline Integration
**Why these specific functions?**
- `rl_on_new_line()`: Tells readline we're on a new line
- `rl_replace_line("", 0)`: Clears readline's internal buffer
- `rl_redisplay()`: Redraws the prompt
- **Order matters**: Must be called in this sequence

### 4. Exit Status Codes
**Why 130 for SIGINT?**
- Bash standard: SIGINT = 128 + signal number (2) = 130
- Allows scripts to detect if command was interrupted
- Maintains compatibility with bash behavior

### 5. Interactive vs Non-Interactive
**Why different behavior?**
- **Interactive**: User expects "exit" message, Ctrl+C continues
- **Non-Interactive**: Scripts expect clean output, Ctrl+C exits
- **Detection**: `isatty(STDIN_FILENO)` checks if connected to terminal

---

## Code Architecture

### File Structure:
```
src/signals/
├── signals.c          # Signal handlers and setup
└── signals.h          # Function declarations

src/app/
├── init.c             # Calls signal_setup_interactive()
├── loop.c             # Checks g_signal in main loop
└── cleanup.c          # Calls signal_restore_defaults()
```

### Key Functions:

#### Signal Setup:
```c
void signal_setup_interactive(void)    // Sets up SIGINT/SIGQUIT handlers
void signal_setup_non_interactive(void) // Sets up default handlers
void signal_restore_defaults(void)     // Restores original handlers
```

#### Signal Handling:
```c
void signal_handler(int sig)           // Main signal handler
static int handle_signal_interrupt()   // Main loop signal processing
```

#### Global Variable:
```c
volatile sig_atomic_t g_signal = 0;    // Single global variable (subject requirement)
```

---

## Debugging and Testing

### Testing Signal Behavior:

#### 1. Test Ctrl+C:
```bash
./minishell
minishell$ echo "test"  # Type this, then press Ctrl+C
# Expected: New prompt appears immediately
```

#### 2. Test Ctrl+D:
```bash
./minishell
minishell$ # Press Ctrl+D
# Expected: "exit" message and shell exits
```

#### 3. Test Ctrl+\:
```bash
./minishell
minishell$ echo "test"  # Type this, then press Ctrl+\
# Expected: Nothing happens (ignored)
```

### Debugging Tips:

#### 1. Add Debug Prints:
```c
void signal_handler(int sig) {
    printf("DEBUG: Signal %d received\n", sig);  // Add this temporarily
    g_signal = sig;
    // ... rest of handler
}
```

#### 2. Check Signal Setup:
```c
void signal_setup_interactive(void) {
    printf("DEBUG: Setting up interactive signals\n");  // Add this
    // ... rest of setup
}
```

#### 3. Monitor Global Variable:
```c
static int handle_signal_interrupt(t_shell *sh, char *line) {
    printf("DEBUG: g_signal = %d\n", g_signal);  // Add this
    if (g_signal == SIGINT) {
        // ... rest of function
    }
}
```

### Common Issues:

#### 1. Signal Not Working:
- Check if `sigaction()` is called correctly
- Verify signal handler is not being overridden
- Ensure `g_signal` is declared as `extern` in header

#### 2. Double Prompts:
- Check if `rl_redisplay()` is called multiple times
- Verify `write("\n")` is not called twice
- Ensure signal handler is not called recursively

#### 3. Buffer Not Clearing:
- Verify `rl_replace_line("", 0)` is called
- Check if readline functions are available
- Ensure proper order of readline function calls

---

## Summary

### Key Takeaways:

1. **Signals are interrupts** that can change program flow
2. **One global variable** is the communication bridge
3. **Signal safety** is crucial - only use safe functions in handlers
4. **Readline integration** requires specific function sequence
5. **Bash compatibility** means following standard exit codes
6. **Interactive vs non-interactive** behavior differs significantly

### Best Practices:

1. **Keep signal handlers simple** - just set flags
2. **Handle logic in main loop** - not in signal handlers
3. **Always check return values** of signal functions
4. **Test on both macOS and Linux** - behavior can differ
5. **Use proper exit codes** - maintain bash compatibility
6. **Clean up resources** - restore signal handlers on exit

---

*This guide provides a complete understanding of signal handling in minishell, from basic concepts to implementation details and debugging techniques.*
