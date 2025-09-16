/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 19:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/16 15:36:56 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_child_exit_status(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		return (EXIT_STATUS_SIGNAL_BASE + WTERMSIG(status));
	}
	return (WEXITSTATUS(status));
}

static int	process_single_pipeline_cmd(t_cmd **current, t_shell *shell,
	int *pipe_fds, int *prev_read_fd)
{
	pid_t	pid;

	pid = setup_pipeline_process(*current, pipe_fds, *prev_read_fd, shell);
	if (pid < 0)
		return (-1);
	execute_pipeline_parent(current, pipe_fds, prev_read_fd);
	return (pid);
}

static int	setup_pipeline_execution(t_cmd *cmd_list, t_shell *shell,
	int *pipe_fds, pid_t *last_pid)
{
	t_cmd	*current;
	int		prev_read_fd;
	int		pid;

	current = cmd_list;
	prev_read_fd = -1;
	while (current)
	{
		pid = process_single_pipeline_cmd(&current, shell,
				pipe_fds, &prev_read_fd);
		if (pid < 0)
		{
			cleanup_pipeline_heredoc_fds(cmd_list);
			return (1);
		}
		*last_pid = pid;
	}
	return (0);
}

static int	wait_for_children(pid_t last_pid)
{
	int	status;
	int	last_status;

	status = 0;
	last_status = EXIT_SUCCESS;
	if (waitpid(last_pid, &status, 0) > 0)
		last_status = get_child_exit_status(status);
	while (wait(&status) > 0)
		;
	return (last_status);
}

int	execute_pipeline(t_cmd *cmd_list, t_shell *shell)
{
	int		pipe_fds[2];
	int		result;
	int		last_status;
	pid_t	last_pid;

	if (!cmd_list || !shell)
		return (1);
	result = process_pipeline_heredocs(cmd_list, shell);
	if (result)
	{
		cleanup_pipeline_heredoc_fds(cmd_list);
		return (result);
	}
	if (setup_pipeline_execution(cmd_list, shell, pipe_fds, &last_pid))
		return (1);
	last_status = wait_for_children(last_pid);
	cleanup_pipeline_heredoc_fds(cmd_list);
	return (last_status);
}
