/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 19:32:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 20:59:21 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_var(t_shell *shell, const char *arg);

void	print_exported_vars(t_shell *shell)
{
	int		i;
	char	*name;
	char	*value;

	if (!shell || !shell->envp)
		return ;
	i = 0;
	while (shell->envp[i])
	{
		name = ft_strchr(shell->envp[i], '=');
		if (name)
		{
			*name = '\0';
			value = name + 1;
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(shell->envp[i], STDOUT_FILENO);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(value, STDOUT_FILENO);
			ft_putendl_fd("\"", STDOUT_FILENO);
			*name = '=';
		}
		i++;
	}
}

char	*extract_var_name(const char *arg)
{
	char	*equal_pos;
	char	*name;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
		return (ft_strdup(arg));
	name = ft_substr(arg, 0, equal_pos - arg);
	return (name);
}

char	*extract_var_value(const char *arg)
{
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
		return (NULL);
	return (ft_strdup(equal_pos + 1));
}

int	validate_and_set_var(t_shell *shell, char *name, char *value)
{
	if (!is_valid_var_name(name))
	{
		print_error("export", "not a valid identifier");
		free(name);
		free(value);
		return (1);
	}
	if (env_set_var(shell, name, value))
	{
		free(name);
		free(value);
		return (1);
	}
	free(name);
	free(value);
	return (0);
}
