# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/19 10:42:06 by tmarcos           #+#    #+#              #
#    Updated: 2025/09/02 15:39:48 by tmarcos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libft.a

GREEN		= \033[0;32m
GREY		= \033[0;90m
RED			= \033[0;31m
RESET		= \033[0m

CC 			= cc

FLAGS 		= -Wall -Werror -Wextra

REMOVE 		= rm -f

INCLUDE		= -I .

SRCS 		= ft_isalpha.c 		\
			ft_isdigit.c 		\
			ft_isalnum.c 		\
			ft_isascii.c 		\
			ft_isprint.c 		\
			ft_strlen.c 		\
			ft_memset.c 		\
			ft_bzero.c 			\
			ft_memcpy.c 		\
			ft_memmove.c 		\
			ft_strlcpy.c 		\
			ft_strlcat.c 		\
			ft_toupper.c 		\
			ft_tolower.c 		\
			ft_strchr.c 		\
			ft_strrchr.c 		\
			ft_strncmp.c		\
			ft_memchr.c			\
			ft_memcmp.c			\
			ft_strnstr.c		\
			ft_atoi.c 			\
			ft_strdup.c 		\
			ft_calloc.c 		\
			ft_itoa.c 			\
			ft_substr.c	 		\
			ft_split.c 			\
			ft_strjoin.c 		\
			ft_strtrim.c		\
			ft_strmapi.c 		\
			ft_striteri.c 		\
			ft_putstr_fd.c 		\
			ft_putchar_fd.c 	\
			ft_putendl_fd.c 	\
			ft_putnbr_fd.c		\
			ft_lstnew.c			\
			ft_lstadd_front.c		\
			ft_lstsize.c			\
			ft_lstlast.c			\
			ft_lstadd_back.c		\
			ft_lstdelone.c			\
			ft_lstclear.c			\
			ft_lstiter.c			\
			ft_lstmap.c				\
			ft_strcmp.c				\
			ft_strarr_dup.c			\
			ft_strarr_free.c		\
			ft_printf/ft_printf.c        \
       		ft_printf/ft_print_ch_str.c  \
       		ft_printf/ft_print_iuptr.c	 \

OBJ_DIR ?= obj
OBJS            = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all:		$(NAME)

$(NAME): 	$(OBJS)
			@ar -rcs $(NAME) $(OBJS)
			@echo "$(GREY)Libft: $(GREEN)$(NAME) was created$(RESET)"

bonus:		$(OBJS)
			@ar -rcs $(NAME) $(OBJS) 
			@echo "$(GREY)Libft: $(GREEN)$(NAME) was created with Bonus$(RESET)"

$(OBJ_DIR)/%.o: %.c libft.h
			@mkdir -p $(dir $@)
			@$(CC) $(FLAGS) $(INCLUDE) -c $< -o $@

clean:
			@rm -rf $(OBJ_DIR)
			@echo "$(GREY)Libft: object files were deleted$(RESET)"

fclean:		clean
			@$(REMOVE) $(NAME)
			@echo "$(GREY)Libft: $(NAME) was deleted$(RESET)"

re:			fclean $(NAME)
		
.PHONY:		all clean fclean re bonus rebonus
