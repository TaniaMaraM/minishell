/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 08:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/08 20:52:40 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

/* Forward declaration */
typedef struct s_shell t_shell;

/* Executor function prototypes */
int		execute_command_list(t_cmd *cmd_list, t_shell *shell);
int		execute_single_command(t_cmd *cmd, t_shell *shell);
int		execute_pipeline(t_cmd *cmd_list, t_shell *shell);

/* Built-in command functions (declared in builtin.h) */

/* Command execution */
void	execute_child_command(t_cmd *cmd, t_shell *shell);
int		execute_external_in_child(t_cmd *cmd, t_shell *shell);
char	*find_command_path(const char *command, t_shell *shell);

/* Redirection handling */
int		setup_redirections(t_redir *redirs);
int		handle_input_redirection(const char *file);

/* Pipeline helpers */
int		execute_pipeline_child(t_cmd *cmd, int *pipe_fds,
			int prev_read_fd, t_shell *shell);
int		execute_pipeline_parent(t_cmd **current, int *pipe_fds,
			int *prev_read_fd);

/* External execution helpers */
char	*check_absolute_path(const char *command);
char	*search_path_dirs(const char *command, char **path_dirs);

/* Redirection helpers - now static functions in redirections.c */
int		handle_output_redirection(const char *file);
int		handle_append_redirection(const char *file);
int		handle_heredoc_redirection(const char *delimiter);

/* Pipeline handling */
int		setup_pipeline(t_cmd *cmd, int *pipe_fds, int prev_read_fd);
int		cleanup_pipeline(int *pipe_fds, int prev_read_fd);

/* Error handling */
void	print_command_error(const char *command, const char *error);
int		handle_execution_error(int error_code, const char *command);

/* Utility functions */
int		is_executable(const char *path);
int		wait_for_child(pid_t pid);
void	set_exit_status(t_shell *shell, int status);

#endif
