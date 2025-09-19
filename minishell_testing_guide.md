# Minishell vs Bash Manual Testing Guide

This comprehensive testing guide will help you verify that your minishell behaves exactly like bash in all required scenarios.

## Setup Instructions

1. Open two terminals side by side
2. In terminal 1: Run your minishell (`./minishell`)
3. In terminal 2: Run bash (`bash`)
4. Test each command in both shells and compare outputs

---

## 1. Basic Command Execution

### Absolute Paths
```bash
# Test these commands in both shells
/bin/ls -
/bin/pwd 
/bin/echo "hello world" 
/bin/ls -la 
```

### Commands with Arguments
```bash
# Test various commands with arguments
ls -l -a 
echo hello world 
cat /etc/passwd 
grep root /etc/passwd 
```

### Empty Commands and Whitespace - OK
```bash
# Press Enter on empty line
[ENTER]

# Only spaces
   
# Only tabs
	

# Mixed spaces and tabs
  	  
```

---

## 2. Built-in Commands Testing

### Echo Command
```bash
echo
echo hello
echo hello world
echo "hello world"
echo 'hello world'
echo -n hello
echo -n "hello world"
echo -n
echo -n -n hello 
echo -n -n -nnnnn hello 
echo hello -n world
```

### Exit Command
```bash
exit
exit 0
exit 1
exit 42
exit 255
exit 256
exit -1
exit abc 
exit 1 2 3
```
*Note: After each exit test, restart both shells*

### PWD Command
```bash
pwd 
cd /tmp
pwd
cd ..
pwd
cd /
pwd
```

### CD Command
```bash
cd
cd ~
cd /tmp
cd ..
cd ../..
cd .
cd /
cd /nonexistent
cd ""
cd /root
pwd  # Check after each cd
```

### ENV Command
```bash
env
env | grep USER
env | grep PATH
```

### Export Command
```bash
export TESTVAR=hello
export TESTVAR2="hello world"
export TESTVAR3='hello world'
export TESTVAR4=
export =value
export 123VAR=test
export VAR-NAME=test
env | grep TESTVAR
echo $TESTVAR
```

### Unset Command
```bash
export TESTVAR=hello
echo $TESTVAR
unset TESTVAR
echo $TESTVAR
```

---

## 3. Environment Variables

### Basic Variable Expansion
```bash
echo $USER
echo $HOME
echo $PATH
echo $PWD
echo $?
echo $NONEXISTENT
```

### Exit Status Testing
```bash
/bin/true
echo $?
/bin/false
echo $?
/bin/ls
echo $?
/bin/ls /nonexistent
echo $?
nosuchcommand
echo $?
```

### Complex Variable Usage = DEU ERRO NAO ARMAZENA
```bash
export VAR=hello
echo $VAR
echo "$VAR"
echo '$VAR'
echo $VAR$VAR
echo ${VAR}world
echo pre$VAR
echo $VAR post
```

### Variables in Different Contexts
```bash
export TESTVAR=value
echo $TESTVAR
echo "$TESTVAR"
echo '$TESTVAR'
echo "pre $TESTVAR post"
echo 'pre $TESTVAR post'
```

---

## 4. Quoting Tests

### Single Quotes
```bash
echo 'hello world'
echo 'hello $USER world'
echo 'hello | grep world'
echo 'hello > file'
echo 'hello && world'
echo '$USER'
echo '$?'
echo ''
ech'o' a
ech"o" a	
echo'a'	
```

### Double Quotes
```bash
	
echo "hello world"
echo "hello $USER world"
echo "hello | grep world"
echo "hello > file"
echo "hello && world"
echo "$USER"
echo "$?"
echo ""
echo "" | cat -e	
echo "" bonjour	
echo "cat lol.c | cat > lol.c"
echo """abc"""	
echo "ls"="lasa"
```

### Mixed Quoting
```bash
echo "hello 'world'"
echo 'hello "world"'
echo "hello" world
echo 'hello' world
echo "hello"'world'
echo 'hello'"world"
```

---

## 5. Redirections

