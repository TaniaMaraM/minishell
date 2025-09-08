/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 09:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/08 11:32:46 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir	*redir_create(t_redir_type type, const char *file)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	redir->fd = -1;
	redir->next = NULL;
	return (redir);
}

void	redir_destroy(t_redir *redir)
{
	if (!redir)
		return ;
	if (redir->file)
		free(redir->file);
	free(redir);
}

void	redir_destroy_list(t_redir *redir_list)
{
	t_redir	*current;
	t_redir	*next;

	current = redir_list;
	while (current)
	{
		next = current->next;
		redir_destroy(current);
		current = next;
	}
}

int	cmd_add_redir(t_cmd *cmd, t_redir_type type, const char *file)
{
	t_redir	*new_redir;
	t_redir	*current;

	new_redir = redir_create(type, file);
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
