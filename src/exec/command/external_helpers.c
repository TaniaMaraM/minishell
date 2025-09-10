/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:45:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 11:21:49 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_absolute_path(const char *command)
{
	if (ft_strchr(command, '/'))
	{
		if (access(command, F_OK) == -1)
			return (NULL);
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	return (NULL);
}

static char	*build_command_path(const char *command, const char *dir)
{
	char	*full_path;
	int		len;

	len = ft_strlen(dir) + ft_strlen(command) + 2;
	full_path = malloc(len);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, dir, ft_strlen(dir) + 1);
	ft_strlcat(full_path, "/", ft_strlen(dir) + 2);
	ft_strlcat(full_path, command, len);
	return (full_path);
}

char	*search_path_dirs(const char *command, char **path_dirs)
{
	char	*full_path;
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		full_path = build_command_path(command, path_dirs[i]);
		if (!full_path)
		{
			ft_strarr_free(path_dirs);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
		{
			ft_strarr_free(path_dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_strarr_free(path_dirs);
	return (NULL);
}
