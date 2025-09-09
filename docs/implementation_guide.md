# Minishell Implementation Guide: Under the Hood

## Table of Contents
1. [The Big Picture: How Minishell Works](#the-big-picture-how-minishell-works)
2. [Phase-by-Phase Breakdown](#phase-by-phase-breakdown)
3. [The Data Flow: How Information Moves](#the-data-flow-how-information-moves)
4. [Memory Management: The Cleanup Crew](#memory-management-the-cleanup-crew)
5. [Error Handling: The Safety Net](#error-handling-the-safety-net)
6. [Signal Handling: The Interrupt System](#signal-handling-the-interrupt-system)
7. [The Architecture: Why It Works](#the-architecture-why-it-works)
8. [Key Learnings Summary](#key-learnings-summary)
9. [What Happens When You Type a Command](#what-happens-when-you-type-a-command)

---

## The Big Picture: How Minishell Works

### The Analogy: A Restaurant Kitchen
Think of minishell like a restaurant kitchen:

1. **Customer (User)** → Orders food (types command)
2. **Waiter (REPL Loop)** → Takes order and delivers it
3. **Chef (Parser)** → Breaks down the order into ingredients
4. **Kitchen Staff (Executor)** → Prepares the food
5. **Server (Output)** → Delivers the result

**In Our Shell:**
1. **User** → Types `echo "hello world" | grep hello`
2. **REPL Loop** → Reads input and manages the flow
3. **Lexer** → Breaks input into tokens: `[echo, "hello world", |, grep, hello]`
4. **Parser** → Builds command structures
5. **Executor** → Runs the commands (Phase 5)
6. **Output** → Shows results

---

## Phase-by-Phase Breakdown

### Phase 0: The Foundation (Bootstrap)
**What it is**: The basic structure that makes everything else possible.

**The Analogy**: Building the restaurant building
- **Makefile**: The construction blueprint
- **Directory structure**: The floor plan
- **Headers**: The electrical/plumbing connections
- **Main function**: The front door

**Key Learning**: 
```c
// main.c - The entry point
int main(int argc, char **argv, char **envp)
{
    t_shell shell;           // Our restaurant
    shell_init(&shell, envp); // Set up the kitchen
    shell_loop(&shell);      // Start serving customers
    shell_cleanup(&shell);   // Close for the night
}
```

**Why this matters**: Without a solid foundation, everything else collapses.

### Phase 1: The REPL Loop (Read-Eval-Print Loop)
**What it is**: The main interaction loop that keeps the shell running.

**The Analogy**: The restaurant's service cycle
- **Read**: Waiter takes customer order
- **Eval**: Kitchen processes the order
- **Print**: Waiter delivers the food
- **Loop**: Repeat for next customer

**Key Learning**:
```c
// loop.c - The service cycle
int shell_loop(t_shell *shell)
{
    while (1)  // Keep serving customers
    {
        char *line = readline(shell->prompt);  // Take order
        if (!line) break;                      // Customer left
        if (ft_strlen(line) == 0) continue;    // Empty order
        add_history(line);                     // Remember order
        status = parse_and_process(line);      // Process order
        free(line);                            // Clean up
    }
}
```

**What's happening under the hood**:
1. **`readline()`**: Reads user input with history support
2. **Signal handling**: Manages Ctrl+C, Ctrl+D interruptions
3. **History management**: Remembers previous commands
4. **Error handling**: Gracefully handles problems

**Why this matters**: This is the heart of the shell - without it, there's no interaction.

### Phase 2: The Lexer (Tokenization)
**What it is**: Breaks down the user's command into meaningful pieces.

**The Analogy**: A chef breaking down a recipe
- **Input**: "Add 2 cups flour, mix well, then bake at 350°F"
- **Lexer's job**: Break into ingredients: ["Add", "2", "cups", "flour", ",", "mix", "well", ",", "then", "bake", "at", "350°F"]

**Key Learning**:
```c
// tokens.h - The ingredient types
typedef enum e_token_type {
    TOKEN_WORD,        // Regular ingredients: "flour", "mix"
    TOKEN_PIPE,        // Connectors: "|"
    TOKEN_REDIR_IN,    // Input tools: "<"
    TOKEN_REDIR_OUT,   // Output tools: ">"
    TOKEN_EOF          // End of recipe
} t_token_type;

// lexer.c - The chef's process
t_token *lexer_next_token(t_lexer *lexer)
{
    // Skip whitespace (ignore extra spaces)
    while (is_whitespace(lexer->input[lexer->pos]))
        lexer->pos++;
    
    // Identify the ingredient type
    if (lexer->input[lexer->pos] == '|')
        return create_token(TOKEN_PIPE, "|", 1);
    else if (is_quote(lexer->input[lexer->pos]))
        return lexer_read_quoted(lexer, lexer->input[lexer->pos]);
    else
        return lexer_read_word(lexer);
}
```

**What's happening under the hood**:
1. **State machine**: Tracks where we are in the input
2. **Quote handling**: Protects text inside quotes
3. **Metacharacter recognition**: Identifies special operators
4. **Error detection**: Catches invalid syntax

**Why this matters**: Without proper tokenization, the parser can't understand the command.

### Phase 3: The Parser (Command Structure Building)
**What it is**: Takes tokens and builds command structures that can be executed.

**The Analogy**: A chef organizing ingredients into cooking steps
- **Tokens**: Individual ingredients
- **Parser's job**: Organize into cooking steps
- **Command structure**: Complete recipe with instructions

**Key Learning**:
```c
// cmd.h - The recipe structure
typedef struct s_cmd {
    char    **argv;      // Ingredients list: ["echo", "hello", "world"]
    t_redir *redirs;     // Cooking tools: input/output redirections
    struct s_cmd *next;  // Next step in the recipe (pipeline)
} t_cmd;

// parser.c - The organization process
t_cmd *parser_parse_command(t_parser *parser)
{
    t_cmd *cmd = cmd_create();                    // Create new recipe
    parser_parse_arguments(parser, cmd);          // Add ingredients
    parser_parse_redirections(parser, cmd);       // Add cooking tools
    return cmd;
}
```

**What's happening under the hood**:
1. **Command building**: Creates command structures with arguments
2. **Pipeline handling**: Connects multiple commands with pipes
3. **Redirection setup**: Prepares input/output redirections
4. **Error detection**: Catches syntax errors

**Why this matters**: This transforms raw tokens into executable command structures.

---

## The Data Flow: How Information Moves

### Step-by-Step Data Flow
```
User Input: "echo 'hello world' | grep hello"
    ↓
1. REPL Loop: Reads input, adds to history
    ↓
2. Lexer: Tokenizes input
    Tokens: [echo, 'hello world', |, grep, hello]
    ↓
3. Parser: Builds command structures
    Command 1: argv=["echo", "hello world"]
    Command 2: argv=["grep", "hello"]
    Pipeline: Command1 | Command2
    ↓
4. Executor: Runs commands (Phase 5)
    ↓
5. Output: Shows results
```

### Memory Management: The Cleanup Crew
**The Analogy**: Restaurant cleanup after each meal

**Key Learning**:
```c
// parser_memory.c - Centralized cleanup
t_parser *parser_alloc(void)
{
    t_parser *parser = malloc(sizeof(t_parser));
    if (!parser) return NULL;  // Check for failure
    parser->error = 0;         // Initialize state
    return parser;
}

void parser_destroy(t_parser *parser)
{
    if (!parser) return;
    if (parser->current_token) token_destroy(parser->current_token);
    if (parser->cmd_list) cmd_destroy_list(parser->cmd_list);
    free(parser);  // Always clean up
}
```

**What's happening under the hood**:
1. **Allocation**: Every malloc has a corresponding free
2. **Error checking**: Every allocation is checked for failure
3. **Cleanup chains**: Destroying parent objects destroys children
4. **Memory safety**: No memory leaks, no double-free

**Why this matters**: Memory leaks crash programs and waste system resources.

---

## Error Handling: The Safety Net

### The Analogy: Restaurant error handling
- **Customer complaint**: Handle gracefully, don't crash
- **Kitchen problem**: Report clearly, continue service
- **System failure**: Clean up and exit safely

**Key Learning**:
```c
// error.c - Centralized error reporting
void print_error(const char *context, const char *message)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    if (context) {
        ft_putstr_fd((char *)context, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
    }
    if (message)
        ft_putstr_fd((char *)message, STDERR_FILENO);
    ft_putchar_fd('\n', STDERR_FILENO);
}

// Usage throughout the code
if (parser->error || !cmd_list)
{
    print_error("parser", "Syntax error");
    return (1);
}
```

**What's happening under the hood**:
1. **Consistent format**: All errors follow the same pattern
2. **Context information**: Shows where the error occurred
3. **Graceful degradation**: System continues working when possible
4. **User-friendly messages**: Clear, actionable error descriptions

**Why this matters**: Good error handling makes the shell reliable and user-friendly.

---

## Signal Handling: The Interrupt System

### The Analogy: Restaurant emergency procedures
- **Fire alarm (Ctrl+C)**: Stop current task, show new prompt
- **Power outage (Ctrl+D)**: Gracefully shut down
- **Kitchen emergency (Ctrl+\)**: Handle safely

**Key Learning**:
```c
// signals.c - Emergency procedures
void signal_handler(int sig)
{
    g_signal = sig;  // Set the emergency flag
}

// loop.c - Emergency response
if (g_signal == SIGINT)  // Fire alarm went off
{
    if (!shell->is_interactive)
        break;      // Non-interactive: evacuate
    continue;       // Interactive: show new prompt
}
```

**What's happening under the hood**:
1. **Signal registration**: Tell the system which signals to handle
2. **Global flag**: Use `g_signal` to communicate between signal handler and main loop
3. **Context awareness**: Different behavior for interactive vs non-interactive
4. **Safe handling**: Never call unsafe functions in signal handlers

**Why this matters**: Proper signal handling makes the shell responsive and safe.

---

## The Architecture: Why It Works

### Modular Design
**The Analogy**: Restaurant departments
- **Kitchen (Parser)**: Prepares the food
- **Service (REPL)**: Manages customer interaction
- **Management (Error Handling)**: Handles problems
- **Maintenance (Memory Management)**: Keeps everything clean

**Key Learning**:
```c
// Each module has a clear responsibility
src/
├── app/          // Customer service (REPL loop)
├── lexeme/       // Ingredient preparation (tokenization)
├── parser/       // Recipe organization (command building)
├── signals/      // Emergency procedures
└── utils/        // Support services (error handling)
```

### Centralized Headers
**The Analogy**: Restaurant's central communication system
- **minishell.h**: The main switchboard
- **Individual files**: Just need to connect to the switchboard

**Key Learning**:
```c
// minishell.h - The central switchboard
#include <stdio.h>
#include <stdlib.h>
// ... all system headers
#include "../lib/libft/libft.h"
#include "tokens.h"
#include "cmd.h"
#include "signals.h"

// Individual files - just connect to switchboard
#include "minishell.h"
```

**Why this works**:
1. **Single source of truth**: All includes in one place
2. **Easy maintenance**: Change includes in one place
3. **Consistent environment**: All files see the same headers
4. **Reduced complexity**: Files don't need to manage dependencies

---

## Key Learnings Summary

### 1. Separation of Concerns
- **Each module** has one clear responsibility
- **Clear interfaces** between modules
- **Independent testing** of each component

### 2. Centralized Management
- **Memory management**: All allocation/deallocation in dedicated functions
- **Error handling**: Consistent error reporting throughout
- **Header management**: Single source of truth for includes

### 3. Defensive Programming
- **Every allocation checked** for failure
- **Every function validates** its inputs
- **Graceful error recovery** whenever possible

### 4. State Management
- **Global signal flag**: Safe communication between signal handler and main loop
- **Parser state**: Tracks position and context during parsing
- **Shell state**: Maintains environment and status information

### 5. Resource Management
- **Allocation ownership**: Clear who owns what memory
- **Cleanup chains**: Destroying parent objects destroys children
- **No resource leaks**: Every resource is properly cleaned up

---

## The Big Picture: Why This Architecture Works

### Scalability
- **Easy to add features**: Clear patterns to follow
- **Modular testing**: Test each component independently
- **Maintainable code**: Changes in one module don't break others

### Reliability
- **Error handling**: Graceful failure recovery
- **Memory safety**: No leaks or crashes
- **Signal safety**: Proper interrupt handling

### Performance
- **Efficient parsing**: Single pass through input
- **Minimal allocations**: Reuse structures where possible
- **Fast error detection**: Fail early, fail fast

### 42 Compliance
- **Norminette compliant**: Clean, readable code
- **Professional structure**: Industry-standard practices
- **Comprehensive testing**: Thorough validation

---

## What Happens When You Type a Command

### Real Example: `echo "hello world" | grep hello`

1. **User types command** → REPL loop reads it
2. **Lexer tokenizes** → `[echo, "hello world", |, grep, hello]`
3. **Parser builds structures** → Two commands connected by pipe
4. **Command 1**: `echo "hello world"` → Will output "hello world"
5. **Command 2**: `grep hello` → Will filter for lines containing "hello"
6. **Pipeline setup** → Command 1's output becomes Command 2's input
7. **Execution** → Commands run in sequence (Phase 5)
8. **Result** → "hello world" (matches the grep pattern)

**The beauty**: Complex command broken down into simple, manageable steps!

---

## Conclusion

This architecture gives us a solid foundation for building a complete shell. Each phase builds on the previous one, creating a robust, maintainable, and extensible system. The modular design, centralized management, and defensive programming practices ensure that the shell is reliable, performant, and ready for the next phase of development.

**Key Takeaways**:
- **Modular design** makes the code maintainable and testable
- **Centralized management** reduces complexity and errors
- **Defensive programming** makes the system robust and reliable
- **Clear data flow** makes the system easy to understand and debug
- **Professional practices** ensure 42 compliance and industry standards

---

*This implementation guide provides a comprehensive understanding of how the minishell works under the hood, from the high-level architecture to the low-level implementation details.*
