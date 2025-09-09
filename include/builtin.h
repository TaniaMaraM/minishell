/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 08:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/09 19:49:10 by rwrobles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

/* Forward declaration */
typedef struct s_shell	t_shell;
typedef struct s_cmd	t_cmd;

/* Built-in detection and execution */
int						is_builtin(const char *command);
int						execute_builtin_in_child(t_cmd *cmd, t_shell *shell);

/* Built-in command implementations */
int						builtin_echo(char **argv, t_shell *shell);
int						builtin_cd(char **argv, t_shell *shell);
int						builtin_pwd(char **argv, t_shell *shell);
int						builtin_export(char **argv, t_shell *shell);
int						builtin_unset(char **argv, t_shell *shell);
int						builtin_env(char **argv, t_shell *shell);
int						builtin_exit(char **argv, t_shell *shell);

/* Environment utilities */
int						env_set_var(t_shell *shell, const char *name,
							const char *value);
int						env_unset_var(t_shell *shell, const char *name);
int						is_valid_var_name(const char *name);

/* Environment helpers */
char					*create_env_var(const char *name, const char *value);
int						update_existing_var(t_shell *shell, const char *name,
							char *new_var);
int						find_var_index(t_shell *shell, const char *name);
int						copy_env_without_var(t_shell *shell, const char *name,
							char **new_envp);

/* Export helpers */
void					print_exported_vars(t_shell *shell);
char					*extract_var_name(const char *arg);
char					*extract_var_value(const char *arg);
int						validate_and_set_var(t_shell *shell, char *name,
							char *value);
int						export_var(t_shell *shell, const char *arg);

#endif
