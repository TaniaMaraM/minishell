/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 19:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/09 19:00:00 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_child_exit_status(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		return (128 + WTERMSIG(status));
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

int	execute_pipeline(t_cmd *cmd_list, t_shell *shell)
{
	int		pipe_fds[2];
	int		prev_read_fd;
	int		status;
	int		last_status;
	int		result;

	if (!cmd_list || !shell)
		return (1);
	result = process_pipeline_heredocs(cmd_list);
	if (result)
	{
		cleanup_pipeline_heredoc_fds(cmd_list);
		return (result);
	}
	if (setup_pipeline_execution(cmd_list, shell, &prev_read_fd, pipe_fds))
		return (1);
	last_status = 0;
	while (wait(&status) > 0)
		last_status = get_child_exit_status(status);
	cleanup_pipeline_heredoc_fds(cmd_list);
	return (last_status);
}
