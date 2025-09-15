/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 09:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/15 14:31:56 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir	*init_redir(t_redir_type type, const char *file)
{
	t_redir	*redir;

	redir = ft_calloc(1, sizeof(t_redir));
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
	redir->expand = 1;
	redir->next = NULL;
	return (redir);
}

static t_redir	*redir_create_with_quote(t_redir_type type, const char *file,
			t_quote_state quote_state)
{
	t_redir	*redir;

	redir = init_redir(type, file);
	if (!redir)
		return (NULL);
	if (type == REDIR_HEREDOC)
		redir->expand = (quote_state == QUOTE_NONE);
	return (redir);
}

int	cmd_add_redir(t_cmd *cmd, t_redir_type type, const char *file)
{
	t_redir	*new_redir;
	t_redir	*current;

	new_redir = init_redir(type, file);
	if (!new_redir)
		return (0);
	if (!cmd->redirs)
	{
		cmd->redirs = new_redir;
		return (1);
	}
	current = cmd->redirs;
	while (current->next)
		current = current->next;
	current->next = new_redir;
	return (1);
}

int	cmd_add_redir_with_quote(t_cmd *cmd, t_redir_type type, const char *file,
		t_quote_state quote_state)
{
	t_redir	*new_redir;
	t_redir	*current;

	new_redir = redir_create_with_quote(type, file, quote_state);
	if (!new_redir)
		return (0);
	if (!cmd->redirs)
	{
		cmd->redirs = new_redir;
		return (1);
	}
	current = cmd->redirs;
	while (current->next)
		current = current->next;
	current->next = new_redir;
	return (1);
}
