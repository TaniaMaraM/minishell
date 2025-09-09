# Phase 3: Parser - A Complete Beginner's Guide

## **What is a Parser? (The Simple Explanation)**

Imagine you have a pile of recipe ingredients and cooking instructions, but they're all mixed up. You need to organize them into a proper recipe that tells you what to do first, what to do second, and how everything fits together.

**A parser** does exactly that - but for computer commands!

### **Real-World Analogy: Organizing a Recipe**
Think of organizing a recipe:
- **Input**: Mixed ingredients and instructions: `["flour", "mix", "2 cups", "bake", "350°F", "eggs"]`
- **Parser's job**: Organize into steps: `"Add 2 cups flour, mix with eggs, then bake at 350°F"`

**In our shell**, it works the same way:
- **Input**: Tokens: `[echo, "hello world", |, grep, hello]`
- **Parser's job**: Build command structure: `echo "hello world" | grep hello`

---

## **Why Do We Need a Parser?**

### **The Problem Without a Parser**
Without a parser, the executor would have to deal with raw tokens:

```bash
# Raw tokens
[TOKEN_WORD: "echo"]
[TOKEN_WORD: "hello world"]
[TOKEN_PIPE: "|"]
[TOKEN_WORD: "grep"]
[TOKEN_WORD: "hello"]
```

**Problems**:
- ❌ **Hard to execute**: Executor has to figure out structure
- ❌ **Error-prone**: Easy to make mistakes with token order
- ❌ **Complex**: Executor becomes very complicated

### **The Solution With a Parser**
With a parser, the executor gets structured commands:

```bash
# Structured commands
Command 1: {
    argv: ["echo", "hello world"]
    redirs: []
    next: Command 2
}
Command 2: {
    argv: ["grep", "hello"]
    redirs: []
    next: NULL
}
Pipeline: Command1 | Command2
```

**Benefits**:
- ✅ **Easy to execute**: Executor knows exactly what to do
- ✅ **Error-resistant**: Parser validates structure
- ✅ **Simple**: Executor can focus on execution, not structure

---

## **How the Parser Works (Step by Step)**

### **The Analogy: A Recipe Organizer**

Think of the parser like a **recipe organizer** that takes ingredients and creates a cooking plan:

#### **Step 1: Initialize the Parser**
```c
t_parser *parser = parser_init(lexer, shell);
```
**The Analogy**: Like setting up your cooking workspace with all the ingredients.

**What it does**: Sets up the parser with the token stream and shell context.

#### **Step 2: Parse the Pipeline**
```c
t_cmd *cmd_list = parser_parse_pipeline(parser);
```
**The Analogy**: Like organizing ingredients into cooking steps.

**What it does**: Builds a list of commands connected by pipes.

#### **Step 3: Parse Each Command**
```c
t_cmd *cmd = parser_parse_command(parser);
```
**The Analogy**: Like organizing ingredients for each dish.

**What it does**: Builds a command structure with arguments and redirections.

#### **Step 4: Parse Arguments and Redirections**
```c
parser_parse_arguments(parser, cmd);
parser_parse_redirections(parser, cmd);
```
**The Analogy**: Like separating ingredients from cooking instructions.

**What it does**: Adds arguments and redirections to the command.

---

## **Command Structure: The Building Blocks**

### **The Analogy: A Recipe Card**

Think of a command structure like a **recipe card**:

```c
typedef struct s_cmd {
    char        **argv;      // Ingredients list
    t_redir     *redirs;     // Cooking instructions
    struct s_cmd *next;      // Next dish in the meal
    struct s_cmd *prev;      // Previous dish in the meal
} t_cmd;
```

#### **argv - The Ingredients List**
**What it is**: Array of strings that become the command and its arguments.

**The Analogy**: Like a list of ingredients: `["flour", "eggs", "sugar"]`

**Example**:
```bash
echo "hello world"
# argv: ["echo", "hello world"]
```

#### **redirs - The Cooking Instructions**
**What it is**: List of redirections that change input/output.

**The Analogy**: Like cooking instructions: "bake at 350°F", "stir for 5 minutes"

**Example**:
```bash
echo "hello" > file.txt
# redirs: [{type: REDIR_OUT, file: "file.txt"}]
```

