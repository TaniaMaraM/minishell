# 🐚 Minishell

<div align="center">
  <img src="https://img.shields.io/badge/language-C-blue.svg" alt="Language">
  <img src="https://img.shields.io/badge/42-School-lightgrey.svg" alt="42 School">
  <img src="https://img.shields.io/badge/shell-implementation-yellow.svg" alt="Shell Implementation">
  <img src="https://img.shields.io/badge/status-completed-green.svg" alt="Status">
</div>

## 📜 About

**Minishell** is a sophisticated command-line interpreter that replicates core functionalities of bash, developed as part of the 42 School curriculum. This project challenges students to understand how shells work at a fundamental level by implementing various features like command execution, signal handling, environment variable management, and process control.

Our implementation focuses on clean code practices, memory management, and proper error handling while closely following UNIX principles.

## ✨ Features

- 🔄 **Interactive command prompt** with command history navigation
- 🔍 **Command resolution and execution** through:
  - Absolute paths (`/bin/ls`)
  - Relative paths (`./program`)
  - PATH environment variable search (`ls`)
- 🔤 **Quote handling**:
  - `'Single quotes'` prevent all interpretation
  - `"Double quotes"` prevent interpretation except for `$` variables
- 📥 **I/O Redirections**:
  - Input redirection (`<`)
  - Output redirection (`>`)
  - Append output (`>>`)
  - Heredoc functionality (`<<`)
- 📊 **Pipeline implementation** (`cmd1 | cmd2 | cmd3`)
- 🔠 **Environment variable expansion**:
  - Regular variables (`$USER`, `$HOME`)
  - Exit status (`$?`)
- ⌨️ **Signal handling**:
  - `Ctrl+C` (SIGINT) - Displays a new prompt
  - `Ctrl+D` (EOF) - Exits the shell
  - `Ctrl+\` (SIGQUIT) - Does nothing (like bash in interactive mode)
- 🛠️ **Built-in commands**:
  - `echo` with `-n` option
  - `cd` with relative/absolute path support
  - `pwd` displaying current directory
  - `export` to define environment variables
  - `unset` to remove environment variables
  - `env` to display the environment
  - `exit` with status code support

## 🏗️ Architecture

The project follows a modular architecture with clear separation of concerns:

### 🔄 Core Components

1. **Command Line Interface**
   - Manages the command prompt and input handling
   - Maintains command history
   - Handles signal processing

2. **Lexer**
   - Tokenizes input string into meaningful tokens
   - Identifies commands, arguments, operators, and special characters
   - Handles quote recognition and preservation

3. **Parser**
   - Transforms tokens into an abstract syntax tree (AST)
   - Organizes commands into pipelines and redirections
   - Validates syntax and provides meaningful error messages

4. **Expander**
   - Resolves environment variables
   - Handles special parameter expansion like `$?`
   - Applies quote removal rules

5. **Executor**
   - Manages command execution flow
   - Handles pipeline creation and inter-process communication
   - Sets up I/O redirections
   - Executes built-in commands internally

6. **Signal Handler**
   - Manages interactive signal behavior
   - Uses a single global variable as required by the subject

7. **Environment Manager**
   - Maintains environment variables
   - Provides API for variable lookup, modification, and deletion

## 🚀 Implementation Details

<div align="center">
  <img src="https://img.shields.io/badge/Clean_Code-✓-brightgreen.svg" alt="Clean Code">
  <img src="https://img.shields.io/badge/Memory_Safe-✓-brightgreen.svg" alt="Memory Safe">
  <img src="https://img.shields.io/badge/Error_Handling-✓-brightgreen.svg" alt="Error Handling">
  <img src="https://img.shields.io/badge/Signal_Management-✓-brightgreen.svg" alt="Signal Management">
</div>

### 📊 Memory Management

- **Allocation Tracking**: Comprehensive tracking of all memory allocations
- **Resource Cleanup**: Structured cleanup routines for each component
- **Zero Leaks**: No memory leaks in custom code (excluding readline's internal allocations)
- **FD Management**: Careful tracking and closing of all file descriptors

### ⚙️ Process Control

- **Fork Strategy**: Optimized process creation for command execution
- **Pipe Management**: Robust creation and cleanup of inter-process communication channels
- **Exit Codes**: Proper collection and propagation of command exit statuses
- **Child Processes**: Careful tracking and cleanup of all child processes

### ⚠️ Error Handling

- **Detailed Errors**: Context-aware error messages with specific information
- **Bash Compatibility**: Error formats consistent with bash behavior
- **Graceful Recovery**: Ability to continue after non-fatal errors
- **POSIX Standards**: Exit status codes following POSIX conventions

## 🔧 Installation

```bash
# Clone the repository
git clone https://github.com/yourusername/minishell.git
cd minishell

