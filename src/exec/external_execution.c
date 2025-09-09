/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 09:30:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/09 11:38:00 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command_path(const char *command, t_shell *shell)
{
	char	*path;
	char	**path_dirs;
	char	*result;

	if (!command || !shell)
		return (NULL);
	result = check_absolute_path(command);
	if (result)
		return (result);
	path = get_env_var("PATH", shell);
	if (!path)
		return (NULL);
	path_dirs = ft_split(path, ':');
	free(path);
	if (!path_dirs)
		return (NULL);
	return (search_path_dirs(command, path_dirs));
}

int	execute_external_in_child(t_cmd *cmd, t_shell *shell)
{
	char	*command_path;

	if (!cmd || !cmd->argv || !cmd->argv[0] || !shell)
		return (1);
	command_path = find_command_path(cmd->argv[0], shell);
	if (!command_path)
	{
		if (ft_strchr(cmd->argv[0], '/') && access(cmd->argv[0], F_OK) == -1)
			print_command_error(cmd->argv[0], "No such file or directory");
		else
			print_command_error(cmd->argv[0], "command not found");
		return (127);
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(command_path, cmd->argv, shell->envp);
	print_error("execve", strerror(errno));
	free(command_path);
	return (126);
}
