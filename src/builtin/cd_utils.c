/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:20:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 19:20:07 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_to_home(t_shell *shell)
{
	char	*home;

	home = get_env_var("HOME", shell);
	if (!home)
	{
		print_error("cd", "HOME not set");
		return (1);
	}
	if (chdir(home) == -1)
	{
		print_error("cd", strerror(errno));
		free(home);
		return (1);
	}
	free(home);
	return (0);
}

int	cd_to_path(const char *path)
{
	if (!path || path[0] == '\0')
		return (0);
	if (chdir(path) == -1)
	{
		print_error("cd", strerror(errno));
		return (1);
	}
	return (0);
}

int	expand_tilde_with_slash(char *path, char *home)
{
	char	*expanded_path;
	int		result;

	expanded_path = ft_strjoin(home, path + 1);
	if (!expanded_path)
		return (1);
	result = cd_to_path(expanded_path);
	free(expanded_path);
	return (result);
}

int	handle_tilde_only(char *home)
{
	return (cd_to_path(home));
}
