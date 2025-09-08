/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 11:21:58 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_pipeline_child(t_cmd *cmd, int *pipe_fds,
		int prev_read_fd, t_shell *shell)
{
	setup_pipeline(cmd, pipe_fds, prev_read_fd);
	if (setup_redirections(cmd->redirs))
		exit(1);
	if (is_builtin(cmd->argv[0]))
		exit(execute_builtin_in_child(cmd, shell));
	else
		exit(execute_external_in_child(cmd, shell));
}

int	execute_pipeline_parent(t_cmd **current, int *pipe_fds,
		int *prev_read_fd)
{
	cleanup_pipeline(pipe_fds, *prev_read_fd);
	if ((*current)->next)
		*prev_read_fd = pipe_fds[0];
	*current = (*current)->next;
	return (0);
}
