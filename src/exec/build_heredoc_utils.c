/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_heredoc_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:35:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/09 18:48:01 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	write_full(int fd, const char *buf, size_t len)
{
	ssize_t	n;
	size_t	off;

	off = 0;
	while (off < len)
	{
		n = write(fd, buf + off, len - off);
		if (n < 0)
			return (-1);
		off += (size_t)n;
	}
	return (0);
}

int	process_heredoc_line_with_expand(char *line, t_redir *r,
	int fd, t_shell *shell)
{
	char	*expanded;

	if (r->expand && shell)
		expanded = expand_string(line, shell, QUOTE_NONE);
	else
		expanded = line;
	if (write_full(fd, expanded, ft_strlen(expanded)) == -1
		|| write_full(fd, "\n", 1) == -1)
	{
		if (expanded != line)
			free(expanded);
		free(line);
		return (-1);
	}
	if (expanded != line)
		free(expanded);
	free(line);
	return (0);
}
