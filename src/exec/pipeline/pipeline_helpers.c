/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/15 14:48:43 by rwrobles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_pipeline_child(t_cmd *cmd, int *pipe_fds,
		int prev_read_fd, t_shell *shell)
{
	int	exit_code;

	setup_pipeline(cmd, pipe_fds, prev_read_fd);
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

int	execute_pipeline_parent(t_cmd **current, int *pipe_fds,
		int *prev_read_fd)
{
	cleanup_pipeline(pipe_fds, *prev_read_fd);
	if ((*current)->next)
		*prev_read_fd = pipe_fds[0];
	*current = (*current)->next;
	return (0);
}
