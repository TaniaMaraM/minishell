/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_resolution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:15:00 by rwrobles         #+#    #+#             */
/*   Updated: 2025/09/03 10:45:00 by rwrobles        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*resolve_variable(const char *var_name, t_shell *shell)
{
	if (!var_name || !shell)
		return (NULL);
	if (ft_strcmp((char *)var_name, "?") == 0)
		return (resolve_exit_status(shell));
	return (get_env_var(var_name, shell));
}

char	*resolve_exit_status(t_shell *shell)
{
	char	*status_str;

	if (!shell)
		return (NULL);
	status_str = ft_itoa(shell->last_status);
	return (status_str);
}

char	*get_env_var(const char *name, t_shell *shell)
{
	char	*value;
	size_t	name_len;
	size_t	i;

	if (!name || !shell || !shell->envp)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], name, name_len) == 0
			&& shell->envp[i][name_len] == '=')
		{
			value = ft_strdup(shell->envp[i] + name_len + 1);
			return (value);
		}
		i++;
	}
	return (ft_strdup(""));
}

char	*parse_variable_name(const char *input, size_t *pos)
{
	size_t	start;
	size_t	len;
	char	*var_name;

	if (!input || !pos)
		return (NULL);
	start = *pos;
	len = 0;
	while (input[start + len] && is_valid_var_char(input[start + len]))
		len++;
	if (len == 0)
		return (NULL);
	var_name = var_name_alloc(input, start, len);
	if (!var_name)
		return (NULL);
	*pos += len;
	return (var_name);
}

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}
