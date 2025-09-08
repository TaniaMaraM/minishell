/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 09:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/08 21:30:02 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parser_parse_redirections(t_parser *parser, t_cmd *cmd)
{
	t_redir_type	type;

	while (parser->current_token
		&& parser_is_redir_token(parser->current_token->type))
	{
		type = parser_token_to_redir_type(parser->current_token->type);
		if (!parser_advance(parser))
			return (0);
		if (!parser->current_token || parser->current_token->type != TOKEN_WORD)
		{
			parser->error = 1;
			return (0);
		}
		if (!cmd_add_redir(cmd, type, parser->current_token->value))
		{
			parser->error = 1;
			return (0);
		}
		if (!parser_advance(parser))
			return (0);
	}
	return (1);
}
