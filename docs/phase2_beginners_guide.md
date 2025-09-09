# Phase 2: Lexer - A Complete Beginner's Guide

## **What is a Lexer? (The Simple Explanation)**

Imagine you're reading a recipe, but instead of seeing the whole recipe at once, you want to break it down into individual ingredients and cooking steps. You'd go through the recipe word by word, identifying each ingredient and instruction.

**A lexer** does exactly that - but for computer commands!

### **Real-World Analogy: Reading a Recipe**
Think of reading a recipe:
- **Input**: `"Add 2 cups flour, mix well, then bake at 350°F"`
- **Lexer's job**: Break into ingredients: `["Add", "2", "cups", "flour", ",", "mix", "well", ",", "then", "bake", "at", "350°F"]`

**In our shell**, it works the same way:
- **Input**: `echo "hello world" | grep hello`
- **Lexer's job**: Break into tokens: `[echo, "hello world", |, grep, hello]`

---

## **Why Do We Need a Lexer?**

### **The Problem Without a Lexer**
Without a lexer, the parser would have to deal with raw text:

```bash
# Raw input string
"echo \"hello world\" | grep hello"
```

**Problems**:
- ❌ **Hard to parse**: Parser has to handle every character
- ❌ **Error-prone**: Easy to make mistakes with quotes and spaces
- ❌ **Complex**: Parser becomes very complicated

### **The Solution With a Lexer**
With a lexer, the parser gets clean tokens:

```bash
# Clean tokens
[TOKEN_WORD: "echo"]
[TOKEN_WORD: "hello world"]  # Quotes already handled
[TOKEN_PIPE: "|"]
[TOKEN_WORD: "grep"]
[TOKEN_WORD: "hello"]
```

**Benefits**:
- ✅ **Easy to parse**: Parser works with clean tokens
- ✅ **Error-resistant**: Lexer handles the tricky parts
- ✅ **Simple**: Parser can focus on structure, not characters

---

## **How the Lexer Works (Step by Step)**

### **The Analogy: A Smart Text Reader**

Think of the lexer like a **smart text reader** that understands different types of words:

#### **Step 1: Initialize the Lexer**
```c
t_lexer *lexer = lexer_init("echo hello world");
```
**The Analogy**: Like opening a book and placing a bookmark at the beginning.

**What it does**: Sets up the lexer with the input string and starting position.

#### **Step 2: Read Character by Character**
```c
while (lexer->pos < lexer->len) {
    char c = lexer->input[lexer->pos];
    // Process the character
}
```
**The Analogy**: Like reading the book word by word.

**What it does**: Goes through each character in the input string.

#### **Step 3: Identify Token Type**
```c
if (c == '|') {
    return create_token(TOKEN_PIPE, "|", 1);
} else if (c == '"') {
    return lexer_read_quoted(lexer, '"');
} else if (!is_whitespace(c)) {
    return lexer_read_word(lexer);
}
```
**The Analogy**: Like recognizing different types of words (nouns, verbs, punctuation).

**What it does**: Determines what type of token this character represents.

#### **Step 4: Create Token**
```c
t_token *token = create_token(type, value, length);
```
**The Analogy**: Like writing down the word and its type on a card.

**What it does**: Creates a token with the type, value, and length.

---

## **Types of Tokens We Handle**

### **The Analogy: Different Types of Words**

Think of tokens like **different types of words** in a sentence:

#### **TOKEN_WORD - Regular Words**
**What it is**: Normal text that becomes command arguments.

**The Analogy**: Like nouns and verbs in a sentence.

**Examples**:
- `echo` → `[TOKEN_WORD: "echo"]`
- `hello` → `[TOKEN_WORD: "hello"]`
- `world` → `[TOKEN_WORD: "world"]`

#### **TOKEN_PIPE - The Connector**
**What it is**: The `|` symbol that connects commands.

**The Analogy**: Like the word "and" that connects sentences.

**Example**:
- `|` → `[TOKEN_PIPE: "|"]`

#### **TOKEN_REDIR_IN - Input Redirection**
**What it is**: The `<` symbol that redirects input.

**The Analogy**: Like "from" in "read from file".

**Example**:
- `<` → `[TOKEN_REDIR_IN: "<"]`

#### **TOKEN_REDIR_OUT - Output Redirection**
**What it is**: The `>` symbol that redirects output.

**The Analogy**: Like "to" in "write to file".

**Example**:
- `>` → `[TOKEN_REDIR_OUT: ">"]`

#### **TOKEN_EOF - End of Input**
**What it is**: Marks the end of the input.

**The Analogy**: Like a period at the end of a sentence.

