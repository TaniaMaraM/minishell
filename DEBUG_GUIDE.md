# LLDB Debugging Guide for Minishell Memory Analysis

## Quick Start - Automated Debugging

```bash
./run_debug.sh
```

This will run an automated session that traces "echo hello" execution and memory management.

## Manual Step-by-Step Debugging

### 1. Start LLDB Session

```bash
lldb ./minishell
```

### 2. Set Strategic Breakpoints

```lldb
# Main execution flow
(lldb) b main
(lldb) b shell_init
(lldb) b shell_loop
(lldb) b process_line
(lldb) b builtin_echo
(lldb) b shell_cleanup

# Memory management
(lldb) b malloc
(lldb) b free
(lldb) b ft_strdup
(lldb) b ft_strarr_free

# Lexer/Parser lifecycle
(lldb) b lexer_init
(lldb) b parser_init
(lldb) b lexer_destroy
(lldb) b parser_destroy
(lldb) b cmd_destroy_list
```

### 3. Launch and Start Debugging

```lldb
(lldb) run
```

### 4. Key Inspection Commands

#### At main():
```lldb
(lldb) frame variable argc argv envp
(lldb) po argv[0]
(lldb) po envp[0]
```

#### At shell_init():
```lldb
(lldb) frame variable shell
(lldb) po shell
(lldb) po shell->envp
(lldb) po shell->last_status
```

#### At process_line():
```lldb
(lldb) po input
(lldb) frame variable sh
(lldb) po sh->last_status
(lldb) po sh->envp
```

#### At malloc/free calls:
```lldb
# For malloc
(lldb) po (size_t)$arg1
(lldb) bt 5

# For free  
(lldb) expr -f hex -- $arg1
(lldb) bt 5
```

#### At builtin_echo():
```lldb
(lldb) po argv[0]
(lldb) po argv[1]
(lldb) frame variable shell
```

### 5. Memory Pointer Verification Commands

#### Check if pointer is valid:
```lldb
(lldb) expr (void*)pointer_name
(lldb) memory read pointer_name
(lldb) po *pointer_name
```

#### Check allocated memory size:
```lldb
(lldb) expr malloc_size(pointer_name)
```

#### Inspect memory content:
```lldb
(lldb) memory read --size 4 --format x pointer_name
(lldb) x/20xg pointer_name
```

### 6. Step-by-Step Execution

```lldb
(lldb) n          # Next line
(lldb) s          # Step into
(lldb) c          # Continue
(lldb) finish     # Step out of current function
```

### 7. Specific Memory Leak Detection

#### Before shell_cleanup():
```lldb
(lldb) expr malloc_zone_statistics(NULL, &stats)
(lldb) frame variable
(lldb) po shell->envp
```

#### Check if all dynamically allocated memory was freed:
```lldb
# Set breakpoint before exit
(lldb) b exit
(lldb) c
# At this point, verify all your pointers are NULL or freed
(lldb) po shell->envp
```

### 8. Advanced Pointer Analysis

#### Track specific allocations:
```lldb
# Set conditional breakpoint
(lldb) br set -n malloc -c '$arg1 == 64'

# Watch memory address
(lldb) watchpoint set variable pointer_name
```

#### Check for double-free errors:
```lldb
(lldb) br set -n free
# Add condition to check if pointer was already freed
```

## Common Issues to Look For

1. **Memory Leaks**: Memory allocated but never freed
2. **Double Free**: Freeing the same memory twice  
3. **Use After Free**: Accessing freed memory
4. **Null Pointer Dereference**: Accessing null pointers
5. **Uninitialized Pointers**: Using uninitialized pointer values

## Minishell Specific Checks

### Environment Variables:
```lldb
(lldb) po shell->envp[0]
(lldb) po shell->envp[1] 
# Check if envp is properly freed in cleanup
```

### Command Processing:
```lldb
# Check if command structures are properly freed
(lldb) frame variable cmd_list
(lldb) po cmd_list->argv[0]
```

### Parser/Lexer:
```lldb
# Verify lexer/parser cleanup
(lldb) frame variable lexer parser
```

## Test Commands for "echo hello"

Once you understand the flow, test with:
- `echo hello`
- `echo hello world`
- `echo "hello world"`
- `echo 'hello world'`
- `export TEST=value`
- `echo $TEST`
- `exit`

## Expected Clean Exit

At the end of debugging, you should see:
1. All malloc calls matched with corresponding free calls
2. shell->envp properly freed
3. No memory leaks (except readline internal ones)
4. Clean exit with status 0
