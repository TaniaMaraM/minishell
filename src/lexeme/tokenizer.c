/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 16:20:00 by rwrobles         #+#    #+#             */
/*   Updated: 2025/08/24 09:45:00 by rwrobles        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "minishell.h"

static void	print_token_info(t_token *token)
{
	if (token->value)
	{
		ft_putstr_fd("Token: ", STDOUT_FILENO);
		ft_putstr_fd(token->value, STDOUT_FILENO);
		ft_putstr_fd(" (type: ", STDOUT_FILENO);
		ft_putnbr_fd(token->type, STDOUT_FILENO);
		ft_putendl_fd(")", STDOUT_FILENO);
	}
}

static int	process_single_token(t_lexer *lexer)
{
	t_token	*token;

	token = lexer_next_token(lexer);
	if (!token)
	{
		print_error("tokenizer", "Failed to get token");
		return (1);
	}
	if (token->type == TOKEN_EOF)
	{
		token_destroy(token);
		return (0);
	}
	else if (token->type == TOKEN_ERROR)
	{
		print_error("tokenizer", "Invalid token");
		token_destroy(token);
		return (1);
	}
	else
		print_token_info(token);
	token_destroy(token);
	return (-1);
}

static int	process_tokens(t_lexer *lexer)
{
	int	status;
	int	result;

	status = 0;
	while (1)
	{
		result = process_single_token(lexer);
		if (result == 0)
			break ;
		else if (result == 1)
		{
			status = 1;
			break ;
		}
	}
	return (status);
}

int	tokenize_and_process(const char *input)
{
	t_lexer	*lexer;
	int		status;

	if (!input)
		return (1);
	lexer = lexer_init(input);
	if (!lexer)
	{
		print_error("tokenizer", "Failed to initialize lexer");
		return (1);
	}
	status = process_tokens(lexer);
	lexer_destroy(lexer);
	return (status);
}
