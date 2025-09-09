/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_heredoc_fds.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:45:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/09 18:26:12 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Close any open heredoc file descriptors in a command
 * @param cmd Command structure that may contain heredoc redirections
 */
void	cleanup_heredoc_fds(t_cmd *cmd)
{
	t_redir	*current;

	if (!cmd || !cmd->redirs)
		return ;
	current = cmd->redirs;
	while (current)
	{
		if (current->type == REDIR_HEREDOC && current->fd >= 0)
		{
			close(current->fd);
			current->fd = -1;
		}
		current = current->next;
	}
}

/**
 * @brief Close any open heredoc file descriptors in a pipeline
 * @param cmd_list List of commands in the pipeline
 */
void	cleanup_pipeline_heredoc_fds(t_cmd *cmd_list)
{
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		cleanup_heredoc_fds(current);
		current = current->next;
	}
}
