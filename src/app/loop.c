/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/09 19:22:54 by rwrobles         ###   ########.fr       */
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
static void	restore_stdin_for_readline(t_shell *sh)
{
	if (dup2(sh->stdin_backup, STDIN_FILENO) == -1)
		perror("dup2 to original stdin");
}

static int	handle_empty_or_signal(t_shell *sh, char *line)
{
	if (handle_signal_interrupt(sh, line))
		return (1);
	if (*line == '\0')
	{
		free(line);
		return (1);
	}
	return (0);
}

int	shell_loop(t_shell *sh)
{
	char	*line;

	while (!sh->should_exit)
	{
		g_signal = 0;
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
			continue ;
		add_history(line);
		process_line(line, sh);
		free(line);
	}
	return (sh->last_status);
}
