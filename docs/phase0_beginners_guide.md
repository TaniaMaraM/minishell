# Phase 0: Project Bootstrap - A Complete Beginner's Guide

## **What is Project Bootstrap? (The Simple Explanation)**

Imagine you want to build a house. Before you can start building, you need to:
1. **Get the land** (create directories)
2. **Get building permits** (set up build system)
3. **Get tools and materials** (install libraries)
4. **Create a blueprint** (write basic code structure)

**Project bootstrap** is exactly like that - but for software projects!

### **Real-World Analogy: Starting a Restaurant**
Think of opening a restaurant:
- **Location**: You need a building (project directory)
- **Kitchen equipment**: You need stoves, ovens, etc. (libraries)
- **Recipes**: You need cooking instructions (source code)
- **Staff training**: You need to teach your team (documentation)
- **Opening day**: You need a basic menu (minimal working program)

**In our minishell project**, it works the same way:
- **Directory structure**: Organize our code files
- **Makefile**: Build system to compile our program
- **Libraries**: External tools we need (libft, readline)
- **Basic main.c**: A simple program that runs and exits

---

## **Why Do We Need Project Bootstrap?**

### **The Problem Without Structure**
Without proper bootstrap, you'd have a messy project:

```
my_project/
├── code1.c
├── code2.c
├── some_library.c
├── test.c
├── random_file.txt
└── backup_code.c.bak
```

**Problems**:
- ❌ **Hard to find files**
- ❌ **No clear organization**
- ❌ **Difficult to build**
- ❌ **Hard to maintain**

### **The Solution With Bootstrap**
With proper bootstrap, you have a clean structure:

```
minishell/
├── src/           # Source code
├── include/       # Header files
├── lib/          # External libraries
├── tests/        # Test files
├── docs/         # Documentation
└── Makefile      # Build system
```

**Benefits**:
- ✅ **Easy to navigate**
- ✅ **Clear organization**
- ✅ **Simple to build**
- ✅ **Easy to maintain**

---

## **The Project Structure (Step by Step)**

### **The Analogy: Organizing a Library**

Think of our project structure like organizing a library:

#### **`src/` - The Main Collection**
**What it is**: Where all our main code lives.

**The Analogy**: Like the main book collection in a library.

**Contains**:
- `app/` - Main application code (like the front desk)
- `lexeme/` - Code that breaks down input (like the catalog system)
- `parser/` - Code that understands commands (like the librarian)
- `expand/` - Code that handles variables (like the reference desk)
- `exec/` - Code that runs commands (like the book delivery system)

#### **`include/` - The Index**
**What it is**: Header files that describe what functions do.

**The Analogy**: Like the library's card catalog - it tells you what's available.

**Contains**:
- `minishell.h` - Main header (like the master catalog)
- `tokens.h` - Token definitions (like the subject index)
- `cmd.h` - Command structures (like the author index)

#### **`lib/` - External Resources**
**What it is**: Libraries written by other people that we use.

**The Analogy**: Like borrowing books from other libraries.

**Contains**:
- `libft/` - Basic C functions (like a dictionary)
- `get_next_line/` - Line reading functions (like a reading guide)

#### **`tests/` - Quality Control**
**What it is**: Code that tests if our program works correctly.

**The Analogy**: Like having someone check if books are in the right place.

#### **`docs/` - Documentation**
**What it is**: Explanations of how everything works.

**The Analogy**: Like the library's user manual.

---

## **The Makefile: Our Build System**

### **The Analogy: A Recipe Book**

Think of a Makefile like a **recipe book** for building our program:

#### **Basic Recipe Structure**
```makefile
# What we're making
NAME = minishell

# What tools we need
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# What ingredients we need
SRC_FILES = $(shell find src/ -name '*.c')
OBJS = $(SRC_FILES:src/%.c=obj/%.o)

# How to make it
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
```

### **Why We Use Makefiles**

