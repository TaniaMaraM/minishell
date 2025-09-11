
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

# Project structure
SRC_DIR     = src
OBJ_DIR     = obj
INCLUDE_DIR = include
LIB_DIR     = lib

# Main source subdirectories
SRC_SUBDIRS = app lexeme parser expand exec builtin signals utils
SRC_DIRS    = $(addprefix $(SRC_DIR)/,$(SRC_SUBDIRS))

# Additional subdirectories (for better organization)
EXEC_SUBDIRS = exec/heredoc exec/pipeline exec/command

# Custom libs
LIBFT_DIR = $(LIB_DIR)/libft
GNL_DIR   = $(LIB_DIR)/get_next_line

LIBFT = $(LIBFT_DIR)/libft.a
GNL   = $(GNL_DIR)/libgnl.a

# Include paths
INCLUDES = -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -I$(GNL_DIR) $(READLINE_INC)

# Find all .c files in main directories, excluding subdirectories
SRC_FILES := $(shell find $(SRC_DIRS) -type f -name '*.c' | grep -v "src/exec/heredoc\|src/exec/pipeline\|src/exec/command")

# Add files from exec subdirectories separately
EXEC_HEREDOC_FILES := $(wildcard $(SRC_DIR)/exec/heredoc/*.c)
EXEC_PIPELINE_FILES := $(wildcard $(SRC_DIR)/exec/pipeline/*.c)
EXEC_COMMAND_FILES := $(wildcard $(SRC_DIR)/exec/command/*.c)

# Combine all source files for compilation
SRC_FILES += $(EXEC_HEREDOC_FILES) $(EXEC_PIPELINE_FILES) $(EXEC_COMMAND_FILES)

# Generate object file paths from source files
OBJS = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default build target
all: $(OBJ_DIR) $(NAME)

# Show detected OS
info:
	@echo "$(GREEN)[System Info]$(RESET)"
	@echo "OS: $(OS)"
	@echo "Readline Include: $(READLINE_INC)"
	@echo "Readline Library: $(READLINE_LIB)"

# Create object directory structure recursively
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

# Main test rule
test: $(NAME)
	@echo "$(GREEN)[Running subject compliance test]$(RESET)"
	@chmod +x ./tests/test
	@./tests/test

# Testing rules for specific phases
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

test-evaluation:
	@echo "$(GREEN)[Running evaluation simulation tests]$(RESET)"
	@./tests/test_evaluation.sh

# Docker commands
valgrind:
	@echo "$(GREEN)[Running]$(RESET) Valgrind check in Docker"
	@docker build -t minishell_valgrind .
	@docker run --rm minishell_valgrind

docker-clean:
	@echo "$(RED)[Docker]$(RESET) Removing Docker image"
	@docker rmi minishell_valgrind 2>/dev/null || true

.PHONY: all clean fclean re test test-phase0 test-phase1 test-phase2 test-phase4 test-phase5 test-unit test-lexer test-builtin valgrind docker-clean
