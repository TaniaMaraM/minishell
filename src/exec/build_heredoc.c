/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:30:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/09 18:29:19 by tmarcos          ###   ########.fr       */
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

static int	handle_heredoc_input(t_redir *r, int *fds, t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = prompt_heredoc_line();
		if (!line)
		{
			close(fds[0]);
			close(fds[1]);
			if (g_signal == SIGINT)
				return (-1);
			break ;
		}
		if (is_delimiter(line, r->file))
		{
			free(line);
			break ;
		}
		if (process_heredoc_line_with_expand(line, r, fds[1], shell) == -1)
		{
			close(fds[0]);
			close(fds[1]);
			return (-1);
		}
	}
	return (0);
}

int	build_heredoc_fd(t_redir *r, t_shell *shell)
{
	int		fds[2];
	int		result;

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