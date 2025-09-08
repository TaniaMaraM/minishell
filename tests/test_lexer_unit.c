/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer_unit.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:45:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/02 16:42:41 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "minishell.h"
#include <stdio.h>

static int	test_lexer_basic(void)
{
	t_lexer	*lexer;
	t_token	*token;
	int		passed;

	lexer = lexer_init("echo hello world");
	if (!lexer)
		return (0);
	passed = 1;
	token = lexer_next_token(lexer);
	if (!token || token->type != TOKEN_WORD || ft_strcmp(token->value, "echo"))
		passed = 0;
	token_destroy(token);
	token = lexer_next_token(lexer);
	if (!token || token->type != TOKEN_WORD || ft_strcmp(token->value, "hello"))
		passed = 0;
	token_destroy(token);
	token = lexer_next_token(lexer);
	if (!token || token->type != TOKEN_WORD || ft_strcmp(token->value, "world"))
		passed = 0;
	token_destroy(token);
	token = lexer_next_token(lexer);
	if (!token || token->type != TOKEN_EOF)
		passed = 0;
	token_destroy(token);
	lexer_destroy(lexer);
	return (passed);
}

static int	test_lexer_quotes(void)
{
	t_lexer	*lexer;
	t_token	*token;
	int		passed;

	lexer = lexer_init("echo 'hello world'");
	if (!lexer)
		return (0);
	passed = 1;
	token = lexer_next_token(lexer);
	if (!token || token->type != TOKEN_WORD || ft_strcmp(token->value, "echo"))
		passed = 0;
	token_destroy(token);
	token = lexer_next_token(lexer);
	if (!token || token->type != TOKEN_WORD || ft_strcmp(token->value, "hello world"))
		passed = 0;
	token_destroy(token);
	lexer_destroy(lexer);
	return (passed);
}

static int	test_lexer_metacharacters(void)
{
	t_lexer	*lexer;
	t_token	*token;
	int		passed;

	lexer = lexer_init("echo hello | grep hello");
	if (!lexer)
		return (0);
	passed = 1;
	token = lexer_next_token(lexer);
	if (!token || token->type != TOKEN_WORD)
		passed = 0;
	token_destroy(token);
	token = lexer_next_token(lexer);
	if (!token || token->type != TOKEN_WORD)
		passed = 0;
	token_destroy(token);
	token = lexer_next_token(lexer);
	if (!token || token->type != TOKEN_PIPE)
		passed = 0;
	token_destroy(token);
	lexer_destroy(lexer);
	return (passed);
}

static int	test_lexer_errors(void)
{
	t_lexer	*lexer;
	t_token	*token;
	int		passed;

	lexer = lexer_init("echo 'unterminated");
	if (!lexer)
		return (0);
	passed = 1;
	token = lexer_next_token(lexer);
	if (!token || token->type != TOKEN_WORD)
		passed = 0;
	token_destroy(token);
	token = lexer_next_token(lexer);
	if (!token || token->type != TOKEN_ERROR)
		passed = 0;
	token_destroy(token);
	lexer_destroy(lexer);
	return (passed);
}

int	main(void)
{
	int	passed;
	int	total;

	passed = 0;
	total = 4;
	printf("Running lexer unit tests...\n");
	if (test_lexer_basic())
	{
		printf("✓ Basic tokenization test passed\n");
		passed++;
	}
	else
		printf("✗ Basic tokenization test failed\n");
	if (test_lexer_quotes())
	{
		printf("✓ Quote handling test passed\n");
		passed++;
	}
	else
		printf("✗ Quote handling test failed\n");
	if (test_lexer_metacharacters())
	{
		printf("✓ Metacharacter test passed\n");
		passed++;
	}
	else
		printf("✗ Metacharacter test failed\n");
	if (test_lexer_errors())
	{
		printf("✓ Error detection test passed\n");
		passed++;
	}
	else
		printf("✗ Error detection test failed\n");
	printf("\nResults: %d/%d tests passed\n", passed, total);
	return (passed == total ? 0 : 1);
}
