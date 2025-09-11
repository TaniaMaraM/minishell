FROM ubuntu:22.04

# Instalar dependências necessárias
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    valgrind \
    libreadline-dev \
    gdb \
    bash \
    vim \
    nano \
    && rm -rf /var/lib/apt/lists/*

# Configurar o ambiente
WORKDIR /app
COPY . .

# Compilar o projeto
RUN make re

# Criar apenas um script de ajuda simples
RUN echo '#!/bin/bash\n\
echo "🐳 Minishell Valgrind Environment"\n\
echo "================================="\n\
echo ""\n\
echo "Comandos Valgrind úteis:"\n\
echo "  valgrind ./minishell                                    - Básico"\n\
echo "  valgrind --leak-check=full ./minishell                  - Completo"\n\
echo "  valgrind --leak-check=full --log-file=log.txt ./minishell - Com log"\n\
echo ""\n\
echo "Exemplos de teste manual:"\n\
echo "  echo \"echo hello\\nexit\" | valgrind --leak-check=full ./minishell"\n\
echo "  echo \"pwd\\nls\\nexit\" | valgrind --leak-check=full --log-file=test1.log ./minishell"\n\
echo "  echo \"export VAR=test\\necho \$VAR\\nexit\" | valgrind --leak-check=full --log-file=test2.log ./minishell"\n\
echo ""\n\
echo "Para testar manualmente:"\n\
echo "  1. Execute: valgrind --leak-check=full --log-file=meu_teste.log ./minishell"\n\
echo "  2. Digite seus comandos"\n\
echo "  3. Digite exit"\n\
echo "  4. Veja o log: cat meu_teste.log"\n\
echo ""\n\
' > /app/help.sh && chmod +x /app/help.sh

# Comando padrão para iniciar o container
CMD ["/bin/bash"]
