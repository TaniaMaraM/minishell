/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:30:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/12 17:38:09 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_delimiter(char *line, char *delimiter)
{
	return (ft_strcmp(line, delimiter) == 0);
}

static char	*prompt_heredoc_line(void)
{
	char	*line;

	line = readline("> ");
	if (!line || g_signal == SIGINT)
		return (NULL);
	return (line);
}


static int	handle_null_line(int *fds)
{
	if (g_signal == SIGINT)
	{
		close(fds[0]);
		close(fds[1]);
		return (-1);
	}
	return (0);
}

static int	process_heredoc_content(char *line, t_redir *r,
		int *fds, t_shell *shell)
{
	if (is_delimiter(line, r->file))
	{
		free(line);
		return (1);
	}
	if (process_heredoc_line_with_expand(line, r, fds[1], shell) == -1)
	{
		close(fds[0]);
		close(fds[1]);
		return (-1);
	}
	return (0);
}

static int	handle_heredoc_input(t_redir *r, int *fds, t_shell *shell)
{
	char	*line;
	int		result;

	while (1)
	{
		line = prompt_heredoc_line();
		if (!line)
			return (handle_null_line(fds));
		result = process_heredoc_content(line, r, fds, shell);
		if (result != 0)
			return (result == 1 ? 0 : result);
	}
	return (0);
}

int	build_heredoc_fd(t_redir *r, t_shell *shell)
{
	int	fds[2];
	int	result;

	if (pipe(fds) == -1)
		return (perror("pipe"), -1);
	setup_heredoc_signals();
	result = handle_heredoc_input(r, fds, shell);
	if (result == -1)
		return (-1);
	close(fds[1]);
	r->fd = fds[0];
	signal_setup_interactive();
	return (0);
}
