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

# Main source directories
SRC_APP     = $(SRC_DIR)/app
SRC_LEXEME  = $(SRC_DIR)/lexeme
SRC_PARSER  = $(SRC_DIR)/parser
SRC_EXPAND  = $(SRC_DIR)/expand
SRC_EXEC    = $(SRC_DIR)/exec
SRC_BUILTIN = $(SRC_DIR)/builtin
SRC_SIGNALS = $(SRC_DIR)/signals
SRC_UTILS   = $(SRC_DIR)/utils

# Exec subdirectories
SRC_EXEC_HEREDOC = $(SRC_EXEC)/heredoc
SRC_EXEC_PIPELINE = $(SRC_EXEC)/pipeline
SRC_EXEC_COMMAND = $(SRC_EXEC)/command

# Custom libs
LIBFT_DIR = $(LIB_DIR)/libft
GNL_DIR   = $(LIB_DIR)/get_next_line

LIBFT = $(LIBFT_DIR)/libft.a
GNL   = $(GNL_DIR)/libgnl.a

# Include paths
INCLUDES = -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -I$(GNL_DIR) $(READLINE_INC)

# Source files by directory
SRC_APP_FILES = cleanup.c init.c input_handler.c loop.c main.c
SRC_LEXEME_FILES = lexer_char_checks.c lexer_parser.c lexer_reader.c lexer_utils.c \
                   lexer.c quote_handling.c tokenizer.c
SRC_PARSER_FILES = command.c parser_argument_process.c parser_argument.c \
                   parser_integration.c parser_main.c parser_memory.c \
                   parser_parse.c parser_utils.c parser.c redirection.c
SRC_EXPAND_FILES = braced_variable.c expander_char.c expander_escape.c \
                   expander_main.c expander_memory.c expander_string.c \
                   expander_utils.c expander_variable.c expander.c \
                   variable_resolution.c
SRC_EXEC_FILES = executor.c
SRC_BUILTIN_FILES = builtin_cd.c builtin_detection.c builtin_echo.c builtin_env.c \
                    builtin_execution.c builtin_exit.c builtin_export.c builtin_pwd.c \
                    builtin_unset.c cd_utils.c env_helpers.c env_utils.c \
                    export_helpers.c export_var.c
SRC_SIGNALS_FILES = heredoc_signals.c signals.c
SRC_UTILS_FILES = command_errors.c error.c

# Exec subdirectory files
SRC_EXEC_HEREDOC_FILES = build_heredoc_utils.c build_heredoc.c heredoc_utils.c heredoc.c
SRC_EXEC_PIPELINE_FILES = executor_pipeline.c pipeline_helpers.c pipeline_process.c pipeline.c
SRC_EXEC_COMMAND_FILES = cleanup_heredoc_fds.c external_execution.c external_helpers.c \
                         redirection_handlers.c redirections.c single_command_exec.c \
                         single_command.c

# Prepend directory paths to source files
SRCS_APP     = $(addprefix $(SRC_APP)/, $(SRC_APP_FILES))
SRCS_LEXEME  = $(addprefix $(SRC_LEXEME)/, $(SRC_LEXEME_FILES))
SRCS_PARSER  = $(addprefix $(SRC_PARSER)/, $(SRC_PARSER_FILES))
SRCS_EXPAND  = $(addprefix $(SRC_EXPAND)/, $(SRC_EXPAND_FILES))
SRCS_EXEC    = $(addprefix $(SRC_EXEC)/, $(SRC_EXEC_FILES))
SRCS_BUILTIN = $(addprefix $(SRC_BUILTIN)/, $(SRC_BUILTIN_FILES))
SRCS_SIGNALS = $(addprefix $(SRC_SIGNALS)/, $(SRC_SIGNALS_FILES))
SRCS_UTILS   = $(addprefix $(SRC_UTILS)/, $(SRC_UTILS_FILES))
SRCS_EXEC_HEREDOC = $(addprefix $(SRC_EXEC_HEREDOC)/, $(SRC_EXEC_HEREDOC_FILES))
SRCS_EXEC_PIPELINE = $(addprefix $(SRC_EXEC_PIPELINE)/, $(SRC_EXEC_PIPELINE_FILES))
SRCS_EXEC_COMMAND = $(addprefix $(SRC_EXEC_COMMAND)/, $(SRC_EXEC_COMMAND_FILES))

# Combine all source files
SRC_FILES = $(SRCS_APP) $(SRCS_LEXEME) $(SRCS_PARSER) $(SRCS_EXPAND) \
            $(SRCS_EXEC) $(SRCS_BUILTIN) $(SRCS_SIGNALS) $(SRCS_UTILS) \
            $(SRCS_EXEC_HEREDOC) $(SRCS_EXEC_PIPELINE) $(SRCS_EXEC_COMMAND)

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
	@mkdir -p $(OBJ_DIR)/app
	@mkdir -p $(OBJ_DIR)/lexeme
	@mkdir -p $(OBJ_DIR)/parser
	@mkdir -p $(OBJ_DIR)/expand
	@mkdir -p $(OBJ_DIR)/exec
	@mkdir -p $(OBJ_DIR)/builtin
	@mkdir -p $(OBJ_DIR)/signals
	@mkdir -p $(OBJ_DIR)/utils
	@mkdir -p $(OBJ_DIR)/exec/heredoc
	@mkdir -p $(OBJ_DIR)/exec/pipeline
	@mkdir -p $(OBJ_DIR)/exec/command

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
	@chmod +x ./test/test
	@./test/test

# Testing rules 
test-edge-cases:
	@echo "$(GREEN)[Running edge case tests]$(RESET)"
	@./test/test_edge_cases.sh

test-evaluation:
	@echo "$(GREEN)[Running evaluation simulation tests]$(RESET)"
	@./test/test_evaluation.sh

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

# Valgrind rules
valgrind: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline_suppress.supp ./$(NAME)
valchild: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=yes --suppressions=readline_suppress.supp ./$(NAME)

.PHONY: all clean fclean re test test-phase0 test-phase1 test-phase2 test-phase4 test-phase5 test-edge-cases test-evaluation valgrind