#### **The Problem Without Makefiles**
Without a build system, you'd have to type:
```bash
cc -Wall -Wextra -Werror -g src/app/main.c src/app/init.c src/app/loop.c src/lexeme/lexer.c src/parser/parser.c -lreadline -o minishell
```

**Problems**:
- ❌ **Very long commands**
- ❌ **Easy to make mistakes**
- ❌ **Hard to remember**
- ❌ **No dependency management**

#### **The Solution With Makefiles**
With a Makefile, you just type:
```bash
make
```

**Benefits**:
- ✅ **Simple command**
- ✅ **No mistakes**
- ✅ **Easy to remember**
- ✅ **Automatic dependency management**

---

## **Libraries: External Tools We Use**

### **The Analogy: Using Professional Tools**

Think of libraries like using **professional tools** instead of making everything yourself:

#### **libft - Our Swiss Army Knife**
**What it is**: Basic C functions that make programming easier.

**The Analogy**: Like having a Swiss Army knife with all the basic tools.

**What it provides**:
- `ft_strlen()` - Count characters in a string
- `ft_strdup()` - Copy a string
- `ft_malloc()` - Allocate memory
- `ft_putstr()` - Print strings

**Why we use it**:
- ✅ **Standard functions** - Everyone knows them
- ✅ **Well-tested** - Reliable and bug-free
- ✅ **42-approved** - Meets school standards

#### **readline - Our Input Helper**
**What it is**: Library that handles user input with history and editing.

**The Analogy**: Like having a smart assistant that remembers what you typed before.

**What it provides**:
- `readline()` - Read user input
- `add_history()` - Remember previous commands
- `rl_clear_history()` - Clear command history

**Why we use it**:
- ✅ **Professional input handling**
- ✅ **Command history**
- ✅ **Line editing**
- ✅ **Required by the project**

---

## **The Basic Main Function**

### **The Analogy: The Front Door of a Building**

Think of `main()` like the **front door** of a building - it's where everything starts.

### **Our Basic Main Function**
```c
int main(int argc, char **argv, char **envp)
{
    t_shell shell;           // Create our shell structure
    
    shell_init(&shell, envp); // Set up the shell
    shell_loop(&shell);      // Start the main loop
    shell_cleanup(&shell);   // Clean up when done
    
    return (0);              // Exit successfully
}
```

### **Step-by-Step Explanation**

#### **Step 1: Create Shell Structure**
```c
t_shell shell;
```
**The Analogy**: Like getting a notebook to keep track of everything.

**What it does**: Creates a structure to hold all our shell's information.

#### **Step 2: Initialize the Shell**
```c
shell_init(&shell, envp);
```
**The Analogy**: Like setting up your workspace before starting work.

**What it does**: 
- Sets up the prompt
- Copies environment variables
- Prepares signal handling

#### **Step 3: Start the Main Loop**
```c
shell_loop(&shell);
```
**The Analogy**: Like starting the main service at a restaurant.

**What it does**: 
- Shows the prompt
- Reads user input
- Processes commands
- Repeats until user exits

#### **Step 4: Clean Up**
```c
shell_cleanup(&shell);
```
**The Analogy**: Like cleaning up after closing the restaurant.

**What it does**: 
- Frees allocated memory
- Closes files
- Resets terminal settings

---

## **Why This Design? (Trade-offs and Alternatives)**

### **Why We Chose This Structure**

#### **1. Modular Directory Organization**
**What we did**: Separate directories for different types of code.

**Why this approach**:
- ✅ **Clear separation**: Easy to find what you're looking for
- ✅ **Scalable**: Easy to add new features
- ✅ **Maintainable**: Changes in one area don't affect others

**Alternative approaches we rejected**:
- ❌ **Single directory**: Everything mixed together (messy)
- ❌ **Too many directories**: Over-complicated structure

#### **2. Makefile Build System**
**What we did**: Use Make to manage compilation.