**Example**:
- End of string → `[TOKEN_EOF: ""]`

---

## **Quote Handling: The Tricky Part**

### **The Analogy: Different Types of Protection**

Think of quotes like **different types of protective barriers**:

#### **Single Quotes (`'`) - Bulletproof Glass**
**What it is**: Nothing gets interpreted inside single quotes.

**The Analogy**: Like bulletproof glass - nothing gets through.

**Example**:
- `'hello $USER'` → `[TOKEN_WORD: "hello $USER"]` (literal text)

#### **Double Quotes (`"`) - Screen Door**
**What it is**: Some things get interpreted inside double quotes.

**The Analogy**: Like a screen door - some things get through.

**Example**:
- `"hello $USER"` → `[TOKEN_WORD: "hello $USER"]` (quotes removed, but $USER preserved for later expansion)

### **How We Handle Quotes**

#### **The State Machine Approach**
**The Analogy**: Like a security guard who changes behavior based on which area they're in.

```c
typedef enum e_quote_state {
    QUOTE_NONE,     // No quotes active
    QUOTE_SINGLE,   // Inside single quotes
    QUOTE_DOUBLE    // Inside double quotes
} t_quote_state;
```

#### **Quote Processing**
```c
if (c == '\'') {
    if (current_state == QUOTE_SINGLE) {
        return QUOTE_NONE;  // End of single quotes
    } else if (current_state == QUOTE_NONE) {
        return QUOTE_SINGLE;  // Start of single quotes
    }
} else if (c == '"') {
    if (current_state == QUOTE_DOUBLE) {
        return QUOTE_NONE;  // End of double quotes
    } else if (current_state == QUOTE_NONE) {
        return QUOTE_DOUBLE;  // Start of double quotes
    }
}
```

---

## **The Lexer State Machine**

### **The Analogy: A Smart Reader with Different Modes**

Think of the lexer state machine like a **smart reader** that changes behavior based on context:

#### **State 1: Normal Reading Mode**
**The Analogy**: Like reading normal text.

**What it does**:
- Recognizes spaces as separators
- Recognizes special characters (`|`, `<`, `>`)
- Recognizes quotes as state changers

#### **State 2: Inside Single Quotes**
**The Analogy**: Like reading text that must be taken literally.

**What it does**:
- Treats everything as literal text
- Only looks for the closing single quote
- Ignores special characters

#### **State 3: Inside Double Quotes**
**The Analogy**: Like reading text that might have special meaning.

**What it does**:
- Treats most things as literal text
- Preserves `$` for variable expansion
- Looks for the closing double quote

### **State Transitions**
```c
// Start in normal mode
t_quote_state state = QUOTE_NONE;

// Process each character
for (each character) {
    if (character == '\'') {
        state = update_quote_state(state, '\'');
    } else if (character == '"') {
        state = update_quote_state(state, '"');
    } else {
        // Process character based on current state
        process_character(character, state);
    }
}
```

---

## **Token Creation and Management**

### **The Analogy: Creating Recipe Cards**

Think of token creation like **creating recipe cards**:

#### **Token Structure**
```c
typedef struct s_token {
    t_token_type type;    // What kind of token (ingredient, instruction, etc.)
    char        *value;   // The actual text
    size_t      length;   // How long the text is
} t_token;
```

#### **Creating a Token**
```c
t_token *create_token(t_token_type type, const char *value, size_t length)
{
    t_token *token = malloc(sizeof(t_token));
    token->type = type;
    token->value = ft_strdup(value);
    token->length = length;
    return token;
}
```

**The Analogy**: Like writing down an ingredient on a card with its type and amount.

#### **Destroying a Token**
```c
void token_destroy(t_token *token)
{
    if (token) {
        free(token->value);
        free(token);
    }
}
```

**The Analogy**: Like throwing away the recipe card when you're done.

---

## **Why This Design? (Trade-offs and Alternatives)**

### **Why We Chose This Approach**

#### **1. State Machine for Quote Handling**
**What we did**: Use a state machine to track quote context.

**Why this approach**:
- ✅ **Accurate**: Handles complex quote scenarios correctly
- ✅ **Efficient**: Single pass through the input
- ✅ **Reliable**: No ambiguity about quote context

**Alternative approaches we rejected**:
- ❌ **Regex parsing**: Too complex for nested quotes
- ❌ **Multiple passes**: Inefficient and error-prone
- ❌ **Simple string replacement**: Doesn't handle quote context

#### **2. Token-Based Architecture**
**What we did**: Create tokens that represent different types of input.

