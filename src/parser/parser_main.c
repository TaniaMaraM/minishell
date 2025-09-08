/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 09:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/08 11:31:51 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parser_parse(t_parser *parser)
{
	if (!parser_advance(parser))
		return (NULL);
	if (!parser_parse_pipeline(parser))
		return (NULL);
	return (parser->cmd_list);
}

int	parser_parse_pipeline(t_parser *parser)
{
	t_cmd	*cmd;
	t_cmd	*last_cmd;

	cmd = parser_parse_command(parser);
	if (!cmd || parser->error)
		return (0);
	parser->cmd_list = cmd;
	last_cmd = cmd;
	while (parser->current_token && parser->current_token->type == TOKEN_PIPE)
	{
		if (!parser_advance(parser))
			return (0);
		cmd = parser_parse_command(parser);
		if (!cmd || parser->error)
			return (0);
		last_cmd->next = cmd;
		cmd->prev = last_cmd;
		last_cmd = cmd;
	}
	return (1);
}

t_cmd	*parser_parse_command(t_parser *parser)
{
	t_cmd	*cmd;

	cmd = cmd_create();
	if (!cmd)
		return (NULL);
	if (!parser_parse_arguments(parser, cmd))
	{
		cmd_destroy(cmd);
		return (NULL);
	}
	if (!parser_parse_redirections(parser, cmd))
	{
		cmd_destroy(cmd);
		return (NULL);
	}
	return (cmd);
}
