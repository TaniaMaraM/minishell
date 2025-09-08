/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 11:30:00 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_signal_interrupt(t_shell *sh, char *line)
{
	if (g_signal == SIGINT)
	{
		sh->last_status = 130;
		g_signal = 0;
		if (line)
			free(line);
		return (1);
	}
	return (0);
}

/**
 * @brief Main shell loop - read input, process and execute commands
 * @param sh Shell context
 * @return Final exit status of the shell
 */
int	shell_loop(t_shell *sh)
{
	char	*line;

	while (!sh->should_exit)
	{
		g_signal = 0;
		line = read_command_line();
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "exit\n", 5);
			sh->should_exit = 1;
			break ;
		}
		if (handle_signal_interrupt(sh, line))
			continue ;
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		process_line(line, sh);
		free(line);
	}
	return (sh->last_status);
}
