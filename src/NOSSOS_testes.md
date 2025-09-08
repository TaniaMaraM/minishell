Testes que nao passaram:

1. echo -n -n hello 
Nosso minishell:
minishell$ echo -n -n hello
-n hellominishell$ echo -n -n -n -n -nnnnnnn hello
-n -n -n -nnnnnnn hellominishell$ 

Bash:
bash-3.2$ echo -n -n hello
hellobash-3.2$ echo -n -n hello
hellobash-3.2$ echo -n -n -n -n -nnnnnnn hello
hellobash-3.2$ 

*** Eh so quando eh -n consecutivo
ignorar todos os options 



EXIT:
Nosso minishel nao ta exit:
comando: exit hahahah

bash-3.2$ exit hahahah
exit
bash: exit: hahahah: numeric argument required
minishell$ exit hahaha
~

minishell: exit: numeric argument required
minishell$ 



PWD:
nosso minishell:
minishell$ pwd
/Users/taninha/42cursus/rank03/trying_minishell
minishell$ cd /tmp
minishell$ pwd
/Users/taninha/42cursus/rank03/trying_minishell
minishell$ cd ..
minishell$ pwd
/Users/taninha/42cursus/rank03/trying_minishell
minishell$ cd /
minishell$ pwd
/Users/taninha/42cursus/rank03/trying_minishell
minishell$ 

Bash:
bash-3.2$ pwd
/Users/taninha
bash-3.2$ cd /tmp
bash-3.2$ pwd
/tmp
bash-3.2$ cd ..
bash-3.2$ pwd
/
bash-3.2$ cd /
bash-3.2$ pwd
/
bash-3.2$ 

Verificar porque nao ta direcionando os comandos cd:
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
pwd  # Check after each cd


EXPORT:
Nosso minishell:
minishell$ export TESTVAR=hello
minishell$ export TESTVAR2="hello world"
minishell$ export TESTVAR3='hello world'
minishell$ export TESTVAR4=
minishell$ export =value
minishell: export: not a valid identifier
minishell$ export 123VAR=test
minishell: export: not a valid identifier
minishell$ export VAR-NAME=test
minishell: export: not a valid identifier
Nao funcionou: pq?
minishell$ env | grep TESTVAR
minishell$ env | grep TESTVAR
minishell$ echo $TESTVAR

minishell$ echo $TESTVAR2



BASH:
bash-3.2$ export TESTVAR=hello
bash-3.2$ export TESTVAR2="hello world"
bash-3.2$ export TESTVAR3='hello world'
bash-3.2$ export TESTVAR4=
bash-3.2$ export =value
bash: export: `=value': not a valid identifier
bash-3.2$ export 123VAR=test
bash: export: `123VAR=test': not a valid identifier
bash-3.2$ export VAR-NAME=test
bash: export: `VAR-NAME=test': not a valid identifier
bash-3.2$ env | grep TESTVAR
TESTVAR4=
TESTVAR3=hello world
TESTVAR2=hello world
TESTVAR=hello
bash-3.2$ echo $TESTVAR
hello
bash-3.2$ echo $TESTVAR2
hello world

OK

Como saber se unset ta funcionando?
minishell$ unset TESTVAR
minishell$ echo $TESTVAR

bash-3.2$ unset TESTVAR
bash-3.2$ echo $TESTVAR

### Exit Status Testing
Frase de retorno de erro diferente do bash.

n/true
minishell: /bin/true: command not found
minishell$ echo $?
127
minishell$ /bin/false
minishell: /bin/false: command not found
minishell$ echo $?
127
Esse caso ta igual ao bash:
minishell$ nosuchcommand
minishell: nosuchcommand: command not found
minishell$ echo $?
127

bash-3.2$ /bin/true
bash: /bin/true: No such file or directory
bash-3.2$ echo $?
127
bash-3.2$ /bin/false
bash: /bin/false: No such file or directory
bash-3.2$ echo $?
127

bash-3.2$ nosuchcommand
bash: nosuchcommand: command not found
bash-3.2$ echo $?
127

### Complex Variable Usage = nao ta armazenando.

minishell$ export VAR=hello
minishell$ echo $VAR

minishell$ echo "$VAR"

minishell$ echo '$VAR'
$VAR
minishell$ echo $VAR$VAR

minishell$ echo ${VAR}world
${VAR}world
minishell$ echo pre$VAR
pre
minishell$ echo $VAR post
 post

bash-3.2$ export VAR=hello
bash-3.2$ echo $VAR
hello
bash-3.2$ echo "$VAR"
hello
bash-3.2$ echo '$VAR'
$VAR
bash-3.2$ echo $VAR$VAR
hellohello
bash-3.2$ echo ${VAR}world
helloworld
bash-3.2$ echo pre$VAR
prehello
bash-3.2$ echo $VAR post
hello post

handle esses casos tb:
```bash
export TESTVAR=value
echo $TESTVAR
echo "$TESTVAR"
echo '$TESTVAR'
echo "pre $TESTVAR post"
echo 'pre $TESTVAR post'
```

CTRL + \ :
Run `sleep 10` then Ctrl+\ → Should send SIGQUIT (quit with core dump in bash)

bash-3.2$ sleep 10
^\Quit: 3
bash-3.2$ echo $?
131

Nosso:
minishell$ sleep 10
^\minishell$ echo $?
0




TESTAR NOVAMENTE DEPOIS QUE ARRUMAR PWD E EXPORT:
## 8. PATH and Executable Finding

### PATH Manipulation
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



Após nossas alterações, os builtins que serão executados no processo filho são:
echo - Apenas exibe texto na saída padrão
pwd - Apenas mostra o diretório atual
env - Apenas exibe as variáveis de ambiente
Enquanto os builtins que serão executados no processo pai são:
cd - Muda o diretório de trabalho (afeta o estado do shell)
export - Define/modifica variáveis de ambiente (afeta o estado do shell)
unset - Remove variáveis de ambiente (afeta o estado do shell)
exit - Encerra o shell (afeta o estado do shell)
