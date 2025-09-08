/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:30:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 20:21:18 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_to_home(t_shell *shell);
int	cd_to_path(const char *path);
int	expand_tilde_with_slash(char *path, char *home);
int	handle_tilde_only(char *home);

static int	process_tilde_path(char *path, char *home)
{
	int	result;

	if (path[1] == '\0')
		result = handle_tilde_only(home);
	else if (path[1] == '/')
	{
		if (expand_tilde_with_slash(path, home))
			return (1);
		result = 0;
	}
	else
		result = cd_to_path(path);
	return (result);
}

static int	handle_tilde_path(char *path, t_shell *shell)
{
	char	*home;
	int		result;

	if (path[0] != '~')
		return (cd_to_path(path));
	home = get_env_var("HOME", shell);
	if (!home)
	{
		print_error("cd", "HOME not set");
		return (1);
	}
	result = process_tilde_path(path, home);
	free(home);
	return (result);
}

static int	update_pwd_var(t_shell *shell)
{
	char	*cwd;
	int		result;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (1);
	result = env_set_var(shell, "PWD", cwd);
	free(cwd);
	return (result);
}

int	builtin_cd(char **argv, t_shell *shell)
{
	int	result;

	if (!argv || !shell)
		return (1);
	if (!argv[1])
		result = cd_to_home(shell);
	else if (argv[2])
	{
		print_error("cd", "too many arguments");
		return (1);
	}
	else
		result = handle_tilde_path(argv[1], shell);
	if (result == 0)
		update_pwd_var(shell);
	return (result);
}