**Why this approach**:
- ✅ **Standard**: Everyone knows how to use Make
- ✅ **Efficient**: Only recompiles what changed
- ✅ **Flexible**: Easy to add new build rules

**Alternative approaches we rejected**:
- ❌ **Manual compilation**: Too error-prone
- ❌ **Complex build systems**: Overkill for this project

#### **3. External Libraries**
**What we did**: Use libft and readline libraries.

**Why this approach**:
- ✅ **Reliability**: Well-tested code
- ✅ **Efficiency**: Don't reinvent the wheel
- ✅ **Standards**: Follows 42 requirements

**Alternative approaches we rejected**:
- ❌ **Write everything from scratch**: Too much work
- ❌ **Use too many libraries**: Over-complicated

---

## **Common Pitfalls and How We Avoid Them**

### **1. Directory Structure Confusion**
**The Problem**: Not knowing where to put files.

**The Analogy**: Like not knowing where to put things in your house.

**How we avoid it**:
- **Clear naming**: Directory names explain their purpose
- **Consistent structure**: Same pattern throughout
- **Documentation**: README files explain the structure

### **2. Build System Errors**
**The Problem**: Makefile not working correctly.

**The Analogy**: Like a recipe that doesn't work.

**How we avoid it**:
- **Simple rules**: Keep Makefile straightforward
- **Test frequently**: Build often to catch errors
- **Clear dependencies**: Make sure files depend on the right things

### **3. Library Integration Issues**
**The Problem**: Libraries not linking correctly.

**The Analogy**: Like trying to use a tool that doesn't fit.

**How we avoid it**:
- **Proper includes**: Include headers correctly
- **Correct linking**: Link libraries in the right order
- **Test compilation**: Make sure everything compiles

---

## **Testing Our Bootstrap**

### **The Analogy: Testing a New Car**

Just like you test a new car before driving it, we test our bootstrap.

### **Test Steps**

#### **1. Compilation Test**
```bash
make
```
**What it tests**: Can we build the program?
**Expected result**: Program compiles without errors

#### **2. Basic Execution Test**
```bash
./minishell
```
**What it tests**: Does the program start?
**Expected result**: Shows a prompt and accepts input

#### **3. Exit Test**
```bash
./minishell
# Type Ctrl+D or "exit"
```
**What it tests**: Can we exit cleanly?
**Expected result**: Program exits without errors

#### **4. Memory Test**
```bash
valgrind ./minishell
```
**What it tests**: Are there memory leaks?
**Expected result**: No memory leaks reported

---

## **Key Learning Points**

### **1. Organization Matters**
**What it is**: Having a clear structure for your project.

**The Analogy**: Like organizing your room - you can find things easily.

**Why it matters**: Good organization makes development faster and less error-prone.

### **2. Build Systems Are Essential**
**What it is**: Automated tools that compile your code.

**The Analogy**: Like having a machine that assembles your product.

**Why it matters**: Manual compilation is error-prone and time-consuming.

### **3. Libraries Save Time**
**What it is**: Using code written by others.

**The Analogy**: Like using professional tools instead of making your own.

**Why it matters**: Don't reinvent the wheel - use proven solutions.

### **4. Testing Early and Often**
**What it is**: Checking that your code works as expected.

**The Analogy**: Like testing a recipe before serving it to guests.

**Why it matters**: Catching problems early saves time and prevents bigger issues.

---

## **What's Next?**

Now that we have our project structure set up, we're ready for **Phase 1: REPL and Signal Handling**!

In Phase 1, we'll learn how to:
- **Create an interactive shell loop**
- **Handle user input with readline**
- **Manage command history**
- **Handle system signals (Ctrl+C, Ctrl+D)**

**The foundation is set - let's build the shell!** 🚀

---

*This guide provides a complete understanding of project bootstrap from basic concepts to implementation details, using real-world analogies to make complex topics accessible to beginners.*
