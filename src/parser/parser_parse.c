/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 09:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/12 18:00:35 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_redir_token(t_parser *parser, t_redir_type *type)
{
	if (!parser->current_token
		|| !parser_is_redir_token(parser->current_token->type))
		return (0);
	*type = parser_token_to_redir_type(parser->current_token->type);
	return (parser_advance(parser));
}

static int	process_redir_filename(t_parser *parser, t_cmd *cmd,
		t_redir_type type)
{
	if (!parser->current_token || parser->current_token->type != TOKEN_WORD)
	{
		parser->error = 1;
		return (0);
	}
	if (expand_token(parser->current_token, parser->shell))
	{
		parser->error = 1;
		return (0);
	}
	if (!cmd_add_redir_with_quote(cmd, type, parser->current_token->value,
			parser->current_token->quote_state))
	{
		parser->error = 1;
		return (0);
	}
	return (parser_advance(parser));
}

int	parser_parse_single_redir(t_parser *parser, t_cmd *cmd)
{
	t_redir_type	type;

	if (!validate_redir_token(parser, &type))
		return (0);
	return (process_redir_filename(parser, cmd, type));
}

int	parser_parse_single_arg(t_parser *parser, t_cmd *cmd)
{
	char	*processed_arg;

	if (!parser->current_token || parser->current_token->type != TOKEN_WORD)
		return (0);
	processed_arg = process_single_argument(parser);
	if (!processed_arg)
		return (0);
	if (!cmd_add_arg(cmd, processed_arg))
	{
		free(processed_arg);
		parser->error = 1;
		return (0);
	}
	free(processed_arg);
	return (1);
}

int	parser_parse_redirections(t_parser *parser, t_cmd *cmd)
{
	while (parser->current_token
		&& parser_is_redir_token(parser->current_token->type))
	{
		if (!parser_parse_single_redir(parser, cmd))
			return (0);
	}
	return (1);
}
