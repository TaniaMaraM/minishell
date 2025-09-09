/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/09 19:34:10 by rwrobles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command_list(t_cmd *cmd_list, t_shell *shell)
{
	t_cmd	*current;
	int		status;

	if (!cmd_list || !shell)
		return (1);
	current = cmd_list;
	if (current->next)
	{
		if (current->argv && ft_strcmp(current->argv[0], "cd") == 0
			&& !current->next)
			status = builtin_cd(current->argv, shell);
		else
			status = execute_pipeline(cmd_list, shell);
	}
	else
		status = execute_single_command(current, shell);
	if (shell)
		shell->last_status = status;
	return (status);
}

void	execute_child_command(t_cmd *cmd, t_shell *shell)
{
	if (setup_redirections(cmd->redirs))
		exit(1);
	if (is_builtin(cmd->argv[0]))
		exit(execute_builtin_in_child(cmd, shell));
	else
		exit(execute_external_in_child(cmd, shell));
	exit(1);
}
