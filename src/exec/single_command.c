/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 20:10:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 20:41:00 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_parent_builtin(t_cmd *cmd, t_shell *shell)
{
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (builtin_exit(cmd->argv, shell));
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (builtin_cd(cmd->argv, shell));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (builtin_export(cmd->argv, shell));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (builtin_unset(cmd->argv, shell));
	return (0);
}

static int	check_parent_builtin(char *cmd_name)
{
	return (ft_strcmp(cmd_name, "exit") == 0
		|| ft_strcmp(cmd_name, "cd") == 0
		|| ft_strcmp(cmd_name, "export") == 0
		|| ft_strcmp(cmd_name, "unset") == 0);
}

static int	handle_child_process(t_cmd *cmd, t_shell *shell)
{
	execute_child_command(cmd, shell);
	return (1);
}

static int	get_child_exit_status(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		return (128 + WTERMSIG(status));
	}
	return (WEXITSTATUS(status));
}

int	execute_single_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!cmd || !shell || !cmd->argv || !cmd->argv[0])
		return (1);
	if (check_parent_builtin(cmd->argv[0]))
		return (execute_parent_builtin(cmd, shell));
	pid = fork();
	if (pid == 0)
		return (handle_child_process(cmd, shell));
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		return (get_child_exit_status(status));
	}
	else
	{
		print_error("fork", strerror(errno));
		return (1);
	}
}
