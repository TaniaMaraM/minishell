/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 20:10:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/09 18:36:10 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_parent_builtin(t_cmd *cmd, t_shell *shell)
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

static void	restore_std_fds(int stdin_fd, int stdout_fd, int stderr_fd)
{
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	dup2(stderr_fd, STDERR_FILENO);
	close(stdin_fd);
	close(stdout_fd);
	close(stderr_fd);
}

int	execute_builtin_with_redirections(t_cmd *cmd, t_shell *shell)
{
	int	stdin_backup;
	int	stdout_backup;
	int	stderr_backup;
	int	result;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	stderr_backup = dup(STDERR_FILENO);
	if (setup_redirections(cmd->redirs))
	{
		restore_std_fds(stdin_backup, stdout_backup, stderr_backup);
		return (1);
	}
	result = execute_parent_builtin(cmd, shell);
	restore_std_fds(stdin_backup, stdout_backup, stderr_backup);
	return (result);
}

int	check_parent_builtin(char *cmd_name)
{
	return (ft_strcmp(cmd_name, "exit") == 0
		|| ft_strcmp(cmd_name, "cd") == 0
		|| ft_strcmp(cmd_name, "export") == 0
		|| ft_strcmp(cmd_name, "unset") == 0);
}