**Why this approach**:
- ✅ **Clear separation**: Lexer handles characters, parser handles structure
- ✅ **Reusable**: Tokens can be used by different parsers
- ✅ **Testable**: Easy to test tokenization separately

**Alternative approaches we rejected**:
- ❌ **Character-by-character parsing**: Too complex for the parser
- ❌ **String-based parsing**: Doesn't handle quotes correctly

#### **3. Centralized Token Management**
**What we did**: Use functions to create and destroy tokens.

**Why this approach**:
- ✅ **Consistent**: Same pattern for all tokens
- ✅ **Safe**: Proper memory management
- ✅ **Maintainable**: Changes in one place

**Alternative approaches we rejected**:
- ❌ **Manual token creation**: Error-prone and inconsistent
- ❌ **No token management**: Memory leaks and errors

---

## **Common Pitfalls and How We Avoid Them**

### **1. Quote Context Errors**
**The Problem**: Not tracking quote state correctly.

**The Analogy**: Like a security guard who forgets which area they're in.

**How we avoid it**:
```c
// Always track quote state
t_quote_state state = QUOTE_NONE;
state = update_quote_state(state, current_char);
```

### **2. Memory Leaks in Tokens**
**The Problem**: Forgetting to free token memory.

**The Analogy**: Like not cleaning up recipe cards after cooking.

**How we avoid it**:
```c
// Always destroy tokens when done
t_token *token = create_token(type, value, length);
// ... use token ...
token_destroy(token);  // Clean up!
```

### **3. Buffer Overflow in Token Values**
**The Problem**: Not allocating enough memory for token values.

**The Analogy**: Like trying to fit a large ingredient in a small container.

**How we avoid it**:
```c
// Always allocate enough memory
char *value = malloc(length + 1);  // +1 for null terminator
ft_memcpy(value, input + start, length);
value[length] = '\0';
```

---

## **Testing Our Lexer**

### **The Analogy: Testing a New Recipe Reader**

Just like you test a new recipe reader, we test our lexer.

### **Test Categories**

#### **1. Basic Tokenization**
```bash
Input: "echo hello"
Expected: [TOKEN_WORD: "echo"] [TOKEN_WORD: "hello"] [TOKEN_EOF]
```
**What it tests**: Can we break simple input into tokens?
**Expected result**: Correct tokens created

#### **2. Quote Handling**
```bash
Input: "echo 'hello world'"
Expected: [TOKEN_WORD: "echo"] [TOKEN_WORD: "hello world"] [TOKEN_EOF]
```
**What it tests**: Do we handle quotes correctly?
**Expected result**: Quotes removed, content preserved

#### **3. Special Characters**
```bash
Input: "echo hello | grep world"
Expected: [TOKEN_WORD: "echo"] [TOKEN_WORD: "hello"] [TOKEN_PIPE: "|"] [TOKEN_WORD: "grep"] [TOKEN_WORD: "world"] [TOKEN_EOF]
```
**What it tests**: Do we recognize special characters?
**Expected result**: Pipe token created correctly

#### **4. Edge Cases**
```bash
Input: "echo ''"
Expected: [TOKEN_WORD: "echo"] [TOKEN_WORD: ""] [TOKEN_EOF]
```
**What it tests**: Do we handle empty quotes?
**Expected result**: Empty string token created

---

## **Key Learning Points**

### **1. Lexing is the First Step**
**What it is**: Breaking down input into manageable pieces.

**The Analogy**: Like breaking down a complex recipe into individual steps.

**Why it matters**: Makes the next step (parsing) much easier.

### **2. State Machines Handle Complexity**
**What it is**: Changing behavior based on context.

**The Analogy**: Like a smart reader who changes how they read based on what they're reading.

**Why it matters**: Handles complex scenarios like nested quotes correctly.

### **3. Tokens Are the Interface**
**What it is**: Clean data structures that represent different types of input.

**The Analogy**: Like standardized recipe cards that everyone can understand.

**Why it matters**: Provides a clean interface between lexer and parser.

### **4. Memory Management is Critical**
**What it is**: Properly allocating and freeing memory for tokens.

**The Analogy**: Like properly storing and cleaning up recipe ingredients.

**Why it matters**: Prevents memory leaks and crashes.

---

## **What's Next?**

Now that we can break down user input into tokens, we're ready for **Phase 3: Parser**!

In Phase 3, we'll learn how to:
- **Take tokens and build command structures**
- **Handle pipelines and redirections**
- **Create a parse tree for execution**
- **Validate command syntax**

**We've broken down the ingredients - now let's cook the meal!** 🚀

---

*This guide provides a complete understanding of lexing from basic concepts to implementation details, using real-world analogies to make complex topics accessible to beginners.*
