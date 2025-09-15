/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:40:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/15 13:40:29 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_var_without_value(t_shell *shell, char *name)
{
	return (validate_and_set_var(shell, name, NULL));
}

static int	handle_var_with_value(t_shell *shell, char *name, const char *arg)
{
	char	*value;

	value = extract_var_value(arg);
	if (!value)
	{
		value = ft_strdup("");
		if (!value)
		{
			free(name);
			return (1);
		}
	}
	return (validate_and_set_var(shell, name, value));
}

int	export_var(t_shell *shell, const char *arg)
{
	char	*name;
	char	*equal_pos;

	if (!arg)
		return (1);
	name = extract_var_name(arg);
	if (!name)
		return (0);
	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
		return (handle_var_without_value(shell, name));
	return (handle_var_with_value(shell, name, arg));
}
