# Phase 5: Executor - A Complete Beginner's Guide

## **What is an Executor? (The Simple Explanation)**

Imagine you're a restaurant manager with a perfectly organized kitchen. You have all the ingredients prepared, all the cooking instructions written down, and all the equipment ready. Now you need someone to actually cook the meals and serve them to customers.

**An executor** is exactly like that - but for computer commands!

### **Real-World Analogy: A Restaurant Kitchen**
Think of a restaurant kitchen:
- **Input**: Organized recipe cards with ingredients and instructions
- **Executor's job**: Actually cook the meals using the kitchen equipment
- **Output**: Finished dishes served to customers

**In our shell**, it works the same way:
- **Input**: Parsed command structures from the parser
- **Executor's job**: Actually run the commands using system calls
- **Output**: Command results and exit status

---

## **Why Do We Need an Executor?**

### **The Problem Without an Executor**
Without an executor, we'd have parsed commands but no way to run them:

```bash
# Parsed command structure (just data)
Command: {
    argv: ["echo", "hello world"]
    redirs: []
    next: NULL
}
```

**Problems**:
- ❌ **No execution**: Commands just sit there doing nothing
- ❌ **No results**: No output or exit status
- ❌ **No interaction**: Can't communicate with the system

### **The Solution With an Executor**
With an executor, we can actually run commands:

```bash
# Executor runs the command
echo "hello world"
# Output: hello world
# Exit status: 0 (success)
```

**Benefits**:
- ✅ **Actual execution**: Commands run and produce results
- ✅ **System interaction**: Can access files, processes, etc.
- ✅ **User feedback**: Shows output and handles errors

---

## **How the Executor Works (Step by Step)**

### **The Analogy: A Professional Chef**

Think of the executor like a **professional chef** who takes recipe cards and actually cooks:

#### **Step 1: Identify the Command Type**
```c
if (is_builtin(cmd->argv[0])) {
    return execute_builtin_in_child(cmd, shell);
} else {
    return execute_external_in_child(cmd, shell);
}
```
**The Analogy**: Like deciding whether to use kitchen equipment or bring in prepared food.

**What it does**: Determines if the command is built-in or external.

#### **Step 2: Handle Redirections**
```c
setup_redirections(cmd->redirs);
```
**The Analogy**: Like setting up the cooking equipment (oven temperature, etc.).

**What it does**: Sets up input/output redirections before execution.

#### **Step 3: Execute the Command**
```c
if (is_builtin(cmd->argv[0])) {
    execute_builtin_in_child(cmd, shell);
} else {
    execute_external_in_child(cmd, shell);
}
```
**The Analogy**: Like actually cooking the meal.

**What it does**: Runs the command and captures the result.

#### **Step 4: Clean Up**
```c
restore_redirections();
```
**The Analogy**: Like cleaning up the kitchen after cooking.

**What it does**: Restores original input/output and cleans up resources.

---

## **Types of Commands We Handle**

### **The Analogy: Different Types of Cooking**

Think of different command types like **different types of cooking**:

#### **Built-in Commands - Kitchen Equipment**
**What they are**: Commands built into the shell itself.

**The Analogy**: Like using the oven, stove, or microwave that's already in the kitchen.

