/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 18:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/02 20:36:41 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin_in_child(t_cmd *cmd, t_shell *shell)
{
	if (!cmd || !cmd->argv || !cmd->argv[0] || !shell)
		return (1);
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (builtin_echo(cmd->argv, shell));
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (builtin_cd(cmd->argv, shell));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (builtin_pwd(cmd->argv, shell));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (builtin_export(cmd->argv, shell));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (builtin_unset(cmd->argv, shell));
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (builtin_env(cmd->argv, shell));
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (builtin_exit(cmd->argv, shell));
	return (1);
}
