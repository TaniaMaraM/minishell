/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 08:30:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/12 19:30:40 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

/* Forward declaration */
typedef struct s_shell	t_shell;

/* Executor function prototypes */
int		execute_command_list(t_cmd *cmd_list, t_shell *shell);
int		execute_single_command(t_cmd *cmd, t_shell *shell);
int		execute_pipeline(t_cmd *cmd_list, t_shell *shell);

/* Built-in command functions (declared in builtin.h) */

/* Command execution */
void	execute_child_command(t_cmd *cmd, t_shell *shell);
int		execute_external_in_child(t_cmd *cmd, t_shell *shell);
char	*find_command_path(const char *command, t_shell *shell);
int		check_parent_builtin(char *cmd_name);
int		execute_builtin_with_redirections(t_cmd *cmd, t_shell *shell);
int		execute_parent_builtin(t_cmd *cmd, t_shell *shell);

/* Redirection handling */
int		process_heredocs(t_redir *redirs, t_shell *shell);
int		setup_redirections(t_redir *redirs);
int		handle_input_redirection(const char *file);
int		handle_heredoc_redirection(const char *delimiter);

/* Pipeline helpers */
int		execute_pipeline_child(t_cmd *cmd, int *pipe_fds,
			int prev_read_fd, t_shell *shell);
int		execute_pipeline_parent(t_cmd **current, int *pipe_fds,
			int *prev_read_fd);

/* External execution helpers */
char	*check_absolute_path(const char *command);
char	*search_path_dirs(const char *command, char **path_dirs);

/* Redirection helpers */
int		handle_output_redirection(const char *file);
int		handle_append_redirection(const char *file);
int		handle_heredoc_redirection(const char *delimiter);

/* Heredoc helpers */
int		process_heredoc_line(char *line, const char *delimiter, int write_fd);
int		handle_heredoc_signal(int *pipe_fds);
void	setup_heredoc_signals(void);
int		setup_heredoc_pipe(int *pipe_fds, int *stdin_backup);
int		cleanup_heredoc(int *pipe_fds, int stdin_backup);

/* New heredoc build functions */
int		build_heredoc_fd(t_redir *r, t_shell *shell);
int		process_heredoc_line_with_expand(char *line, t_redir *r,
			int fd, t_shell *shell);
int		write_full(int fd, const char *buf, size_t len);

/* Pipeline handling */
int		setup_pipeline(t_cmd *cmd, int *pipe_fds, int prev_read_fd);
int		cleanup_pipeline(int *pipe_fds, int prev_read_fd);
int		setup_pipeline_process(t_cmd *current, int pipe_fds[2],
			int prev_read_fd, t_shell *shell, pid_t *pid_out);
int		process_pipeline_heredocs(t_cmd *cmd_list, t_shell *shell);

/* Error handling */
void	print_command_error(const char *command, const char *error);
int		handle_execution_error(int error_code, const char *command);

/* Utility functions */
int		is_executable(const char *path);
int		wait_for_child(pid_t pid);
void	set_exit_status(t_shell *shell, int status);

/* Cleanup functions */
void	cleanup_heredoc_fds(t_cmd *cmd);
void	cleanup_pipeline_heredoc_fds(t_cmd *cmd_list);

#endif