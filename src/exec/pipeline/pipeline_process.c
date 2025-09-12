/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 20:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/10 14:25:55 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipeline_process(t_cmd *current, int pipe_fds[2],
	int prev_read_fd, t_shell *shell)
{
	pid_t	pid;

	if (current->next && pipe(pipe_fds) == -1)
		return (1);
	pid = fork();
	if (pid == 0)
		execute_pipeline_child(current, pipe_fds, prev_read_fd, shell);
	else if (pid < 0)
		return (1);
	return (0);
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