#### **next/prev - The Meal Sequence**
**What it is**: Links to other commands in a pipeline.

**The Analogy**: Like "first course", "main course", "dessert"

**Example**:
```bash
echo "hello" | grep "h"
# Command 1: echo "hello" (next: Command 2)
# Command 2: grep "h" (prev: Command 1)
```

---

## **Redirection Structure: The Cooking Instructions**

### **The Analogy: Different Types of Cooking Instructions**

Think of redirections like **different types of cooking instructions**:

```c
typedef struct s_redir {
    t_redir_type type;    // What type of instruction
    char        *file;    // What file to use
    int         fd;       // File descriptor
    struct s_redir *next; // Next instruction
} t_redir;
```

#### **REDIR_IN (`<`) - Read From File**
**What it is**: Takes input from a file instead of keyboard.

**The Analogy**: Like "read the recipe from the cookbook"

**Example**:
```bash
grep "hello" < input.txt
# redirs: [{type: REDIR_IN, file: "input.txt"}]
```

#### **REDIR_OUT (`>`) - Write To File**
**What it is**: Sends output to a file instead of screen.

**The Analogy**: Like "write the recipe to a notebook"

**Example**:
```bash
echo "hello" > output.txt
# redirs: [{type: REDIR_OUT, file: "output.txt"}]
```

#### **REDIR_APPEND (`>>`) - Append To File**
**What it is**: Adds output to the end of a file.

**The Analogy**: Like "add this recipe to your collection"

**Example**:
```bash
echo "hello" >> log.txt
# redirs: [{type: REDIR_APPEND, file: "log.txt"}]
```

#### **REDIR_HEREDOC (`<<`) - Here Document**
**What it is**: Takes input from multiple lines until a delimiter.

**The Analogy**: Like "write down this recipe until I say stop"

**Example**:
```bash
cat << EOF
line 1
line 2
EOF
# redirs: [{type: REDIR_HEREDOC, file: "EOF"}]
```

---

## **Pipeline Parsing: Connecting Commands**

### **The Analogy: A Multi-Course Meal**

Think of pipeline parsing like **organizing a multi-course meal**:

#### **Single Command (Appetizer Only)**
```bash
echo "hello"
# One command, no pipeline
```

#### **Pipeline (Full Meal)**
```bash
echo "hello" | grep "h" | wc -l
# Command 1: echo "hello" → Command 2: grep "h" → Command 3: wc -l
```

### **How We Build Pipelines**

#### **Step 1: Parse First Command**
```c
t_cmd *first_cmd = parser_parse_command(parser);
```

#### **Step 2: Check for Pipe**
```c
if (parser->current_token->type == TOKEN_PIPE) {
    parser_advance(parser);  // Skip the pipe token
    // Parse next command
}
```

#### **Step 3: Link Commands**
```c
first_cmd->next = second_cmd;
second_cmd->prev = first_cmd;
```

#### **Step 4: Continue Until No More Pipes**
```c
while (parser->current_token->type == TOKEN_PIPE) {
    // Parse next command and link it
}
```

---

## **Argument Parsing: Building the Ingredient List**

### **The Analogy: Collecting Ingredients**

Think of argument parsing like **collecting ingredients** for a recipe:

#### **The Process**
```c
int parser_parse_arguments(t_parser *parser, t_cmd *cmd)
{
    while (parser->current_token && 
           parser->current_token->type == TOKEN_WORD) {
        cmd_add_arg(cmd, parser->current_token->value);
        parser_advance(parser);
    }
}
```

#### **Step-by-Step**
1. **Check if token is a word**: `TOKEN_WORD`
2. **Add to command**: `cmd_add_arg(cmd, token->value)`
3. **Move to next token**: `parser_advance(parser)`
4. **Repeat until no more words**

### **Example**
```bash
Input: echo "hello world" "goodbye"
Tokens: [TOKEN_WORD: "echo"] [TOKEN_WORD: "hello world"] [TOKEN_WORD: "goodbye"]

Result:
cmd->argv[0] = "echo"
cmd->argv[1] = "hello world"
cmd->argv[2] = "goodbye"
cmd->argv[3] = NULL
```

