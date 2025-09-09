# Program name
NAME = minishell

# Colors for visual feedback
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m
CHECK = \033[0;34m✔\033[0m

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Libraries flags (readline added here)
LDLIBS = -lreadline

# Detect OS for readline path
OS := $(shell uname)
ifeq ($(OS),Darwin)
	# macOS with Homebrew
	READLINE_INC = -I/opt/homebrew/opt/readline/include
	READLINE_LIB = -L/opt/homebrew/opt/readline/lib
else
	# Linux (default system readline)
	READLINE_INC = 
	READLINE_LIB = 
endif

# 📂 Project structure
SRC_DIR     = src
OBJ_DIR     = obj
INCLUDE_DIR = include
LIB_DIR     = lib

# Source subdirectories
SRC_SUBDIRS = app lexeme parser expand exec builtin signals utils
SRC_DIRS    = $(addprefix $(SRC_DIR)/,$(SRC_SUBDIRS))

# Custom libs
LIBFT_DIR = $(LIB_DIR)/libft
GNL_DIR   = $(LIB_DIR)/get_next_line

LIBFT = $(LIBFT_DIR)/libft.a
GNL   = $(GNL_DIR)/libgnl.a

# 📚 Include paths
INCLUDES = -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -I$(GNL_DIR) $(READLINE_INC)

# Automatically detect all .c files under src/
SRC_FILES := $(shell find $(SRC_DIRS) -type f -name '*.c')

# 🔧 Create matching .o paths in obj/
OBJS = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# 🎯 Default rule
all: $(OBJ_DIR) $(NAME)

# Show detected OS
info:
	@echo "$(GREEN)[System Info]$(RESET)"
	@echo "OS: $(OS)"
	@echo "Readline Include: $(READLINE_INC)"
	@echo "Readline Library: $(READLINE_LIB)"

# 📁 Create obj/ directory structure recursively
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(addprefix $(OBJ_DIR)/, $(sort $(dir $(SRC_FILES:$(SRC_DIR)/%=%))))

# Link final binary
$(NAME): $(LIBFT) $(GNL) $(OBJS)
	@echo "$(GREEN)[Linking]$(RESET) $(NAME)"
	$(CC) $(CFLAGS) $(OBJS) $(GNL) $(LIBFT) $(READLINE_LIB) -lreadline -o $(NAME)
	@echo "$(CHECK) Executable built successfully ✅"

# Compile each .c into obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(GREEN)[Compiling]$(RESET) $<"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile external libs
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) OBJ_DIR=obj

$(GNL):
	@$(MAKE) -C $(GNL_DIR) OBJ_DIR=obj

# Cleaning
clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean OBJ_DIR=obj
	@$(MAKE) -C $(GNL_DIR) clean OBJ_DIR=obj
	@echo "$(RED)Object files cleaned.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean OBJ_DIR=obj
	@$(MAKE) -C $(GNL_DIR) fclean OBJ_DIR=obj
	@echo "$(RED)Binary $(NAME) removed.$(RESET)"

re: fclean all

# Testing rules
test: $(NAME)
	@echo "$(GREEN)[Running comprehensive test suite]$(RESET)"
	@./tests/run_all_tests.sh

test-phase0:
	@echo "$(GREEN)[Running Phase 0 tests]$(RESET)"
	@./tests/test_phase0.sh

test-phase1:
	@echo "$(GREEN)[Running Phase 1 tests]$(RESET)"
	@./tests/test_phase1.sh

test-phase2:
	@echo "$(GREEN)[Running Phase 2 tests]$(RESET)"
	@./tests/test_phase2.sh

test-phase4:
	@echo "$(GREEN)[Running Phase 4 tests]$(RESET)"
	@./tests/test_phase4.sh

test-phase5:
	@echo "$(GREEN)[Running Phase 5 tests]$(RESET)"
	@./tests/test_phase5.sh

test-edge-cases:
	@echo "$(GREEN)[Running edge case tests]$(RESET)"
	@./tests/test_edge_cases.sh

test-stress:
	@echo "$(GREEN)[Running stress tests]$(RESET)"
	@./tests/test_stress.sh

test-evaluation:
	@echo "$(GREEN)[Running evaluation simulation tests]$(RESET)"
	@./tests/test_evaluation.sh

test-advanced: test-edge-cases test-stress test-evaluation
	@echo "$(GREEN)[All advanced tests completed]$(RESET)"

test-eval-visual:
	@echo "$(GREEN)[Running visual evaluation tester]$(RESET)"
	@chmod +x ./tests/evaluation_tester.sh
	@./tests/evaluation_tester.sh

test-unit:
	@echo "$(GREEN)[Running unit tests]$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) tests/test_lexer_unit.c src/lexeme/*.c src/utils/error.c $(LIBFT) -o test_lexer_unit
	@./test_lexer_unit
	@rm -f test_lexer_unit

test-lexer: test-unit
	@echo "$(GREEN)[Lexer unit tests completed]$(RESET)"

test-builtin:
	@echo "$(GREEN)[Running builtin unit tests]$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) tests/test_builtin_unit.c src/builtin/*.c src/expand/*.c src/utils/error.c $(LIBFT) -o test_builtin_unit
	@./test_builtin_unit
	@rm -f test_builtin_unit

# Docker commands
valgrind:
	@echo "$(GREEN)[Running]$(RESET) Valgrind check in Docker"
	@docker build -t minishell_valgrind .
	@docker run --rm minishell_valgrind

valgrind-segfault:
	@echo "$(GREEN)[Running]$(RESET) Valgrind check for segfault case"
	@docker build -t minishell_valgrind .
	@docker run --rm minishell_valgrind bash -c "echo 'echo \"\"' | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell"

docker-clean:
	@echo "$(RED)[Docker]$(RESET) Removing Docker image"
	@docker rmi minishell_valgrind 2>/dev/null || true

.PHONY: all clean fclean re test test-phase0 test-phase1 test-phase2 test-phase4 test-phase5 test-unit test-lexer test-builtin valgrind valgrind-segfault docker-clean