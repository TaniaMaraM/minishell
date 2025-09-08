/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 21:00:32 by tmarcos          ###   ########.fr       */
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

int	execute_single_command(t_cmd *cmd, t_shell *shell);

static int	setup_pipeline_process(t_cmd *current, int pipe_fds[2],
	int prev_read_fd, t_shell *shell)
{
	pid_t	pid;

	if (current->next && pipe(pipe_fds) == -1)
		return (1);
	pid = fork();
	if (pid == 0)
		execute_pipeline_child(current, pipe_fds, prev_read_fd, shell);
	else if (pid < 0)
		return (1);
	return (0);
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

int	execute_pipeline(t_cmd *cmd_list, t_shell *shell)
{
	t_cmd	*current;
	int		pipe_fds[2];
	int		prev_read_fd;
	int		status;
	int		last_status;

	if (!cmd_list || !shell)
		return (1);
	current = cmd_list;
	prev_read_fd = -1;
	while (current)
	{
		if (setup_pipeline_process(current, pipe_fds, prev_read_fd, shell))
			return (1);
		execute_pipeline_parent(&current, pipe_fds, &prev_read_fd);
	}
	last_status = 0;
	while (wait(&status) > 0)
		last_status = get_child_exit_status(status);
	return (last_status);
}