### Input Redirection (<)
```bash
echo "line1\nline2\nline3" > testfile.txt
cat < testfile.txt
grep line2 < testfile.txt
wc -l < testfile.txt
cat < nonexistent.txt
```

### Output Redirection (>)
```bash
echo "hello" > output.txt
cat output.txt
ls -l > filelist.txt
cat filelist.txt
echo "world" > output.txt
cat output.txt
```

### Append Redirection (>>)
```bash
echo "line1" > append.txt
echo "line2" >> append.txt
echo "line3" >> append.txt
cat append.txt
```

### Here Document (<<)
```bash
cat << EOF
hello
world
EOF
grep hello << END
hello world
test hello
goodbye
END

cat << DELIMITER
line with $USER
line with 'quotes'
line with "double quotes"
DELIMITER
```

### Multiple Redirections
```bash
echo "test" > file1.txt > file2.txt
cat < file1.txt > output.txt
```

---

## 6. Pipes

### Simple Pipes
```bash
ls | grep test
echo "hello world" | cat
cat /etc/passwd | grep root
echo "one\ntwo\nthree" | wc -l
```

### Multiple Pipes
```bash
ls -l | grep test | wc -l
echo "hello\nworld\ntest" | grep e | wc -l
cat /etc/passwd | head -5 | tail -2
```

### Pipes with Errors
```bash
ls /nonexistent | grep test
nosuchcommand | cat
ls | nosuchcommand
```

### Pipes with Redirections
```bash
ls | cat > output.txt
echo "test" | cat > input.txt
cat < input.txt | grep test > output.txt
```

---

## 7. Signal Handling

### Ctrl+D Tests
1. Empty prompt + Ctrl+D → Should exit shell
2. Type command (don't execute) + Ctrl+D → Should do nothing
3. Run `cat` (without args) then Ctrl+D → Should end cat input

### Ctrl+C Tests
1. Empty prompt + Ctrl+C → Should show new prompt
2. Type command (don't execute) + Ctrl+C → Should clear line and show new prompt
3. Press Enter after Ctrl+C → Should not execute cleared command
4. Run `sleep 10` then Ctrl+C → Should interrupt and show new prompt
5. Run `cat` (without args) then Ctrl+C → Should interrupt and show new prompt

### Ctrl+\ Tests
1. Empty prompt + Ctrl+\ → Should do nothing
2. Type command (don't execute) + Ctrl+\ → Should do nothing
3. Run `sleep 10` then Ctrl+\ → Should send SIGQUIT (quit with core dump in bash)

---

## 8. PATH and Executable Finding

### PATH Manipulation - TESTAR DE NOVO NO PC DA ESCOLA
```bash
echo $PATH
export PATH=/bin:/usr/bin
ls
pwd
export PATH=/usr/bin
ls  # Should work if ls is in /usr/bin
export PATH=/nonexistent
ls  # Should fail
unset PATH
ls  # Should fail
export PATH=/bin:/usr/bin
ls  # Should work again
```

### Relative Paths
```bash
cd /bin
./ls
cd /usr/bin
cd /tmp
../bin/ls  # If this path exists
```

---

## 9. Error Handling

### Non-existent Commands
```bash
nosuchcommand
./nonexistent
/bin/nonexistent
```

### Non-existent Files
```bash
cat nonexistent.txt
ls nonexistent_dir
cd nonexistent_dir
```

### Permission Errors
```bash
# Try to access restricted areas
cd /root
ls /root
cat /etc/shadow
```

---

## 10. Complex Command Testing

### Long Commands
```bash
echo this is a very long command with many arguments and words to test the parsing capabilities
ls -l -a -h -t -r --color=auto /usr/bin /bin /sbin | head -20 | tail -10
```

### Mixed Features
```bash
export VAR="hello world"
echo 'test $VAR' | grep test
ls -l | grep "^d" > directories.txt
cat << EOF | grep hello > output.txt
hello world
test
hello again
EOF
```

### Edge Cases
```bash
cat | cat | ls
echo "" | cat
echo | cat | echo test
```

---
