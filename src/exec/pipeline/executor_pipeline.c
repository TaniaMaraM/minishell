/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 19:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/13 14:19:15 by rwrobles         ###   ########.fr       */
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

static pid_t	setup_pipeline_execution(t_cmd *cmd_list, t_shell *shell,
	int *prev_read_fd, int *pipe_fds)
{
	t_cmd	*current;
	pid_t	pid;
	pid_t	last_pid;

	current = cmd_list;
	*prev_read_fd = -1;
	last_pid = -1;
	while (current)
	{
		pid = setup_pipeline_process(current, pipe_fds, *prev_read_fd, shell);
		if (pid < 0)
		{
			cleanup_pipeline_heredoc_fds(cmd_list);
			return (-1);
		}
		last_pid = pid;
		execute_pipeline_parent(&current, pipe_fds, prev_read_fd);
	}
	return (last_pid);
}

static int	wait_for_children(pid_t last_pid)
{
	int	status;
	int	last_status;

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
	int		prev_read_fd;
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
	last_pid = setup_pipeline_execution(cmd_list, shell, 
			&prev_read_fd, pipe_fds);
	if (last_pid == -1)
		return (1);
	last_status = wait_for_children(last_pid);
	cleanup_pipeline_heredoc_fds(cmd_list);
	return (last_status);
}
