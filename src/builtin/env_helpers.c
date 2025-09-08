/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 18:45:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 19:35:29 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_env_var(const char *name, const char *value)
{
	char	*new_var;
	int		len;

	if (value)
		len = ft_strlen(name) + ft_strlen(value) + 2;
	else
		len = ft_strlen(name) + 2;
	new_var = malloc(len);
	if (!new_var)
		return (NULL);
	ft_strlcpy(new_var, name, ft_strlen(name) + 1);
	ft_strlcat(new_var, "=", ft_strlen(name) + 2);
	if (value)
		ft_strlcat(new_var, value, len);
	return (new_var);
}

int	update_existing_var(t_shell *shell, const char *name, char *new_var)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], name, ft_strlen(name)) == 0
			&& shell->envp[i][ft_strlen(name)] == '=')
		{
			free(shell->envp[i]);
			shell->envp[i] = new_var;
			return (0);
		}
		i++;
	}
	return (1);
}

int	find_var_index(t_shell *shell, const char *name)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], name, ft_strlen(name)) == 0
			&& shell->envp[i][ft_strlen(name)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	copy_env_without_var(t_shell *shell, const char *name,
		char **new_envp)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], name, ft_strlen(name)) != 0
			|| shell->envp[i][ft_strlen(name)] != '=')
		{
			new_envp[j] = ft_strdup(shell->envp[i]);
			if (!new_envp[j])
			{
				ft_strarr_free(new_envp);
				return (1);
			}
			j++;
		}
		i++;
	}
	new_envp[j] = NULL;
	return (0);
}
