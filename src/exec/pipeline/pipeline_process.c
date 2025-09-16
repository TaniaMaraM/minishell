/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 20:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/15 21:54:22 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static pid_t	create_pipeline_process(t_cmd *current, int *pipe_fds,
	int prev_read_fd, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execute_pipeline_child(current, pipe_fds, prev_read_fd, shell);
	return (pid);
}

pid_t	setup_pipeline_process(t_cmd *current, int pipe_fds[2],
	int prev_read_fd, t_shell *shell)
{
	pid_t	pid;

	if (current->next)
	{
		if (pipe(pipe_fds) == -1)
			return (-1);
	}
	else
	{
		pipe_fds[0] = -1;
		pipe_fds[1] = -1;
	}
	pid = create_pipeline_process(current, pipe_fds, prev_read_fd, shell);
	if (pid < 0)
	{
		if (current->next)
		{
			close(pipe_fds[0]);
			close(pipe_fds[1]);
		}
		return (-1);
	}
	return (pid);
}

int	process_pipeline_heredocs(t_cmd *cmd_list, t_shell *shell)
{
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		if (process_heredocs(current->redirs, shell))
			return (EXIT_STATUS_SIGINT);
		if (g_signal == SIGINT)
			return (EXIT_STATUS_SIGINT);
		current = current->next;
	}
	return (0);
}
