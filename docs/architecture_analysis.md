# Minishell Architecture Analysis

## Table of Contents
1. [Header Organization & Dependencies](#header-organization--dependencies)
2. [Core Data Structure - t_shell](#core-data-structure---tshell)
3. [Signal Handling Architecture](#signal-handling-architecture)
4. [Initialization Flow](#initialization-flow)
5. [Main Loop Architecture](#main-loop-architecture)
6. [Error Handling Strategy](#error-handling-strategy)
7. [Memory Management Philosophy](#memory-management-philosophy)
8. [Build System Integration](#build-system-integration)
9. [What's Missing (Next Phases)](#whats-missing-next-phases)
10. [Architecture Strengths](#architecture-strengths)
11. [Key Learning Points with Analogies](#key-learning-points-with-analogies)

---

## Header Organization & Dependencies

### `include/minishell.h` - Main Header

```c
// System includes (all allowed by subject)
# include <stdio.h>      // Standard I/O
# include <readline/readline.h>  // REPL functionality
# include <signal.h>     // Signal handling
# include <sys/wait.h>   // Process management
# include <errno.h>      // Error handling
# include <string.h>     // String operations
# include <fcntl.h>      // File operations
# include <sys/stat.h>   // File status
# include <dirent.h>     // Directory operations
# include <termios.h>    // Terminal control
# include <curses.h>     // Cursor control
# include <term.h>       // Terminal capabilities

// Custom libraries
# include "../lib/libft/libft.h"
# include "../lib/get_next_line/get_next_line.h"
```

**Key Design Decision**: We include all necessary system headers in the main header, following the subject's allowed functions list.

### `include/signals.h` - Signal Handling

```c
# include <signal.h>

// Global signal variable - subject requirement
extern volatile sig_atomic_t g_signal;

// Signal handling functions
void	signal_setup_interactive(void);
void	signal_setup_non_interactive(void);
void	signal_restore_defaults(void);
void	signal_handler(int sig);
```

**Separation of Concerns**: Signal handling is isolated in its own header for modularity.

---

## Core Data Structure - `t_shell`

```c
typedef struct s_shell
{
    char    **envp;         // Environment variables array
    int     last_status;    // Last command exit status for $?
    int     is_interactive; // Whether running in interactive mode
    char    *prompt;        // Current prompt string
} t_shell;
```

### Field Explanations

- **`envp`**: We need our own copy to modify environment variables
- **`last_status`**: Required for `$?` expansion (subject requirement)
- **`is_interactive`**: Different signal handling for interactive vs non-interactive
- **`prompt`**: Configurable prompt (could be extended later)

### Design Rationale

This structure encapsulates all the state needed for a shell session, following the principle of keeping related data together.

---

## Signal Handling Architecture

### Global Variable Approach (Subject Requirement)

```c
volatile sig_atomic_t g_signal = 0;  // Single global variable
```

**Why `volatile`?**
- Tells compiler the variable can change outside normal program flow
- Prevents compiler optimizations that might cache the value

**Why `sig_atomic_t`?**
- Only type guaranteed to be atomic in signal handlers
- Can be safely read/written from signal handlers

### Signal Handler Strategy

```c
void signal_handler(int sig)
{
    g_signal = sig;  // Just store the signal number
    // Main loop handles the actual response
}
```

**Why This Approach?**
- **Subject Compliance**: Uses exactly one global variable as required
- **Signal Safety**: `sig_atomic_t` is the only type safe to use in signal handlers
- **Separation of Concerns**: Handler just sets flag, main loop handles logic

### Signal Setup Functions

```c
void signal_setup_interactive(void)
{
    struct sigaction sa;
    
    // Setup SIGINT (Ctrl+C) handler
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    
    // Setup SIGQUIT (Ctrl+\) handler - do nothing in interactive mode
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}
```

**Key Points:**
- **`SA_RESTART`**: Automatically restarts interrupted system calls
- **`SIG_IGN`**: Ignore SIGQUIT in interactive mode (subject requirement)
- **`sigemptyset`**: Clear the signal mask

---

## Initialization Flow

```c
int shell_init(t_shell *shell, char **envp)
{
    // 1. Validate inputs
    if (!shell || !envp) return (1);
    
    // 2. Initialize shell state
    shell->last_status = 0;
    shell->is_interactive = isatty(STDIN_FILENO);
    shell->prompt = "minishell$ ";
    
    // 3. Copy environment (deep copy)
    shell->envp = ft_strarr_dup(envp);
    if (!shell->envp)
    {
        print_error("initialization", "Failed to copy environment");
        return (1);
    }
    
    // 4. Setup signal handling based on mode
    if (shell->is_interactive)
        signal_setup_interactive();
    else
        signal_setup_non_interactive();
    
    return (0);
}
```

### Key Learning Points

- **Environment Copying**: We make a deep copy because we'll modify it
- **Interactive Detection**: `isatty()` determines if we're in a terminal
- **Signal Setup**: Different strategies for interactive vs non-interactive
- **Error Handling**: Check for allocation failures and return appropriate codes

---

## Main Loop Architecture

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

### Design Patterns

- **Signal Reset**: Clear flag at start of each iteration
- **EOF Handling**: `readline()` returns NULL on EOF
- **History Management**: Only add non-empty lines
- **Memory Management**: Always free the line
- **Status Tracking**: Update last command status for `$?` expansion

---

## Error Handling Strategy

```c
void print_error(const char *context, const char *message)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    if (context)
    {
        ft_putstr_fd((char *)context, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
    }
    if (message)
        ft_putstr_fd((char *)message, STDERR_FILENO);
    ft_putchar_fd('\n', STDERR_FILENO);
}
```

### Consistent Error Format

All errors follow the pattern: `minishell: context: message`

Examples:
- `minishell: initialization: Failed to copy environment`
- `minishell: cd: No such file or directory`
- `minishell: export: Invalid identifier`

### Error Handling Principles

- **Always write to STDERR**: Don't pollute stdout
- **Consistent formatting**: Easy to parse and understand
- **Context information**: Help users understand what went wrong
- **Graceful degradation**: Continue operation when possible


====== PARAMOS AQUI =====

## Memory Management Philosophy

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

### Memory Management Rules

1. **Every allocation has one owner**
2. **Owner must free the allocation**
3. **No double-freeing**
4. **Check for NULL before freeing**
5. **Free in reverse order of allocation**

---

## Build System Integration

### Makefile Structure

```makefile
# Source subdirectories
SRC_SUBDIRS = app lexeme parser expand exec builtin signals utils
SRC_DIRS    = $(addprefix $(SRC_DIR)/,$(SRC_SUBDIRS))

# Custom libs
LIBFT_DIR = $(LIB_DIR)/libft
GNL_DIR   = $(LIB_DIR)/get_next_line

LIBFT = $(LIBFT_DIR)/libft.a
GNL   = $(GNL_DIR)/libgnl.a

# Include paths
INCLUDES = -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -I$(GNL_DIR)
```

### Key Features

- **Modular Compilation**: Each source directory compiles separately
- **Library Integration**: Links libft and get_next_line
- **Dependency Management**: Proper header dependencies
- **Clean Build**: Removes all generated files
- **No Unnecessary Relinking**: Only recompiles changed files

---

## What's Missing (Next Phases)

### Current Limitations

1. **No Command Parsing**: Just reads input, doesn't process it
2. **No Builtins**: No `echo`, `cd`, `pwd`, etc.
3. **No Redirections**: No `<`, `>`, `>>`, `<<`
4. **No Pipes**: No `|` support
5. **No Variable Expansion**: No `$VAR` or `$?` expansion
6. **No Quote Handling**: No single/double quote processing
7. **No PATH Resolution**: No command lookup in PATH

### Next Phase Priorities

1. **Phase 1**: Improve signal handling in main loop
2. **Phase 2**: Implement lexer for tokenization
3. **Phase 3**: Implement parser for command structure
4. **Phase 4**: Implement variable expansion
5. **Phase 5**: Implement command execution

---

## Architecture Strengths

✅ **Modular Design**: Clear separation of concerns  
✅ **Subject Compliance**: Follows all requirements  
✅ **Memory Safety**: Proper allocation/deallocation  
✅ **Signal Safety**: Correct signal handling  
✅ **Extensible**: Easy to add new features  
✅ **Clean Code**: Readable and maintainable  
✅ **Error Handling**: Consistent error reporting  
✅ **Build System**: Proper dependency management  

---

## Key Learning Points with Analogies

### 1. Signal Handling Philosophy

**The Analogy: Fire Alarm System**
Think of signals like a fire alarm in a building:
- **Global Variable (`g_signal`)**: Like a fire alarm indicator light that everyone can see
- **Signal Handler**: Like the fire alarm itself - it just rings and sets the indicator
- **Main Loop**: Like the building manager who sees the indicator and decides what to do

**Why This Design?**
- **Signal handlers are like emergency responders** - they must be fast and simple
- **Complex logic in signal handlers is dangerous** - like trying to evacuate a building while the alarm is still ringing
- **The main loop is like the command center** - it can safely handle complex decisions

### 2. Environment Variable Management

**The Analogy: Office Filing System**
Think of environment variables like office files:
- **Original `envp`**: Like the master filing cabinet (read-only)
- **Our Copy (`shell->envp`)**: Like your personal working copy where you can add/remove files
- **Deep Copy**: Like photocopying every document instead of just copying the folder labels

**Why We Copy?**
- **We need to modify the environment** (add/remove variables)
- **Original is read-only** - like you can't write on the master files
- **Isolation** - our changes don't affect other programs

### 3. Interactive vs Non-Interactive Mode

**The Analogy: Restaurant vs Takeout**
- **Interactive Mode**: Like dining in a restaurant
  - Customer (user) is present and can interact
  - Waiter (shell) can ask questions, handle interruptions
  - Ctrl+C is like saying "excuse me" - waiter stops and asks what you need
  
- **Non-Interactive Mode**: Like takeout
  - Customer (user) is not present
  - Kitchen (shell) just processes the order
  - Ctrl+C is like canceling the order - kitchen stops immediately

**Why Different Signal Handling?**
- **Interactive**: User expects to continue working after interruption
- **Non-Interactive**: Script should exit cleanly on interruption

### 4. Memory Management Philosophy

**The Analogy: Library Book System**
- **Allocation**: Like checking out a book (you're responsible for it)
- **Ownership**: Like having a library card - only you can return the book
- **Cleanup**: Like returning all books before leaving the library

**Key Rules:**
- **Every allocation has one owner** - like every book has one borrower
- **Owner must return it** - like you must return the book
- **No double-returns** - like you can't return the same book twice

### 5. REPL Loop Architecture

**The Analogy: Conversation with a Friend**
- **Read**: Like listening to what your friend says
- **Eval**: Like understanding and processing what they said
- **Print**: Like responding back
- **Loop**: Like continuing the conversation

**Signal Handling in Conversation:**
- **Signal comes in**: Like someone tapping your shoulder
- **You acknowledge**: Like saying "hold on, someone needs me"
- **You handle it**: Like dealing with the interruption
- **You continue**: Like returning to your conversation

---

## Questions for Understanding

1. **Do you understand the signal handling approach?** Why do we use a global variable instead of passing data to the handler?

2. **Why do we copy the environment variables?** What happens if we don't?

3. **What's the purpose of the `is_interactive` flag?** How does it affect signal handling?

4. **How does the memory management system prevent leaks?** What happens if we forget to free something?

5. **Why is the modular design important?** How does it help with debugging and maintenance?

---

*This document serves as a comprehensive reference for the minishell architecture. Keep it handy for future reference and questions.*
