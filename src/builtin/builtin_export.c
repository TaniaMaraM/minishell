/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 20:07:09 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_export_arg(char **argv, int i, t_shell *shell)
{
	char	*var_name;
	char	*equal_pos;
	int		result;

	equal_pos = ft_strchr(argv[i], '=');
	result = 0;
	if (equal_pos)
	{
		var_name = ft_substr(argv[i], 0, equal_pos - argv[i]);
		if (var_name)
		{
			if (export_var(shell, argv[i]))
				result = 1;
			free(var_name);
		}
	}
	else
	{
		if (export_var(shell, argv[i]))
			result = 1;
	}
	return (result);
}

int	builtin_export(char **argv, t_shell *shell)
{
	int	i;
	int	error;

	if (!argv || !shell)
		return (1);
	if (!argv[1])
	{
		print_exported_vars(shell);
		return (0);
	}
	error = 0;
	i = 1;
	while (argv[i])
	{
		if (process_export_arg(argv, i, shell))
			error = 1;
		i++;
	}
	return (error);
}
