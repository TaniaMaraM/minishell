/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 17:20:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/09 19:41:26 by rwrobles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

volatile sig_atomic_t	g_signal = 0;

void	signal_setup_interactive(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	signal_setup_non_interactive(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	signal_restore_defaults(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = sig;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	signal_handler_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = sig;
		write(STDOUT_FILENO, "\n", 1);
		close(STDIN_FILENO);
	}
}
