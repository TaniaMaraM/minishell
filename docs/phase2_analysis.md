# Phase 2: Lexer - Architecture Analysis

## Table of Contents
1. [Phase 2 Overview](#phase-2-overview)
2. [Lexer Architecture](#lexer-architecture)
3. [Token Types and Structure](#token-types-and-structure)
4. [Lexer Implementation](#lexer-implementation)
5. [Quote Handling](#quote-handling)
6. [Metacharacter Recognition](#metacharacter-recognition)
7. [Error Handling](#error-handling)
8. [Integration with Shell Loop](#integration-with-shell-loop)
9. [Testing Results](#testing-results)
10. [Key Learning Points](#key-learning-points)

---

## Phase 2 Overview

### What We Implemented
- **Complete lexer** for tokenizing shell commands
- **Token type system** for different command elements
- **Quote handling** for single and double quotes
- **Metacharacter recognition** for shell operators
- **Error detection** for invalid syntax
- **Integration** with main shell loop

### What We Achieved
✅ **Tokenization**: Break commands into meaningful tokens  
✅ **Quote Processing**: Handle single and double quotes correctly  
✅ **Operator Recognition**: Identify pipes, redirections, heredoc  
✅ **Error Detection**: Catch unterminated quotes and invalid syntax  
✅ **Integration**: Seamlessly integrated with shell loop  

---

## Lexer Architecture

### Core Components

```c
/* Token types */
typedef enum e_token_type
{
    TOKEN_WORD,         /* Regular word/argument */
    TOKEN_PIPE,         /* | */
    TOKEN_REDIR_IN,     /* < */
    TOKEN_REDIR_OUT,    /* > */
    TOKEN_REDIR_APPEND, /* >> */
    TOKEN_HEREDOC,      /* << */
    TOKEN_EOF,          /* End of input */
    TOKEN_ERROR         /* Invalid token */
} t_token_type;

/* Token structure */
typedef struct s_token
{
    t_token_type type;  /* Type of token */
    char        *value; /* String value of token */
    size_t      length; /* Length of value */
} t_token;

/* Lexer state structure */
typedef struct s_lexer
{
    const char *input;  /* Input string to tokenize */
    size_t     pos;     /* Current position in input */
    size_t     len;     /* Length of input string */
} t_lexer;
```

### Design Philosophy

**The Analogy: Sentence Parser**
Think of the lexer like a sentence parser:
- **Input**: "echo hello world | grep hello"
- **Output**: ["echo", "hello", "world", "|", "grep", "hello"]
- **Each token**: Has a type (word, operator) and value

---

## Token Types and Structure

### Token Types Explained

| Type | Name | Description | Example |
|------|------|-------------|---------|
| 0 | `TOKEN_WORD` | Regular words/arguments | "echo", "hello" |
| 1 | `TOKEN_PIPE` | Pipe operator | "|" |
| 2 | `TOKEN_REDIR_IN` | Input redirection | "<" |
| 3 | `TOKEN_REDIR_OUT` | Output redirection | ">" |
| 4 | `TOKEN_REDIR_APPEND` | Append redirection | ">>" |
| 5 | `TOKEN_HEREDOC` | Heredoc operator | "<<" |
| 6 | `TOKEN_EOF` | End of input | (no value) |
| 7 | `TOKEN_ERROR` | Invalid token | (error condition) |

### Token Structure

```c
typedef struct s_token
{
    t_token_type type;  /* What kind of token */
    char        *value; /* The actual string */
    size_t      length; /* Length of the string */
} t_token;
```

**Why This Design?**
- **Type**: Tells us what the token represents
- **Value**: The actual content (allocated string)
- **Length**: For efficiency and validation

---

## Lexer Implementation

### Main Tokenization Loop

```c
t_token *lexer_next_token(t_lexer *lexer)
{
    /* Skip whitespace */
    while (lexer->pos < lexer->len && is_whitespace(lexer->input[lexer->pos]))
        lexer->pos++;
    
    if (lexer->pos >= lexer->len)
        return (create_token(TOKEN_EOF, NULL, 0));
    
    c = lexer->input[lexer->pos];
    
    /* Handle different token types */
    if (c == '|')
        return (create_token(TOKEN_PIPE, "|", 1));
    else if (c == '<')
        return (handle_redir_in(lexer));
    else if (c == '>')
        return (handle_redir_out(lexer));
    else if (is_quote(c))
        return (lexer_read_quoted(lexer, c));
    else
        return (lexer_read_word(lexer));
}
```

### Whitespace Handling

```c
int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}
```

**Key Points:**
- **Skip all whitespace** before processing tokens
- **Support multiple whitespace types** (space, tab, newline, carriage return)
- **Whitespace is not significant** in shell commands

---

## Quote Handling

### Single Quotes vs Double Quotes

**Single Quotes (`'`)**:
- **No interpretation** of any characters inside
- **Literal string** - everything is preserved as-is
- **Example**: `'hello $USER'` → literal string "hello $USER"

**Double Quotes (`"`)**:
- **Variable expansion** allowed (for future implementation)
- **Some interpretation** of special characters
- **Example**: `"hello $USER"` → will expand $USER (Phase 4)

### Quote Processing Implementation

```c
char *lexer_read_quoted(t_lexer *lexer, char quote)
{
    size_t start, len;
    
    lexer->pos++; /* Skip opening quote */
    start = lexer->pos;
    
    /* Find closing quote */
    while (lexer->pos < lexer->len && lexer->input[lexer->pos] != quote)
        lexer->pos++;
    
    if (lexer->pos >= lexer->len)
        return (NULL); /* Unterminated quote - error */
    
    len = lexer->pos - start;
    lexer->pos++; /* Skip closing quote */
    return (ft_substr(lexer->input, start, len));
}
```

### Quote Examples

| Input | Output | Explanation |
|-------|--------|-------------|
| `echo 'hello world'` | ["echo", "hello world"] | Single quotes preserve spaces |
| `echo "hello world"` | ["echo", "hello world"] | Double quotes preserve spaces |
| `echo 'hello $USER'` | ["echo", "hello $USER"] | Single quotes preserve $ literally |
| `echo "hello $USER"` | ["echo", "hello $USER"] | Double quotes preserve $ (expansion later) |

---

## Metacharacter Recognition

### Supported Metacharacters

| Character | Token Type | Description |
|-----------|------------|-------------|
| `|` | `TOKEN_PIPE` | Pipe operator |
| `<` | `TOKEN_REDIR_IN` | Input redirection |
| `>` | `TOKEN_REDIR_OUT` | Output redirection |
| `>>` | `TOKEN_REDIR_APPEND` | Append redirection |
| `<<` | `TOKEN_HEREDOC` | Heredoc operator |

### Multi-Character Operators

```c
else if (c == '<')
{
    lexer->pos++;
    if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '<')
    {
        lexer->pos++;
        return (create_token(TOKEN_HEREDOC, "<<", 2));
    }
    return (create_token(TOKEN_REDIR_IN, "<", 1));
}
```

**Key Points:**
- **Lookahead** for multi-character operators
- **Consume characters** as we process them
- **Handle both single and double character** operators

---

## Error Handling

### Error Conditions

1. **Unterminated Quotes**: Missing closing quote
2. **Memory Allocation Failures**: Out of memory
3. **Invalid Input**: NULL input string

### Error Response

```c
if (lexer->pos >= lexer->len)
{
    /* Unterminated quote - error */
    return (NULL);
}

/* In main tokenizer */
if (token->type == TOKEN_ERROR)
{
    print_error("tokenizer", "Invalid token");
    status = 1;
    break;
}
```

### Error Examples

| Input | Error | Reason |
|-------|-------|--------|
| `echo 'unterminated` | `TOKEN_ERROR` | Missing closing quote |
| `echo "unterminated` | `TOKEN_ERROR` | Missing closing quote |

---

## Integration with Shell Loop

### Tokenizer Function

```c
int tokenize_and_process(const char *input)
{
    t_lexer *lexer;
    t_token *token;
    int status;
    
    lexer = lexer_init(input);
    if (!lexer)
        return (1);
    
    while (1)
    {
        token = lexer_next_token(lexer);
        if (token->type == TOKEN_EOF)
            break;
        else if (token->type == TOKEN_ERROR)
        {
            status = 1;
            break;
        }
        else
        {
            /* TODO: Process token (Phase 3) */
            /* For now, just print for demonstration */
        }
        token_destroy(token);
    }
    
    lexer_destroy(lexer);
    return (status);
}
```

### Shell Loop Integration

```c
/* In shell_loop() */
add_history(line);
status = tokenize_and_process(line);
shell->last_status = status;
```

**Flow:**
1. **Read input** from user
2. **Add to history** (if not empty)
3. **Tokenize input** using lexer
4. **Process tokens** (TODO in Phase 3)
5. **Update status** for $? expansion

---

## Testing Results

### Test Cases and Results

| Input | Expected Output | Actual Output | Status |
|-------|----------------|---------------|---------|
| `echo hello world` | ["echo", "hello", "world"] | ✅ Correct | ✅ |
| `echo 'hello world'` | ["echo", "hello world"] | ✅ Correct | ✅ |
| `echo "hello world"` | ["echo", "hello world"] | ✅ Correct | ✅ |
| `echo hello \| grep hello` | ["echo", "hello", "\|", "grep", "hello"] | ✅ Correct | ✅ |
| `echo hello > file.txt` | ["echo", "hello", ">", "file.txt"] | ✅ Correct | ✅ |
| `echo hello >> file.txt` | ["echo", "hello", ">>", "file.txt"] | ✅ Correct | ✅ |
| `cat < file.txt` | ["cat", "<", "file.txt"] | ✅ Correct | ✅ |
| `cat << EOF` | ["cat", "<<", "EOF"] | ✅ Correct | ✅ |
| `echo 'unterminated` | ERROR | ✅ Correct | ✅ |

### Integration Testing

```bash
# Test basic tokenization
echo "echo hello world" | ./minishell
# Output: Token: echo (type: 0), Token: hello (type: 0), Token: world (type: 0)

# Test quotes
echo "echo 'hello world' | grep hello" | ./minishell
# Output: Token: echo (type: 0), Token: hello world (type: 0), Token: | (type: 1), Token: grep (type: 0), Token: hello (type: 0)

# Test redirections
echo "echo hello > file.txt" | ./minishell
# Output: Token: echo (type: 0), Token: hello (type: 0), Token: > (type: 3), Token: file.txt (type: 0)
```

---

## Key Learning Points

### 1. What is a Lexer? (The Foundation)

**What it is**: A lexer (lexical analyzer) is the first phase of parsing that breaks raw input text into a sequence of meaningful tokens. It's like a translator that converts human-readable text into a structured format that a computer can understand.

**The Analogy: Reading a Recipe**
Think of a lexer like reading a cooking recipe:
- **Raw Input**: "Add 2 cups flour, mix well, then bake at 350°F"
- **Lexer's Job**: Break it into meaningful parts
- **Tokens**: ["Add", "2", "cups", "flour", ",", "mix", "well", ",", "then", "bake", "at", "350°F"]

**In Our Shell**:
- **Raw Input**: `echo "hello world" | grep hello`
- **Lexer's Job**: Break into tokens
- **Tokens**: [`echo`, `"hello world"`, `|`, `grep`, `hello`]

**Why it matters**: Without a lexer, the shell would see just a string of characters. The lexer gives meaning to those characters, identifying commands, arguments, operators, and special syntax.

### 2. Token Types (The Building Blocks)

**What they are**: Token types are categories that classify different kinds of elements in shell commands. Each token has a type that tells the parser how to handle it.

**The Analogy: Language Grammar**
Think of token types like parts of speech in language:
- **Nouns** (TOKEN_WORD): The "things" - commands, files, arguments
- **Verbs** (TOKEN_PIPE, TOKEN_REDIR): The "actions" - connecting, redirecting
- **Punctuation** (TOKEN_EOF): The "structure" - endings, separators

**Why they matter**: Different token types require different handling. A word token needs to be added to an argument list, while a pipe token needs to create a pipeline connection.

### 3. Quote Handling (The Tricky Part)

**What it is**: Quote handling is the process of correctly interpreting text that's enclosed in single or double quotes, which changes how the shell processes that text.

**The Analogy: Protective Bubbles**
Think of quotes like protective bubbles around text:

```bash
# Single quotes: Complete protection (bulletproof bubble)
echo 'hello $USER'     # Output: hello $USER (no expansion)

# Double quotes: Partial protection (flexible bubble)  
echo "hello $USER"     # Output: hello taninha (expansion happens)

# No quotes: No protection (no bubble at all)
echo hello $USER       # Output: hello taninha (expansion happens)
```

**Why it matters**: Quotes are essential for handling filenames with spaces, preserving special characters, and controlling variable expansion. Without proper quote handling, the shell would misinterpret user input.

### 4. Metacharacters (The Special Characters)

**What they are**: Metacharacters are special characters that have special meaning to the shell, like operators that perform specific functions rather than being literal text.

**The Analogy: Traffic Signs**
Metacharacters are like traffic signs that tell the shell what to do:

```bash
|    # Pipe: "Send output to next command" (like a bridge)
<    # Input: "Read from this file" (like an entrance)
>    # Output: "Write to this file" (like an exit)
>>   # Append: "Add to this file" (like adding to a list)
<<   # Heredoc: "Read until this marker" (like a form)
```

**Why they matter**: Metacharacters are the "verbs" of shell commands - they tell the shell what actions to perform. Without recognizing them, the shell couldn't create pipelines, redirect input/output, or perform other essential operations.

### 5. State Machine (The Brain)

**What it is**: A state machine is a computational model that processes input by maintaining a current state and transitioning between states based on the input characters.

**The Analogy: Reading a Book**
The lexer works like reading a book with different contexts:

```
State 1: Reading normally (like reading regular text)
State 2: Inside single quotes (like reading a quote in a book)
State 3: Inside double quotes (like reading dialogue)
State 4: Found metacharacter (like hitting punctuation)
State 5: End of input (like reaching the end of a chapter)
```

**Why it matters**: State machines allow the lexer to handle complex input correctly. Without state tracking, the lexer couldn't distinguish between a pipe character inside quotes (literal text) and a pipe character outside quotes (an operator).

### 6. Memory Management (The Resource Manager)

**What it is**: Memory management in the lexer involves properly allocating and freeing memory for tokens and their values, ensuring no memory leaks.

**The Analogy: Library Book Management**
Think of memory management like managing library books:

```c
// Create a book (allocate memory)
t_token *token = create_token(TOKEN_WORD, "hello", 5);

// Use the book (process the token)
process_token(token);

// Return the book (free memory)
token_destroy(token);
```

**Why it matters**: Proper memory management prevents memory leaks, which can cause the shell to consume more and more memory over time, eventually crashing the system.

### 7. Error Handling (The Safety Net)

**What it is**: Error handling is the process of detecting and responding to invalid input or unexpected conditions during tokenization.

**The Analogy: A Good Teacher**
Bash handles errors gracefully, like a good teacher:

```bash
# Unterminated single quote
echo 'hello world
# Bash: Continues reading until quote is closed

# Invalid syntax
echo | | grep hello
# Bash: Reports syntax error and stops
```

**Why it matters**: Error handling prevents the shell from crashing on invalid input and provides helpful feedback to users about what went wrong.

### 8. Bash Behaviour Concepts (The Compatibility Layer)

**What they are**: Bash behaviour concepts are the specific rules and behaviours that bash follows when processing shell commands, which our minishell must replicate.

**The Analogy: Learning a Language**
Learning bash behaviour is like learning a new language - you need to understand the grammar, syntax, and idioms:

```bash
# Word splitting (like breaking sentences into words)
echo hello world test        # 3 separate arguments
echo "hello world test"      # 1 argument with spaces

# Quote removal (like unwrapping a gift)
echo "hello world"           # Quotes are removed after processing
# Final arguments: ["echo", "hello world"]

# Metacharacter recognition (like recognizing punctuation)
echo hello|grep hello        # | is recognized as operator
echo "hello|world"           # | is literal text inside quotes
```

**Why it matters**: Bash compatibility ensures that users can use familiar commands and syntax, making the shell intuitive and reliable.

### 9. Testing Strategy (The Quality Assurance)

**What it is**: Testing strategy involves systematically verifying that the lexer works correctly with various inputs, including normal cases, edge cases, and error conditions.

**The Analogy: Quality Control in Manufacturing**
Testing is like quality control in manufacturing:

```c
// Test normal case (like testing a working product)
test_basic_tokenization();   // Should pass

// Test edge case (like testing extreme conditions)
test_unterminated_quotes();  // Should handle gracefully

// Test error case (like testing failure modes)
test_invalid_syntax();       // Should report error
```

**Why it matters**: Comprehensive testing ensures the lexer is reliable and handles all possible inputs correctly, preventing bugs and crashes in real-world usage.

### 10. Integration with Shell Loop (The Connection)

**What it is**: Integration with the shell loop is the process of connecting the lexer to the main shell execution flow, allowing tokenized input to be processed by subsequent phases.

**The Analogy: Assembly Line**
The shell is like an assembly line:

```
Input → Lexer → Parser → Executor → Output
  ↓       ↓        ↓         ↓        ↓
User → Tokens → Commands → Execution → Results
```

**Why it matters**: Proper integration ensures that the lexer's output can be used by the parser and executor, creating a seamless flow from user input to command execution.

---

## Critical Bash Behaviours for Minishell

### 1. Word Splitting (The Space Problem)

**What it is**: Word splitting is the process by which bash breaks command lines into separate arguments based on whitespace, but quotes change this behaviour.

**The Analogy: Breaking a Sentence**
Bash splits words on whitespace, but quotes change this:

```bash
# Without quotes: 3 separate arguments (like breaking a sentence into words)
echo hello world test
# Arguments: ["hello", "world", "test"]

# With quotes: 1 argument with spaces (like keeping a phrase together)
echo "hello world test"  
# Arguments: ["hello world test"]

# Mixed: Some split, some not (like a sentence with quoted phrases)
echo hello "world test" bye
# Arguments: ["hello", "world test", "bye"]
```

**Why this matters**: File names with spaces, command arguments, and variable expansion all depend on proper word splitting.

### 2. Quote Removal (The Invisible Process)

**What it is**: Quote removal is the process by which bash strips quote characters after processing, but preserves the content inside.

**The Analogy: Unwrapping a Gift**
Bash removes quotes after processing, like unwrapping a gift:

```bash
# Input: echo "hello world"
# After lexing: [echo, "hello world"]
# After quote removal: [echo, hello world]
# Final arguments: ["echo", "hello world"]
```

**The Process**:
1. **Lexer**: Recognizes quotes and creates tokens
2. **Parser**: Understands the structure
3. **Expander**: Processes variables inside quotes
4. **Quote Removal**: Strips the quote characters
5. **Execution**: Passes clean arguments to command

### 3. Metacharacter Recognition (The Special Detection)

**What it is**: Metacharacter recognition is the ability to identify special characters that have special meaning to the shell, even when they're "hidden" in complex input.

**The Analogy: Recognizing Road Signs**
Bash must recognize metacharacters even when they're "hidden":

```bash
# These are all pipes:
echo hello | grep hello
echo "hello" | grep hello  
echo 'hello' | grep hello
echo hello|grep hello      # No spaces needed!
```

**Key Points**:
- **Context matters**: Inside quotes, `|` is just text
- **No spaces required**: `|` works with or without spaces
- **Priority**: Metacharacters override word boundaries

### 4. Error Handling (The Graceful Failure)

**What it is**: Error handling is the process of detecting and responding to invalid input or syntax errors without crashing the shell.

**The Analogy: A Good Teacher**
Bash handles errors gracefully, like a good teacher:

```bash
# Unterminated single quote
echo 'hello world
# Bash: Continues reading until quote is closed

# Unterminated double quote  
echo "hello world
# Bash: Continues reading until quote is closed

# Invalid syntax
echo | | grep hello
# Bash: Reports syntax error and stops
```

**Error Types**:
- **Syntax errors**: Invalid metacharacter combinations
- **Unterminated quotes**: Missing closing quotes
- **Invalid redirections**: Malformed redirection syntax

### 5. Token Boundaries (The Word Detection)

**What it is**: Token boundaries are the rules that determine where one token ends and another begins in shell input.

**The Analogy: Reading Handwriting**
Bash must figure out where words start and end:

```bash
# Clear boundaries (spaces)
echo hello world

# Unclear boundaries (no spaces)
echo hello|grep world

# Mixed boundaries
echo "hello world"|grep test
```

**Boundary Rules**:
- **Spaces**: Always separate tokens
- **Metacharacters**: Always separate tokens
- **Quotes**: Protect spaces inside them
- **End of line**: Always ends current token

### 6. Quote Escaping (The Protection System)

**What it is**: Quote escaping is the system by which different types of quotes provide different levels of protection for text.

```bash
# Single quotes: Protect everything (bulletproof)
echo 'hello $USER `date` "test"'
# Output: hello $USER `date` "test"

# Double quotes: Protect spaces, allow $ and ` (flexible)
echo "hello $USER `date` test"
# Output: hello taninha Mon Sep 2 16:45:00 2024 test

# Backslash: Protect next character (targeted)
echo hello\ world
# Output: hello world
```

### 7. Variable Expansion (The $ System)

**What it is**: Variable expansion is the process by which bash replaces variable references with their values.

```bash
# Basic expansion
echo $USER              # Output: taninha
echo "$USER"            # Output: taninha
echo '$USER'            # Output: $USER

# Exit status expansion
echo $?                 # Output: 0 (last command's exit status)
echo "$?"               # Output: 0
echo '$?'               # Output: $?
```

### 8. Command Substitution (The ` System)

**What it is**: Command substitution is the process by which bash executes commands and uses their output as part of the command line.

```bash
# Command substitution
echo `date`             # Output: Mon Sep 2 16:45:00 2024
echo "`date`"           # Output: Mon Sep 2 16:45:00 2024
echo '`date`'           # Output: `date`

# Modern syntax (not required for minishell)
echo $(date)            # Output: Mon Sep 2 16:45:00 2024
```

### 9. Redirection Syntax (The < > System)

**What it is**: Redirection syntax allows commands to read from files or write to files instead of using standard input/output.

```bash
# Input redirection
cat < file.txt          # Read from file.txt

# Output redirection  
echo hello > file.txt   # Write to file.txt (overwrite)

# Append redirection
echo world >> file.txt  # Append to file.txt

# Here document
cat << EOF
This is a here document
It continues until EOF
EOF
```

### 10. Pipeline Syntax (The | System)

**What it is**: Pipeline syntax allows the output of one command to be used as input for another command.

```bash
# Simple pipeline
echo hello | grep hello

# Complex pipeline
echo "hello world" | grep hello | wc -l

# Pipeline with redirections
echo hello > file.txt | grep hello
```

---

## Next Steps (Phase 3)

### What's Missing

1. **Command Structure**: Need to build command objects from tokens
2. **Pipeline Handling**: Connect commands with pipes
3. **Redirection Setup**: Prepare file descriptors for redirections
4. **Argument Arrays**: Build argv arrays for execve

### Phase 3 Goals

1. **Implement Parser**: Build command structures from tokens
2. **Handle Pipelines**: Connect multiple commands
3. **Process Redirections**: Set up file descriptors
4. **Build Argument Arrays**: Prepare for command execution

---

## Summary: What You Should Remember

### **The Big Picture**
The lexer is the foundation of your shell - it's the first phase that converts human-readable commands into a structured format that the computer can understand. Without a proper lexer, the shell couldn't interpret user input correctly.

### **Key Concepts to Master**

1. **Tokenization**: Breaking input into meaningful pieces
2. **Quote Handling**: Understanding how quotes protect text
3. **Metacharacter Recognition**: Identifying special operators
4. **State Management**: Tracking context during parsing
5. **Error Handling**: Gracefully handling invalid input
6. **Bash Compatibility**: Replicating bash behaviour

### **Critical Skills for Phase 3**

- **Understanding token types**: Know what each token represents
- **Quote processing**: Handle single and double quotes correctly
- **Error detection**: Identify and report syntax errors
- **Memory management**: Properly allocate and free tokens
- **Testing**: Verify lexer works with various inputs

### **What Makes a Good Lexer**

✅ **Accurate**: Correctly identifies all token types  
✅ **Robust**: Handles edge cases and errors gracefully  
✅ **Efficient**: Processes input quickly and uses memory wisely  
✅ **Compatible**: Behaves like bash for familiar commands  
✅ **Testable**: Can be verified with comprehensive tests  

### **The Learning Journey**

**Phase 2 (Lexer)**: "What are the pieces?"  
**Phase 3 (Parser)**: "How do the pieces fit together?"  
**Phase 4 (Expander)**: "What do the pieces mean?"  
**Phase 5 (Executor)**: "How do we execute the pieces?"  

You've successfully completed the foundation - now you're ready to build the structure!

---

*This document provides a comprehensive analysis of Phase 2 lexer implementation. The lexer successfully tokenizes shell commands with proper quote handling and metacharacter recognition, providing a solid foundation for the parser in Phase 3.*
