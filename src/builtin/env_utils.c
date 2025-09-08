/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 18:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 20:07:24 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	allocate_new_env(t_shell *shell, char *new_var, int count)
{
	char	**new_envp;
	int		i;

	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
	{
		free(new_var);
		return (1);
	}
	i = 0;
	while (shell->envp[i])
	{
		new_envp[i] = shell->envp[i];
		i++;
	}
	new_envp[i] = new_var;
	new_envp[i + 1] = NULL;
	free(shell->envp);
	shell->envp = new_envp;
	return (0);
}

int	env_set_var(t_shell *shell, const char *name, const char *value)
{
	char	*new_var;
	int		i;
	int		result;

	if (!shell || !name)
		return (1);
	new_var = create_env_var(name, value);
	if (!new_var)
		return (1);
	if (!update_existing_var(shell, name, new_var))
		return (0);
	i = 0;
	while (shell->envp[i])
		i++;
	result = allocate_new_env(shell, new_var, i);
	return (result);
}

int	env_unset_var(t_shell *shell, const char *name)
{
	char	**new_envp;
	int		env_count;
	int		i;

	if (!shell || !name)
		return (1);
	if (find_var_index(shell, name) == -1)
		return (0);
	i = 0;
	while (shell->envp[i])
		i++;
	env_count = i;
	new_envp = malloc(sizeof(char *) * env_count);
	if (!new_envp)
		return (1);
	if (copy_env_without_var(shell, name, new_envp))
		return (1);
	ft_strarr_free(shell->envp);
	shell->envp = new_envp;
	return (0);
}

int	is_valid_var_name(const char *name)
{
	int	i;

	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
