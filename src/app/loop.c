/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/16 20:58:07 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_empty_or_whitespace(const char *line)
{
	int	i;

	if (!line || *line == '\0')
		return (1);
	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

static int	handle_signal_interrupt(t_shell *sh, char *line)
{
	if (g_signal == SIGINT)
	{
		sh->last_status = EXIT_STATUS_SIGINT;
		g_signal = 0;
		if (is_empty_or_whitespace(line))
		{
			if (line)
				free(line);
			return (1);
		}
	}
	return (0);
}

static void	restore_stdin_for_readline(t_shell *sh)
{
	if (dup2(sh->stdin_backup, STDIN_FILENO) == -1)
		perror("dup2 to original stdin");
}

static int	handle_empty_or_signal(t_shell *sh, char *line)
{
	if (handle_signal_interrupt(sh, line))
		return (1);
	if (is_empty_or_whitespace(line))
	{
		free(line);
		return (1);
	}
	return (0);
}

/**
 * @brief Main shell loop - reads input, processes and executes commands
 * @param sh Shell context with state and environment
 * @return Final exit status of the shell
 */
int	shell_loop(t_shell *sh)
{
	char	*line;

	while (!sh->should_exit)
	{
		restore_stdin_for_readline(sh);
		line = read_command_line();
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "exit\n", 5);
			sh->should_exit = 1;
			break ;
		}
		if (handle_empty_or_signal(sh, line))
		{
			continue ;
		}
		g_signal = 0;
		if (sh->is_interactive)
			add_history(line);
		process_line(line, sh);
		free(line);
	}
	return (sh->last_status);
}