---

## **Redirection Parsing: Adding Cooking Instructions**

### **The Analogy: Adding Cooking Instructions**

Think of redirection parsing like **adding cooking instructions** to a recipe:

#### **The Process**
```c
int parser_parse_redirections(t_parser *parser, t_cmd *cmd)
{
    while (parser_is_redir_token(parser->current_token->type)) {
        t_redir_type type = parser_token_to_redir_type(parser->current_token->type);
        parser_advance(parser);  // Skip redirection token
        
        if (parser->current_token->type == TOKEN_WORD) {
            cmd_add_redir(cmd, type, parser->current_token->value);
            parser_advance(parser);  // Skip filename token
        }
    }
}
```

#### **Step-by-Step**
1. **Check if token is redirection**: `TOKEN_REDIR_IN`, `TOKEN_REDIR_OUT`, etc.
2. **Get redirection type**: Convert token to redirection type
3. **Skip redirection token**: `parser_advance(parser)`
4. **Get filename**: Next token should be `TOKEN_WORD`
5. **Add redirection**: `cmd_add_redir(cmd, type, filename)`
6. **Skip filename token**: `parser_advance(parser)`

### **Example**
```bash
Input: echo "hello" > output.txt
Tokens: [TOKEN_WORD: "echo"] [TOKEN_WORD: "hello"] [TOKEN_REDIR_OUT: ">"] [TOKEN_WORD: "output.txt"]

Result:
cmd->argv[0] = "echo"
cmd->argv[1] = "hello"
cmd->argv[2] = NULL
cmd->redirs = {type: REDIR_OUT, file: "output.txt"}
```

---

## **Error Handling: Validating the Recipe**

### **The Analogy: Checking Recipe Validity**

Think of error handling like **checking if a recipe makes sense**:

#### **Common Errors**

##### **1. Missing Command**
```bash
Input: | grep hello
Error: No command before pipe
```

##### **2. Missing Filename**
```bash
Input: echo hello >
Error: No filename after redirection
```

##### **3. Unexpected Token**
```bash
Input: echo hello | | grep world
Error: Unexpected pipe token
```

#### **How We Handle Errors**
```c
int parser_expect(t_parser *parser, t_token_type expected)
{
    if (!parser->current_token || 
        parser->current_token->type != expected) {
        parser->error = 1;
        return (0);
    }
    return (1);
}
```

**The Analogy**: Like checking "do I have the right ingredient for this step?"

---

## **Memory Management: Organizing the Kitchen**

### **The Analogy: Organizing Cooking Supplies**

Think of memory management like **organizing cooking supplies**:

#### **Allocation (Getting Supplies)**
```c
t_cmd *cmd = cmd_alloc();
t_redir *redir = redir_alloc(type, filename);
```

**The Analogy**: Like getting ingredients from the pantry.

#### **Cleanup (Putting Supplies Away)**
```c
void cmd_destroy(t_cmd *cmd)
{
    if (cmd->argv) ft_strarr_free(cmd->argv);
    if (cmd->redirs) redir_destroy_list(cmd->redirs);
    free(cmd);
}
```

**The Analogy**: Like putting ingredients back in the pantry after cooking.

#### **Centralized Management**
```c
// All allocation in one place
t_parser *parser_alloc(void);
t_cmd *cmd_alloc(void);
t_redir *redir_alloc(t_redir_type type, const char *file);
```

**The Analogy**: Like having a central pantry manager.

---

## **Why This Design? (Trade-offs and Alternatives)**

### **Why We Chose This Approach**

#### **1. Command-Based Structure**
**What we did**: Create command structures that represent executable units.

**Why this approach**:
- ✅ **Natural**: Commands are the natural unit of execution
- ✅ **Flexible**: Easy to add new command types
- ✅ **Efficient**: Executor can work with complete commands

**Alternative approaches we rejected**:
- ❌ **Token-based execution**: Too low-level and complex
- ❌ **String-based execution**: Doesn't handle structure

#### **2. Linked List for Pipelines**
**What we did**: Use linked lists to connect commands in pipelines.

**Why this approach**:
- ✅ **Simple**: Easy to traverse and modify
- ✅ **Efficient**: No need to know pipeline length in advance
- ✅ **Flexible**: Easy to add/remove commands

