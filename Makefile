# Program name
NAME = minishell

# Colors for visual feedback
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m
CHECK = \033[0;34m✓\033[0m

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

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

# Source subdirectories
SRC_SUBDIRS = app lexeme parser expand exec builtin signals utils
SRC_DIRS    = $(addprefix $(SRC_DIR)/,$(SRC_SUBDIRS))

# Custom libs
LIBFT_DIR = $(LIB_DIR)/libft
GNL_DIR   = $(LIB_DIR)/get_next_line

LIBFT = $(LIBFT_DIR)/libft.a
GNL   = $(GNL_DIR)/libgnl.a

# Include paths
INCLUDES = -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -I$(GNL_DIR) $(READLINE_INC)

# Automatically detect all .c files under src/
SRC_FILES := $(shell find $(SRC_DIRS) -type f -name '*.c')

# Create matching .o paths in obj/
OBJS = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default rule
all: $(OBJ_DIR) $(NAME)

# Create obj/ directory structure recursively
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(addprefix $(OBJ_DIR)/, $(sort $(dir $(SRC_FILES:$(SRC_DIR)/%=%))))

# Link final binary
$(NAME): $(LIBFT) $(GNL) $(OBJS)
	@echo "$(GREEN)[Linking]$(RESET) $(NAME)"
	$(CC) $(CFLAGS) $(OBJS) $(GNL) $(LIBFT) $(READLINE_LIB) -lreadline -o $(NAME)
	@echo "$(CHECK) Executable built successfully"

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

# Evaluation test
test-eval: $(NAME)
	@echo "$(GREEN)[Running evaluation tester]$(RESET)"
	@chmod +x ./tests/evaluation_tester.sh
	@./tests/evaluation_tester.sh

# Valgrind check (Linux only)
valgrind: $(NAME)
	@echo "$(GREEN)[Running Valgrind memory check]$(RESET)"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

# Show system info
info:
	@echo "$(GREEN)[System Info]$(RESET)"
	@echo "OS: $(OS)"
	@echo "Readline Include: $(READLINE_INC)"
	@echo "Readline Library: $(READLINE_LIB)"

.PHONY: all clean fclean re test-eval valgrind info
