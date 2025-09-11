/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 09:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/11 20:54:24 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parser	*parser_init(t_lexer *lexer, t_shell *shell)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
	{
		lexer_destroy(lexer);
		return (NULL);
	}
	parser->lexer = lexer;
	parser->current_token = NULL;
	parser->cmd_list = NULL;
	parser->error = 0;
	parser->shell = shell;
	return (parser);
}

void	parser_destroy(t_parser *parser)
{
	if (!parser)
		return ;
	if (parser->current_token)
		token_destroy(parser->current_token);
	free(parser);
}

int	parser_advance(t_parser *parser)
{
	if (parser->current_token)
		token_destroy(parser->current_token);
	parser->current_token = lexer_next_token(parser->lexer);
	if (!parser->current_token)
	{
		parser->error = 1;
		return (0);
	}
	return (1);
}

int	parser_expect(t_parser *parser, t_token_type expected)
{
	if (!parser->current_token || parser->current_token->type != expected)
	{
		parser->error = 1;
		return (0);
	}
	return (1);
}
