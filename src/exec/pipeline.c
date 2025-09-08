/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:15:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/03 16:20:00 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipeline(t_cmd *cmd, int *pipe_fds, int prev_read_fd)
{
	if (!cmd)
		return (1);
	if (prev_read_fd != -1)
	{
		if (dup2(prev_read_fd, STDIN_FILENO) == -1)
			return (1);
		close(prev_read_fd);
	}
	if (pipe_fds[1] != -1)
	{
		if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)
			return (1);
		close(pipe_fds[1]);
	}
	return (0);
}

int	cleanup_pipeline(int *pipe_fds, int prev_read_fd)
{
	if (prev_read_fd != -1)
		close(prev_read_fd);
	if (pipe_fds[1] != -1)
		close(pipe_fds[1]);
	return (0);
}
