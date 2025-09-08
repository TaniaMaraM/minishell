FROM ubuntu:latest

# Instalar dependências necessárias
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    valgrind \
    libreadline-dev \
    gdb \
    bash \
    && rm -rf /var/lib/apt/lists/*

# Configurar o ambiente
WORKDIR /app
COPY . .

# Compilar o projeto
RUN make re

# Script para executar o valgrind
RUN echo '#!/bin/bash\n\
echo "Running Valgrind on minishell..."\n\
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=/app/bash.supp ./minishell "$@"\n\
' > /app/run_valgrind.sh && chmod +x /app/run_valgrind.sh

# Criar um arquivo de supressão para readline
RUN echo '{\n\
    <Readline>\n\
    Memcheck:Leak\n\
    ...\n\
    fun:readline\n\
    ...\n\
}\n\
{\n\
    <AddHistory>\n\
    Memcheck:Leak\n\
    ...\n\
    fun:add_history\n\
    ...\n\
}\n\
' > /app/bash.supp

# Comando padrão para iniciar o container
ENTRYPOINT ["/bin/bash"]
