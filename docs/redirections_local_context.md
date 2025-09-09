# Redirections with Local Context - Technical Implementation

## **Overview**

This document explains how we implemented redirections using a local context approach that complies with the 42 minishell subject constraint of "at most one global variable."

## **The Problem**

The 42 subject explicitly states: **"at most one global variable"** to store a received signal number. Using global variables for redirections would violate this rule and result in a 0 grade.

### **What We Avoided (❌ Subject Violation)**
```c
// Multiple global variables - VIOLATES SUBJECT
int g_stdin_backup;
int g_stdout_backup; 
int g_stderr_backup;

int setup_redirections(t_redir *redirs) {
    g_stdin_backup = dup(STDIN_FILENO);
    g_stdout_backup = dup(STDOUT_FILENO);
    g_stderr_backup = dup(STDERR_FILENO);
    // ... apply redirections
}

int restore_redirections(void) {
    dup2(g_stdin_backup, STDIN_FILENO);
    dup2(g_stdout_backup, STDOUT_FILENO);
    dup2(g_stderr_backup, STDERR_FILENO);
    // ... cleanup
}
```

**Problems with this approach:**
- **Subject violation**: More than one global variable
- **Thread safety**: Multiple processes could interfere
- **State corruption**: If one command fails, backups are lost
- **Memory leaks**: Global state persists between commands

## **Our Solution: Local Context Approach**

### **Core Principle**
Since we use **child processes** for command execution, we don't need to restore redirections in the parent. Each child process handles its own redirections and then exits, automatically cleaning up all file descriptors.

### **Implementation**

#### **1. Simple Redirection Setup**
```c
int setup_redirections(t_redir *redirs)
{
    t_redir *current;

    if (!redirs)
        return (0);
    current = redirs;
    while (current)
    {
        if (process_redirection(current))
            return (1);
        current = current->next;
    }
    return (0);
}
```

#### **2. Redirection Processing**
```c
static int process_redirection(t_redir *redir)
{
    if (redir->type == REDIR_IN)
        return (handle_input_redirection(redir->file));
    else if (redir->type == REDIR_OUT)
        return (handle_output_redirection(redir->file));
    else if (redir->type == REDIR_APPEND)
        return (handle_append_redirection(redir->file));
    else if (redir->type == REDIR_HEREDOC)
        return (handle_heredoc_redirection(redir->file));
    return (0);
}
```

#### **3. Individual Redirection Handlers**
```c
int handle_input_redirection(const char *file)
{
    int fd = open(file, O_RDONLY);
    if (fd == -1) {
        print_error(file, strerror(errno));
        return (1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    return (0);
}

int handle_output_redirection(const char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        print_error(file, strerror(errno));
        return (1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return (0);
}
```

## **How It Works in Practice**

### **Command Execution Flow**

#### **Single Command Execution**
```
execute_single_command() → fork() → child process:
  1. setup_redirections() applies redirections
  2. execute command (builtin or external)
  3. child process exits (automatic cleanup)
```

#### **Pipeline Execution**
```
execute_pipeline() → for each command:
  1. fork() → child process
  2. setup_pipeline() connects pipes
  3. setup_redirections() applies redirections
  4. execute command in child
  5. child exits (no restoration needed)
```

### **Example: Output Redirection**
```bash
echo "hello world" > output.txt
```

**What happens:**
1. **Parent process**: Parses command, creates child
2. **Child process**: 
   - `handle_output_redirection("output.txt")` opens file
   - `dup2(fd, STDOUT_FILENO)` redirects stdout to file
   - `close(fd)` closes original file descriptor
   - `execve("echo", ["echo", "hello world"], env)` executes
   - Child exits, file descriptors automatically cleaned up

### **Example: Input Redirection**
```bash
cat < input.txt
```

**What happens:**
1. **Parent process**: Parses command, creates child
2. **Child process**:
   - `handle_input_redirection("input.txt")` opens file
   - `dup2(fd, STDIN_FILENO)` redirects stdin from file
   - `close(fd)` closes original file descriptor
   - `execve("cat", ["cat"], env)` executes
   - Child exits, file descriptors automatically cleaned up

### **Example: Pipeline with Redirections**
```bash
cat input.txt | grep "hello" > output.txt
```

**What happens:**
1. **First command** (`cat input.txt`):
   - Child process opens `input.txt` for reading
   - Redirects stdin to file
   - Writes to pipe (stdout redirected to pipe)
   - Child exits

2. **Second command** (`grep "hello" > output.txt`):
   - Child process reads from pipe (stdin redirected to pipe)
   - Opens `output.txt` for writing
   - Redirects stdout to file
   - Child exits

## **Benefits of This Approach**