# Install dependencies
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install gcc make libreadline-dev

# macOS
brew install readline

# Compile the project
make
```

## 🖥️ Usage

### Basic Usage

```bash
# Start the shell
./minishell
```

### Command Examples

```bash
# Basic commands
minishell$ ls -la
minishell$ echo "Hello, world!"
minishell$ pwd

# Redirections
minishell$ ls > files.txt
minishell$ cat < files.txt
minishell$ echo "appended text" >> files.txt

# Pipelines
minishell$ ls -la | grep .c | wc -l

# Environment variables
minishell$ echo $HOME
minishell$ export NEW_VAR=value
minishell$ echo $NEW_VAR

# Heredoc
minishell$ cat << EOF
> Type your multi-line text here
> It will be sent to cat when you type EOF
> EOF
```

## 🧪 Testing

### Comprehensive Testing Guide

We've created a detailed testing guide to help you verify that the minishell behaves correctly in all required scenarios. It includes test cases for:

- Basic command execution
- Built-in commands
- Environment variables
- Quoting and parsing
- Redirections and pipes
- Signal handling
- Error management

```bash
# Access the testing guide
cat minishell_testing_guide.md
```

### Memory Leak Detection

```bash
# Run with Valgrind to check for memory leaks
./test_valgrind.sh
```

> **Note**: The subject explicitly allows memory leaks from the readline library, so these are ignored in the Valgrind output.

### Functional Testing

```bash
# Run the automated test suite
cd tests
./test_evaluation.sh
```

## 📁 Project Structure

```
minishell/
├── include/                 # Header files
│   ├── minishell.h          # Main header with core structures
│   ├── tokens.h             # Token definitions and lexer API
│   ├── cmd.h                # Command structures
│   ├── expand.h             # Expansion functionality
│   ├── exec.h               # Executor definitions
│   ├── builtin.h            # Built-in command declarations
│   └── signals.h            # Signal handling
├── lib/                     # Libraries
│   ├── libft/               # Custom C library
│   └── get_next_line/       # Line reading utility
├── src/                     # Source code
│   ├── app/                 # Core application
│   │   ├── main.c           # Entry point
│   │   ├── loop.c           # Main execution loop
│   │   ├── init.c           # Initialization
│   │   ├── input_handler.c  # Input processing
│   │   └── cleanup.c        # Resource management
│   ├── lexeme/              # Lexical analysis
│   ├── parser/              # Command parsing
│   ├── expand/              # Variable expansion
│   ├── exec/                # Command execution
│   │   ├── command/         # Command handling
│   │   ├── pipeline/        # Pipeline management
│   │   └── heredoc/         # Heredoc implementation
│   ├── builtin/             # Built-in commands
│   ├── signals/             # Signal handling
│   └── utils/               # Utility functions
└── tests/                   # Test scripts
```

## 👥 Contributors

- [Your Name] - [@TaniaMaraM](https://github.com/TaniaMaraM)
- [Your Partner's Name] - [@RenWro](https://github.com/RenWro)

## 📄 License

This project is part of the 42 School curriculum and is subject to its academic guidelines.

---

<div align="center">
  <p>Created with ❤️ by 42 students</p>
</div>
