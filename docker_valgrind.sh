#!/bin/bash

# Script simples para testar minishell com Valgrind no Docker
# Uso: ./docker_valgrind.sh [comando]

set -e

# Cores
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}🐳 Minishell Docker Valgrind Tester${NC}"
echo "=================================="

case "${1:-interactive}" in
    "build")
        echo -e "${YELLOW}🔨 Construindo imagem Docker...${NC}"
        docker build -t minishell-valgrind .
        echo -e "${GREEN}✅ Imagem construída!${NC}"
        ;;
    "run"|"interactive")
        echo -e "${YELLOW}🐳 Iniciando container interativo...${NC}"
        echo "Comandos úteis dentro do container:"
        echo "  ./help.sh        - Mostrar ajuda"
        echo "  ./quick_test.sh  - Teste rápido"
        echo "  valgrind ./minishell - Valgrind manual"
        echo ""
        docker run -it --rm minishell-valgrind
        ;;
    "test")
        echo -e "${YELLOW}🧪 Executando teste rápido...${NC}"
        docker run --rm minishell-valgrind ./quick_test.sh
        ;;
    "clean")
        echo -e "${YELLOW}🧹 Limpando imagens Docker...${NC}"
        docker rmi minishell-valgrind 2>/dev/null || true
        docker system prune -f
        echo -e "${GREEN}✅ Limpeza concluída!${NC}"
        ;;
    *)
        echo "Uso: $0 [comando]"
        echo ""
        echo "Comandos:"
        echo "  build       - Construir imagem Docker"
        echo "  run         - Container interativo (padrão)"
        echo "  test        - Teste rápido"
        echo "  clean       - Limpar imagens"
        ;;
esac