**Examples**:
- `cd` - Change directory (like moving to a different cooking station)
- `echo` - Print text (like announcing what you're cooking)
- `pwd` - Print working directory (like saying where you are)
- `export` - Set environment variables (like adjusting kitchen settings)
- `unset` - Remove environment variables (like turning off equipment)
- `env` - Show environment variables (like checking kitchen status)
- `exit` - Exit the shell (like closing the kitchen)

#### **External Commands - Bringing in Food**
**What they are**: Programs that exist as separate files on the system.

**The Analogy**: Like bringing in ingredients or prepared food from outside.

**Examples**:
- `ls` - List files (like checking what's in the pantry)
- `grep` - Search text (like finding a specific ingredient)
- `cat` - Display file contents (like reading a recipe)
- `wc` - Count words/lines (like counting ingredients)

---

## **Built-in Commands: Detailed Implementation**

### **1. Echo Command**

#### **What it is**: Prints text to the screen.

**The Analogy**: Like a restaurant announcer who tells customers what's being served.

#### **How it works**:
```c
int builtin_echo(char **argv, t_shell *shell)
{
    int i = 1;
    int print_newline = 1;
    
    // Check for -n flag
    if (argv[1] && ft_strcmp(argv[1], "-n") == 0) {
        print_newline = 0;
        i = 2;
    }
    
    // Print all arguments
    while (argv[i]) {
        ft_putstr_fd(argv[i], STDOUT_FILENO);
        if (argv[i + 1])
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }
    
    // Print newline unless -n flag
    if (print_newline)
        ft_putchar_fd('\n', STDOUT_FILENO);
    
    return (0);
}
```

#### **Key Features**:
- **`-n` flag**: Suppresses the newline at the end
- **Multiple arguments**: Prints all arguments separated by spaces
- **No arguments**: Prints just a newline

#### **Examples**:
```bash
echo hello world          # → hello world
echo -n hello world       # → hello world (no newline)
echo                      # → (just newline)
```

### **2. CD Command**

#### **What it is**: Changes the current working directory.

**The Analogy**: Like moving to a different room in a building.

#### **How it works**:
```c
int builtin_cd(char **argv, t_shell *shell)
{
    if (!argv[1]) {
        // No argument - go to home directory
        return (cd_to_home(shell));
    }
    if (argv[2]) {
        // Too many arguments
        print_error("cd", "too many arguments");
        return (1);
    }
    // Change to specified directory
    return (cd_to_path(argv[1]));
}
```

#### **Key Features**:
- **No argument**: Changes to home directory (`$HOME`)
- **Relative path**: Changes relative to current directory
- **Absolute path**: Changes to absolute path
- **Error handling**: Reports errors for invalid directories

#### **Examples**:
```bash
cd                    # → Go to home directory
cd /tmp               # → Go to /tmp directory
cd ..                 # → Go to parent directory
cd nonexistent        # → Error: No such file or directory
```

### **3. PWD Command**

#### **What it is**: Prints the current working directory.

**The Analogy**: Like asking "Where am I?" in a building.

#### **How it works**:
```c
int builtin_pwd(char **argv, t_shell *shell)
{
    char *cwd = getcwd(NULL, 0);
    if (!cwd) {
        print_error("pwd", strerror(errno));
        return (1);
    }
    ft_putendl_fd(cwd, STDOUT_FILENO);
    free(cwd);
    return (0);
}
```

#### **Key Features**:
- **System call**: Uses `getcwd()` to get current directory
- **Memory management**: Allocates and frees memory properly
- **Error handling**: Reports errors if directory can't be determined

#### **Examples**:
```bash
pwd                   # → /Users/username/project
```

### **4. Export Command**

#### **What it is**: Sets environment variables that can be used by other programs.

**The Analogy**: Like writing down important information on a notice board that everyone can see.

#### **How it works**:
```c
int builtin_export(char **argv, t_shell *shell)
{
    if (!argv[1]) {
        // No arguments - print all exported variables
        print_exported_vars(shell);
        return (0);
    }
    
    int error = 0;
    int i = 1;
    while (argv[i]) {
        if (export_var(shell, argv[i]))
            error = 1;
        i++;
    }
    return (error);
}
```

#### **Key Features**:
- **No arguments**: Lists all exported variables
- **Variable setting**: Sets variables in format `NAME=value`
- **Validation**: Checks if variable names are valid
- **Multiple variables**: Can set multiple variables at once

#### **Examples**:
```bash
export                 # → Lists all exported variables
export MY_VAR=hello    # → Sets MY_VAR to "hello"
export VAR1=1 VAR2=2   # → Sets multiple variables
export 123INVALID=bad  # → Error: not a valid identifier
```

### **5. Unset Command**

#### **What it is**: Removes environment variables.

**The Analogy**: Like erasing information from the notice board.

#### **How it works**:
```c
int builtin_unset(char **argv, t_shell *shell)
{
    if (!argv[1])
        return (0);  // No arguments - do nothing
    
    int error = 0;
    int i = 1;
    while (argv[i]) {
        if (!is_valid_var_name(argv[i])) {
            print_error("unset", "not a valid identifier");
            error = 1;
        } else {
            if (env_unset_var(shell, argv[i]))
                error = 1;
        }
        i++;
    }
    return (error);
}
```

#### **Key Features**:
- **No arguments**: Does nothing (no error)
- **Variable removal**: Removes specified variables
- **Validation**: Checks if variable names are valid
- **Multiple variables**: Can remove multiple variables at once

#### **Examples**:
```bash
unset MY_VAR           # → Removes MY_VAR
unset VAR1 VAR2        # → Removes multiple variables
unset 123INVALID       # → Error: not a valid identifier
```

### **6. ENV Command**

#### **What it is**: Displays all environment variables.

**The Analogy**: Like showing the complete notice board to everyone.

#### **How it works**:
```c
int builtin_env(char **argv, t_shell *shell)
{
    int i = 0;
    while (shell->envp[i]) {
        ft_putendl_fd(shell->envp[i], STDOUT_FILENO);
        i++;
    }
    return (0);
}
```

#### **Key Features**:
- **Simple display**: Shows all environment variables
- **No arguments**: Ignores any arguments
- **Format**: Shows `NAME=value` format

#### **Examples**:
```bash
env                    # → Shows all environment variables
env extra args         # → Still shows all variables (ignores args)
```

### **7. Exit Command**

#### **What it is**: Exits the shell with a specified status code.

**The Analogy**: Like closing the restaurant and turning off the lights.

#### **How it works**:
```c
int builtin_exit(char **argv, t_shell *shell)
{
    int exit_code = 0;
    
    if (!argv[1]) {
        exit_code = 0;  // Default exit code
    } else if (argv[2]) {
        print_error("exit", "too many arguments");
        return (1);
    } else if (!is_valid_exit_code(argv[1])) {
        print_error("exit", "numeric argument required");
        exit(255);
    } else {
        exit_code = ft_atoi(argv[1]);
    }
    
    ft_putendl_fd("exit", STDOUT_FILENO);
    exit(exit_code);
}
```

#### **Key Features**:
- **No argument**: Exits with code 0 (success)
- **Numeric argument**: Exits with specified code
- **Validation**: Checks if exit code is valid number
- **Error handling**: Reports errors for invalid usage

#### **Examples**:
```bash
exit                   # → Exits with code 0
exit 42                # → Exits with code 42
exit abc               # → Error: numeric argument required
exit 1 2               # → Error: too many arguments
```

---

## **External Command Execution**

### **The Analogy: Bringing in Prepared Food**

Think of external commands like **bringing in prepared food** from outside:

#### **PATH Resolution**
```c
char *find_command_path(const char *command, t_shell *shell)
{
    char *path = get_env_var("PATH", shell);
    char **path_dirs = ft_split(path, ':');
    
    for (int i = 0; path_dirs[i]; i++) {
        char *full_path = malloc(ft_strlen(path_dirs[i]) + ft_strlen(command) + 2);
        ft_strlcpy(full_path, path_dirs[i], ft_strlen(path_dirs[i]) + 1);
        ft_strlcat(full_path, "/", ft_strlen(path_dirs[i]) + 2);
        ft_strlcat(full_path, command, ft_strlen(path_dirs[i]) + ft_strlen(command) + 2);
        
        if (access(full_path, X_OK) == 0) {
            ft_strarr_free(path_dirs);
            return (full_path);
        }
        free(full_path);
    }
    ft_strarr_free(path_dirs);
    return (NULL);
}
```

**The Analogy**: Like looking in different stores to find a specific ingredient.

**What it does**: Searches through PATH directories to find the executable.

#### **Process Creation**
```c
int execute_external_in_child(t_cmd *cmd, t_shell *shell)
{
    char *command_path = find_command_path(cmd->argv[0], shell);
    if (!command_path) {
        print_command_error(cmd->argv[0], "command not found");
        return (127);
    }
    
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    execve(command_path, cmd->argv, shell->envp);
    print_error("execve", strerror(errno));
    free(command_path);
    return (126);
}
```

**The Analogy**: Like hiring a specialist to prepare a specific dish.

**What it does**: Creates a new process to run the external command.

---

## **Pipeline Execution: Multi-Course Meals**

### **The Analogy: A Multi-Course Meal**

Think of pipeline execution like **preparing a multi-course meal**:

#### **Pipeline Setup**
```c
int execute_pipeline(t_cmd *cmd_list, t_shell *shell)
{
    t_cmd *current = cmd_list;
    int pipe_fds[2];
    int prev_read_fd = -1;
    
    while (current) {
        if (current->next) {
            pipe(pipe_fds);  // Create pipe for next command
        }
        
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            setup_pipeline(current, pipe_fds, prev_read_fd);
            if (setup_redirections(current->redirs))
                exit(1);
            if (is_builtin(current->argv[0]))
                exit(execute_builtin_in_child(current, shell));
            else
                exit(execute_external_in_child(current, shell));
        } else {
            // Parent process
            cleanup_pipeline(pipe_fds, prev_read_fd);
            if (current->next)
                prev_read_fd = pipe_fds[0];
            current = current->next;
        }
    }
    
    while (wait(NULL) > 0);  // Wait for all children
    return (0);
}
```

**The Analogy**: Like setting up a cooking line where each station passes food to the next.

#### **Command Connection**
```c
int setup_pipeline(t_cmd *cmd, int *pipe_fds, int prev_read_fd)
{
    if (prev_read_fd != -1) {
        dup2(prev_read_fd, STDIN_FILENO);   // Read from previous command
        close(prev_read_fd);
    }
    if (pipe_fds[1] != -1) {
        dup2(pipe_fds[1], STDOUT_FILENO);   // Write to next command
        close(pipe_fds[1]);
    }
    return (0);
}
```

**The Analogy**: Like connecting cooking stations with conveyor belts.

---

## **Redirection Handling: Special Cooking Instructions**

### **The Analogy: Special Cooking Instructions**

Think of redirections like **special cooking instructions**:

#### **Input Redirection (`<`)**
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
```
**The Analogy**: Like "read the recipe from this cookbook instead of asking me".

**What it does**: Takes input from a file instead of keyboard.

#### **Output Redirection (`>`)**
```c
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
**The Analogy**: Like "write the recipe to this notebook instead of telling me".

**What it does**: Sends output to a file instead of screen.

#### **Append Redirection (`>>`)**
```c
int handle_append_redirection(const char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        print_error(file, strerror(errno));
        return (1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return (0);
}
```
**The Analogy**: Like "add this recipe to your collection".

**What it does**: Adds output to the end of a file.

#### **Heredoc (`<<`)**
```c
int handle_heredoc_redirection(const char *delimiter)
{
    int pipe_fds[2];
    char *line;
    
    pipe(pipe_fds);
    while (1) {
        line = readline("> ");
        if (!line)
            break;
        if (ft_strcmp(line, (char *)delimiter) == 0) {
            free(line);
            break;
        }
        ft_putendl_fd(line, pipe_fds[1]);
        free(line);
    }
    close(pipe_fds[1]);
    dup2(pipe_fds[0], STDIN_FILENO);
    close(pipe_fds[0]);
    return (0);
}
```
**The Analogy**: Like "write down this recipe until I say stop".

**What it does**: Takes input from multiple lines until a delimiter.

---

## **Error Handling: Kitchen Emergencies**

### **The Analogy: Handling Kitchen Emergencies**

Think of error handling like **handling kitchen emergencies**:

#### **Command Not Found**
```c
if (!command_path) {
    print_command_error(cmd->argv[0], "command not found");
    return (127);
}
```
**The Analogy**: Like "I don't have that ingredient in my kitchen".

**What it does**: Reports when a command doesn't exist.

#### **Permission Denied**
```c
if (access(command_path, X_OK) != 0) {
    print_command_error(cmd->argv[0], "permission denied");
    return (126);
}
```
**The Analogy**: Like "I'm not allowed to use that equipment".

**What it does**: Reports when a command can't be executed.

#### **File Not Found**
```c
if (open(redir->file, O_RDONLY) == -1) {
    print_error(redir->file, strerror(errno));
    return (1);
}
```
**The Analogy**: Like "that cookbook doesn't exist".

**What it does**: Reports when a redirection file doesn't exist.

---

## **Exit Status Management: Recipe Results**

### **The Analogy: Rating Recipe Results**

Think of exit status like **rating how well a recipe turned out**:

#### **Exit Status Values**
- **0**: Success (like "perfect meal!")
- **1**: General error (like "something went wrong")
- **126**: Command not executable (like "can't use that equipment")
- **127**: Command not found (like "don't have that ingredient")
- **128+**: Signal-related errors (like "emergency stop")

#### **Status Propagation**
```c
int execute_command_list(t_cmd *cmd_list, t_shell *shell)
{
    int status = 0;
    if (current->next)
        status = execute_pipeline(cmd_list, shell);
    else
        status = execute_single_command(current, shell);
    
    set_exit_status(shell, status);
    return (status);
}
```
**The Analogy**: Like remembering how the last dish turned out.

---

## **Why This Design? (Trade-offs and Alternatives)**

### **Why We Chose This Approach**

#### **1. Child Process Model**
**What we did**: All commands (built-in and external) execute in child processes.

**Why this approach**:
- ✅ **Isolation**: External commands can't crash the shell
- ✅ **Consistency**: Same process model for all commands
- ✅ **Signal handling**: Proper signal isolation
- ✅ **Memory safety**: Child process crashes don't affect parent

**Alternative approaches we rejected**:
- ❌ **Built-ins in parent**: Would make shell vulnerable to crashes
- ❌ **Mixed approach**: Inconsistent behavior

#### **2. Simple Built-in Detection**
**What we did**: Use string comparisons to detect built-ins.

**Why this approach**:
- ✅ **Fast**: O(1) detection time
- ✅ **Simple**: Easy to understand and maintain
- ✅ **No memory overhead**: No dynamic allocation
- ✅ **Norminette compliant**: No complex data structures

**Alternative approaches we rejected**:
- ❌ **Linked list registry**: Unnecessary complexity for 7 fixed built-ins
- ❌ **Hash table**: Overkill for small number of built-ins

#### **3. Process-based External Execution**
**What we did**: Use fork/exec for external commands.

**Why this approach**:
- ✅ **Standard**: Unix standard for process creation
- ✅ **Isolation**: External commands can't affect shell
- ✅ **Flexible**: Can run any executable
- ✅ **Signal handling**: Proper signal management

**Alternative approaches we rejected**:
- ❌ **Thread-based**: More complex and less portable
- ❌ **Direct execution**: Would crash the shell on errors

---

## **Common Pitfalls and How We Avoid Them**

### **1. File Descriptor Leaks**
**The Problem**: Not closing file descriptors after use.

**The Analogy**: Like leaving the oven on after cooking.

**How we avoid it**:
```c
// Always close file descriptors
int fd = open(file, O_RDONLY);
// ... use fd ...
close(fd);  // Clean up!
```

### **2. Zombie Processes**
**The Problem**: Not waiting for child processes to finish.

**The Analogy**: Like not cleaning up after hired help.

**How we avoid it**:
```c
// Always wait for child processes
pid_t pid = fork();
if (pid > 0) {
    waitpid(pid, &status, 0);  // Wait for child
}
```

### **3. Signal Handling in Children**
**The Problem**: Child processes inheriting signal handlers.

**The Analogy**: Like hired help using the wrong emergency procedures.

**How we avoid it**:
```c
// Reset signal handlers in child
if (pid == 0) {
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    execve(command, argv, envp);
}
```

---

## **Key Learning Points**

### **1. Process Management is Critical**
**What it is**: Creating and managing child processes for command execution.

**The Analogy**: Like managing a team of cooks in the kitchen.

**Why it matters**: Proper process management prevents crashes and resource leaks.

### **2. Built-ins vs External Commands**
**What it is**: Different ways to handle different types of commands.

**The Analogy**: Like using kitchen equipment vs. bringing in prepared food.

**Why it matters**: Each approach has different benefits and trade-offs.

### **3. Error Handling Makes Shells Robust**
**What it is**: Gracefully handling errors and reporting them clearly.

**The Analogy**: Like having emergency procedures in the kitchen.

**Why it matters**: Users need to know what went wrong and how to fix it.

### **4. Exit Status is Communication**
**What it is**: Commands communicate their success or failure through exit codes.

**The Analogy**: Like a restaurant rating system.

**Why it matters**: Enables automation and error handling in scripts.

### **5. Redirections Modify I/O**
**What it is**: Redirections change where input comes from and output goes.

**The Analogy**: Like using different cooking methods.

**Why it matters**: Provides flexibility in command execution.

### **6. Pipelines Connect Commands**
**What it is**: Pipes create data flow between commands.

**The Analogy**: Like connecting cooking steps in a meal.

**Why it matters**: Enables complex command combinations.

---

## **What's Next?**

Now that we have a **fully functional shell** with command execution, we have completed the core functionality! 

The remaining phases focus on **enhancements and polish**:

- **Phase 6**: Advanced redirections and pipes
- **Phase 7**: Environment management
- **Phase 8**: Built-in command implementations
- **Phase 9**: Error handling and user experience
- **Phase 10**: Memory management and resource cleanup
- **Phase 11**: Testing and quality assurance

**We've built a working shell - now let's make it production-ready!** 🚀

---

*This guide provides a complete understanding of command execution from basic concepts to implementation details, using real-world analogies to make complex topics accessible to beginners.*