/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/10 14:25:55 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_heredoc_input(const char *delimiter, int *pipe_fds,
		int stdin_backup)
{
	char	*line;

	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line || g_signal == SIGINT)
		{
			if (handle_heredoc_signal(pipe_fds))
			{
				dup2(stdin_backup, STDIN_FILENO);
				close(stdin_backup);
				signal_setup_interactive();
				return (EXIT_STATUS_SIGINT);
			}
			break ;
		}
		if (!process_heredoc_line(line, delimiter, pipe_fds[1]))
			break ;
	}
	return (0);
}

int	handle_heredoc_redirection(const char *delimiter)
{
	int	pipe_fds[2];
	int	stdin_backup;
	int	result;

	if (!delimiter || setup_heredoc_pipe(pipe_fds, &stdin_backup))
		return (1);
	result = process_heredoc_input(delimiter, pipe_fds, stdin_backup);
	if (result)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
		return (result);
	}
	signal_setup_interactive();
	return (cleanup_heredoc(pipe_fds, stdin_backup));
}