### **1. Subject Compliance**
- **✅ Follows 42 rules**: Only one global variable (for signals)
- **✅ No grade penalties**: Complies with project requirements
- **✅ Clean architecture**: No global state pollution

### **2. Process Safety**
- **✅ Isolated state**: Each command has its own redirection context
- **✅ No interference**: Multiple commands don't affect each other
- **✅ Thread safe**: No shared global state

### **3. Error Recovery**
- **✅ Automatic cleanup**: Local redirections are cleaned up on errors
- **✅ No state corruption**: Failed commands don't affect subsequent ones
- **✅ Predictable behavior**: Each command is independent

### **4. Memory Management**
- **✅ No memory leaks**: Local variables are automatically cleaned up
- **✅ No file descriptor leaks**: Child processes handle cleanup
- **✅ Stack allocation**: No dynamic allocation for redirections

## **Comparison with Alternatives**

### **Alternative 1: Global Variables (Rejected)**
```c
// ❌ VIOLATES SUBJECT
int g_stdin_backup, g_stdout_backup, g_stderr_backup;
```
**Problems**: Subject violation, thread safety, state corruption

### **Alternative 2: Static Local Variables (Rejected)**
```c
// ❌ Still global-like
int setup_redirections(t_redir *redirs) {
    static int backup_fds[3];  // Acts like global
}
```
**Problems**: Still acts like global variables, not truly local

### **Alternative 3: Structure-Based Context (Rejected)**
```c
// ❌ Overkill for simple case
typedef struct s_redir_context {
    int stdin_backup, stdout_backup, stderr_backup;
} t_redir_context;
```
**Problems**: More complex, requires dynamic allocation, unnecessary

### **Our Approach: Local Context (✅ Chosen)**
```c
// ✅ Simple, subject-compliant
int setup_redirections(t_redir *redirs) {
    // Apply redirections directly
    // Child process handles cleanup automatically
}
```
**Benefits**: Simple, safe, subject-compliant, efficient

## **Error Handling**

### **File Access Errors**
```c
int handle_input_redirection(const char *file)
{
    int fd = open(file, O_RDONLY);
    if (fd == -1) {
        print_error(file, strerror(errno));
        return (1);  // Signal error to caller
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    return (0);
}
```

### **Redirection Chain Errors**
```c
int setup_redirections(t_redir *redirs)
{
    t_redir *current = redirs;
    while (current) {
        if (process_redirection(current))
            return (1);  // Stop on first error
        current = current->next;
    }
    return (0);
}
```

## **Testing Results**

### **Output Redirection**
```bash
$ echo "hello world" > test.txt
$ cat test.txt
hello world
```
**✅ Working correctly**

### **Input Redirection**
```bash
$ echo "hello world" > test.txt
$ cat < test.txt
hello world
```
**✅ Working correctly**

### **Pipelines**
```bash
$ echo "hello world" | wc -w
2
```
**✅ Working correctly**

### **Multiple Redirections**
```bash
$ echo "hello" > input.txt
$ cat < input.txt > output.txt
$ cat output.txt
hello
```
**✅ Working correctly**

## **Integration with Project Architecture**

### **Executor Integration**
```c
int execute_single_command(t_cmd *cmd, t_shell *shell)
{
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        if (setup_redirections(cmd->redirs))
            exit(1);
        if (is_builtin(cmd->argv[0]))
            exit(execute_builtin_in_child(cmd, shell));
        else
            exit(execute_external_in_child(cmd, shell));
    } else {
        // Parent process - no restoration needed
        waitpid(pid, &status, 0);
        return (WEXITSTATUS(status));
    }
}
```

### **Pipeline Integration**
```c
int execute_pipeline(t_cmd *cmd_list, t_shell *shell)
{
    // Each command in pipeline gets its own local context
    // No shared state between pipeline commands
    // Each child process handles its own redirections
}
```

## **Defense Points for Peer Evaluation**

1. **"We strictly follow the subject"**: Only one global variable for signals, as required.

2. **"Local context is safer"**: Each command has isolated redirection state, preventing interference.

3. **"Child process model eliminates complexity"**: No need for restoration since child processes exit.

4. **"Simple and efficient"**: No dynamic allocation, automatic cleanup, easy to understand.

5. **"Error recovery is automatic"**: Failed redirections don't corrupt subsequent commands.

## **Conclusion**

The local context approach successfully solves the global variable constraint while maintaining full redirection functionality. It's:

- **Subject-compliant**: Follows 42 rules strictly
- **Safe**: Isolated state prevents interference
- **Simple**: No complex restoration logic needed
- **Efficient**: Automatic cleanup, no memory leaks
- **Maintainable**: Easy to understand and debug

This implementation demonstrates how constraints can lead to better, cleaner designs that are both compliant and robust.
