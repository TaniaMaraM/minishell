NAME		= libgnl.a

GREEN		= \033[0;32m
RED			= \033[0;31m
RESET		= \033[0m

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDE		= -I . -I ../libft

SRC_DIR		= .
OBJ_DIR		?= obj

SRCS		= get_next_line.c

OBJS		= $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)
	@echo "$(GREEN)$(NAME) was created$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	@echo "$(RED)Object files removed$(RESET)"

fclean: clean
	rm -f $(NAME)
	@echo "$(RED)$(NAME) removed$(RESET)"

re: fclean all

.PHONY: all clean fclean re
