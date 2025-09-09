/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:15:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/09 12:23:00 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input_redirection(const char *file)
{
	int	fd;

	if (!file)
		return (1);
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		print_error(file, strerror(errno));
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	handle_output_redirection(const char *file)
{
	int	fd;

	if (!file)
		return (1);
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		print_error(file, strerror(errno));
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_append_redirection(const char *file)
{
	int	fd;

	if (!file)
		return (1);
	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		print_error(file, strerror(errno));
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}