**Alternative approaches we rejected**:
- ❌ **Array-based**: Need to know length in advance
- ❌ **Tree-based**: Over-complicated for linear pipelines

#### **3. Separate Redirection Lists**
**What we did**: Keep redirections separate from arguments.

**Why this approach**:
- ✅ **Clear separation**: Arguments vs. redirections are different
- ✅ **Easy processing**: Executor can handle them separately
- ✅ **Flexible**: Multiple redirections per command

**Alternative approaches we rejected**:
- ❌ **Mixed with arguments**: Confusing and error-prone
- ❌ **Single redirection**: Too limiting

---

## **Common Pitfalls and How We Avoid Them**

### **1. Memory Leaks in Command Lists**
**The Problem**: Forgetting to free command lists.

**The Analogy**: Like not cleaning up after cooking.

**How we avoid it**:
```c
// Always destroy command lists
t_cmd *cmd_list = parser_parse(parser);
// ... use cmd_list ...
cmd_destroy_list(cmd_list);  // Clean up!
```

### **2. Incorrect Token Consumption**
**The Problem**: Not advancing the parser correctly.

**The Analogy**: Like skipping steps in a recipe.

**How we avoid it**:
```c
// Always advance after consuming tokens
parser_advance(parser);  // Move to next token
```

### **3. Redirection Token Confusion**
**The Problem**: Not handling redirection tokens correctly.

**The Analogy**: Like misreading cooking instructions.

**How we avoid it**:
```c
// Check token type before processing
if (parser_is_redir_token(parser->current_token->type)) {
    // Handle redirection
}
```

---

## **Testing Our Parser**

### **The Analogy: Testing a New Recipe Organizer**

Just like you test a new recipe organizer, we test our parser.

### **Test Categories**

#### **1. Basic Command Parsing**
```bash
Input: "echo hello"
Expected: Command{argv: ["echo", "hello"], redirs: [], next: NULL}
```
**What it tests**: Can we parse simple commands?
**Expected result**: Correct command structure

#### **2. Pipeline Parsing**
```bash
Input: "echo hello | grep h"
Expected: Command1{argv: ["echo", "hello"]} -> Command2{argv: ["grep", "h"]}
```
**What it tests**: Can we parse pipelines?
**Expected result**: Commands linked correctly

#### **3. Redirection Parsing**
```bash
Input: "echo hello > file.txt"
Expected: Command{argv: ["echo", "hello"], redirs: [{type: REDIR_OUT, file: "file.txt"}]}
```
**What it tests**: Can we parse redirections?
**Expected result**: Redirections added correctly

#### **4. Error Handling**
```bash
Input: "echo |"
Expected: Error (missing command after pipe)
```
**What it tests**: Do we catch syntax errors?
**Expected result**: Parser sets error flag

---

## **Key Learning Points**

### **1. Parsing is Structure Building**
**What it is**: Taking tokens and building meaningful structures.

**The Analogy**: Like taking ingredients and creating a cooking plan.

**Why it matters**: Makes execution much easier and more reliable.

### **2. Commands Are the Natural Unit**
**What it is**: Commands represent what the user wants to execute.

**The Analogy**: Like individual dishes in a meal.

**Why it matters**: Commands are what the executor actually runs.

### **3. Pipelines Connect Commands**
**What it is**: Pipes create data flow between commands.

**The Analogy**: Like connecting cooking steps in a meal.

**Why it matters**: Enables complex command combinations.

### **4. Redirections Modify I/O**
**What it is**: Redirections change where input comes from and output goes.

**The Analogy**: Like using different cooking methods.

**Why it matters**: Provides flexibility in command execution.

---

## **What's Next?**

Now that we can build command structures from tokens, we're ready for **Phase 4: Expander**!

In Phase 4, we'll learn how to:
- **Expand variables in command arguments**
- **Handle different quote contexts**
- **Resolve environment variables**
- **Process exit status variables**

**We've organized the ingredients - now let's prepare them for cooking!** 🚀

---

*This guide provides a complete understanding of parsing from basic concepts to implementation details, using real-world analogies to make complex topics accessible to beginners.*
