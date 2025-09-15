/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command_exec.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 19:10:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/15 14:48:43 by rwrobles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_child_process(t_cmd *cmd, t_shell *shell)
{
	int	exit_code;

	signal_setup_non_interactive();
	if (setup_redirections(cmd->redirs))
	{
		shell_cleanup(shell);
		exit(1);
	}
	if (is_builtin(cmd->argv[0]))
		exit_code = execute_builtin_in_child(cmd, shell);
	else
		exit_code = execute_external_in_child(cmd, shell);
	shell_cleanup(shell);
	exit(exit_code);
}

static int	get_child_exit_status(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		return (EXIT_STATUS_SIGNAL_BASE + WTERMSIG(status));
	}
	return (WEXITSTATUS(status));
}

static int	handle_parent_process(t_cmd *cmd, pid_t pid)
{
	int	status;

	if (pid > 0)
	{
		waitpid(pid, &status, 0);
		cleanup_heredoc_fds(cmd);
		return (get_child_exit_status(status));
	}
	cleanup_heredoc_fds(cmd);
	print_error("fork", strerror(errno));
	return (1);
}

/* Function declarations from single_command.c */
int	check_parent_builtin(char *cmd_name);
int	execute_builtin_with_redirections(t_cmd *cmd, t_shell *shell);

int	execute_single_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		result;

	if (!cmd || !shell || !cmd->argv || !cmd->argv[0])
		return (1);
	if (process_heredocs(cmd->redirs, shell))
		return (EXIT_STATUS_SIGINT);
	if (g_signal == SIGINT)
	{
		cleanup_heredoc_fds(cmd);
		return (EXIT_STATUS_SIGINT);
	}
	if (check_parent_builtin(cmd->argv[0]))
	{
		result = execute_builtin_with_redirections(cmd, shell);
		cleanup_heredoc_fds(cmd);
		return (result);
	}
	pid = fork();
	if (pid == 0)
		return (handle_child_process(cmd, shell));
	return (handle_parent_process(cmd, pid));
}
