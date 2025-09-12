/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 19:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/13 00:45:13 by rwrobles         ###   ########.fr       */
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

typedef struct s_pipeline_exec
{
	int		*prev_read_fd;
	int		*pipe_fds;
	pid_t	*last_pid;
}	t_pipeline_exec;

static int	setup_pipeline_execution(t_cmd *cmd_list, t_shell *shell,
	t_pipeline_exec *exec_params)
{
	t_cmd	*current;
	pid_t	pid;

	current = cmd_list;
	*(exec_params->prev_read_fd) = -1;
	while (current)
	{
		pid = setup_pipeline_process(current,
				exec_params->pipe_fds, *(exec_params->prev_read_fd), shell);
		if (pid < 0)
		{
			cleanup_pipeline_heredoc_fds(cmd_list);
			return (1);
		}
		*(exec_params->last_pid) = pid;
		execute_pipeline_parent(&current,
			exec_params->pipe_fds, exec_params->prev_read_fd);
	}
	return (0);
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
	int				pipe_fds[2];
	int				prev_read_fd;
	int				result;
	pid_t			last_pid;
	t_pipeline_exec	exec_params;

	if (!cmd_list || !shell)
		return (1);
	result = process_pipeline_heredocs(cmd_list, shell);
	if (result)
	{
		cleanup_pipeline_heredoc_fds(cmd_list);
		return (result);
	}
	exec_params.prev_read_fd = &prev_read_fd;
	exec_params.pipe_fds = pipe_fds;
	exec_params.last_pid = &last_pid;
	if (setup_pipeline_execution(cmd_list, shell, &exec_params))
		return (1);
	result = wait_for_children(last_pid);
	cleanup_pipeline_heredoc_fds(cmd_list);
	return (result);
}
