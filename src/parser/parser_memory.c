/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_memory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 09:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/08 11:32:02 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parser	*parser_alloc(void)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->lexer = NULL;
	parser->current_token = NULL;
	parser->cmd_list = NULL;
	parser->error = 0;
	return (parser);
}

t_cmd	*cmd_alloc(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}

t_redir	*redir_alloc(t_redir_type type, const char *file)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->fd = -1;
	redir->next = NULL;
	return (redir);
}

void	parser_set_error(t_parser *parser)
{
	if (parser)
		parser->error = 1;
}
