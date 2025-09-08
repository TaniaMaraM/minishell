# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/22 16:40:49 by tmarcos           #+#    #+#              #
#    Updated: 2025/08/01 14:24:59 by tmarcos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME		= libftprintf.a

GREEN		= \033[0;32m
GREY		= \033[0;90m
RED			= \033[0;31m
RESET		= \033[0m

CC 			= cc

FLAGS 		= -Wall -Werror -Wextra

REMOVE 		= rm -f

INCLUDE		= -I .

SRCS 		= ft_printf.c			\
				ft_print_ch_str.c			\
				ft_print_iuptr.c				\


OBJ_DIR ?= obj
OBJS            = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all:		$(NAME)

$(NAME): $(OBJS)
	@ar -rcs $(NAME) $(OBJS)
	@echo "$(GREY)libftprintf: $(GREEN)$(NAME) was created$(RESET)"

$(OBJ_DIR)/%.o: %.c ft_printf.h
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) $(INCLUDE) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(GREY)libftprintf: object files were deleted$(RESET)"

fclean:		clean
	@$(REMOVE) $(NAME)
	@echo "$(GREY)libftprintf: $(NAME) was deleted$(RESET)"

re:			fclean all

.PHONY:		all clean fclean
