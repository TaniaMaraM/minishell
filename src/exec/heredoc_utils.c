/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 13:40:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/09 18:30:48 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_heredoc_line(char *line, const char *delimiter, int write_fd)
{
	if (!line || g_signal == SIGINT)
		return (0);
	if (ft_strcmp(line, (char *)delimiter) == 0)
	{
		free(line);
		return (0);
	}
	ft_putendl_fd(line, write_fd);
	free(line);
	return (1);
}

int	handle_heredoc_signal(int *pipe_fds)
{
	if (g_signal == SIGINT)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return (1);
	}
	return (0);
}

void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

int	setup_heredoc_pipe(int *pipe_fds, int *stdin_backup)
{
	if (pipe(pipe_fds) == -1)
		return (1);
	*stdin_backup = dup(STDIN_FILENO);
	setup_heredoc_signals();
	g_signal = 0;
	return (0);
}

int	cleanup_heredoc(int *pipe_fds, int stdin_backup)
{
	int	tmp_fd;

	(void)stdin_backup;
	signal_setup_interactive();
	close(pipe_fds[1]);
	tmp_fd = dup(pipe_fds[0]);
	close(pipe_fds[0]);
	dup2(tmp_fd, STDIN_FILENO);
	close(tmp_fd);
	return (0);
}