/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/09 18:26:12 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_redirection(t_redir *redir)
{
	if (redir->type == REDIR_IN)
		return (handle_input_redirection(redir->file));
	else if (redir->type == REDIR_OUT)
		return (handle_output_redirection(redir->file));
	else if (redir->type == REDIR_APPEND)
		return (handle_append_redirection(redir->file));
	else if (redir->type == REDIR_HEREDOC)
		return (handle_heredoc_redirection(redir->file));
	return (0);
}

int	process_heredocs(t_redir *redirs)
{
	t_redir	*current;
	t_shell	*shell;

	if (!redirs)
		return (0);
	shell = NULL;
	current = redirs;
	while (current)
	{
		if (current->type == REDIR_HEREDOC)
		{
			current->expand = 1;
			if (build_heredoc_fd(current, shell) == -1)
			{
				if (g_signal == SIGINT)
					return (130);
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}

int	setup_redirections(t_redir *redirs)
{
	t_redir	*current;

	if (!redirs)
		return (0);
	current = redirs;
	while (current)
	{
		if (current->type == REDIR_HEREDOC)
		{
			if (current->fd < 0)
				return (perror("heredoc fd invalid"), 1);
			if (dup2(current->fd, STDIN_FILENO) == -1)
				return (perror("dup2 heredoc"), 1);
			close(current->fd);
			current->fd = -1;
		}
		else
		{
			if (process_redirection(current))
				return (1);
		}
		current = current->next;
	}
	return (0);
}
