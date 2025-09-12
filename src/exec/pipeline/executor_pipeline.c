/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 19:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/12 17:18:23 by tmarcos          ###   ########.fr       */
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

static int	setup_pipeline_execution(t_cmd *cmd_list, t_shell *shell,
	int *prev_read_fd, int *pipe_fds)
{
	t_cmd	*current;

	current = cmd_list;
	*prev_read_fd = -1;
	while (current)
	{
		if (setup_pipeline_process(current, pipe_fds, *prev_read_fd, shell))
		{
			cleanup_pipeline_heredoc_fds(cmd_list);
			return (1);
		}
		execute_pipeline_parent(&current, pipe_fds, prev_read_fd);
	}
	return (0);
}

static int	wait_for_children(void)
{
	int	status;
	int	last_status;
	int	current_status;

	last_status = EXIT_SUCCESS;
	while (wait(&status) > 0)
	{
		current_status = get_child_exit_status(status);
		if (current_status != 0)
			last_status = current_status;
	}
	return (last_status);
}

int	execute_pipeline(t_cmd *cmd_list, t_shell *shell)
{
	int		pipe_fds[2];
	int		prev_read_fd;
	int		result;
	int		last_status;

	if (!cmd_list || !shell)
		return (1);
 	result = process_pipeline_heredocs(cmd_list, shell);
	if (result)
	{
		cleanup_pipeline_heredoc_fds(cmd_list);
		return (result);
	}
	if (setup_pipeline_execution(cmd_list, shell, &prev_read_fd, pipe_fds))
		return (1);
	last_status = wait_for_children();
	cleanup_pipeline_heredoc_fds(cmd_list);
	return (last_status);
}
