/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 18:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/02 18:00:00 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(char **argv, t_shell *shell)
{
	int	i;
	int	error;

	if (!argv || !shell)
		return (1);
	if (!argv[1])
		return (0);
	error = 0;
	i = 1;
	while (argv[i])
	{
		if (!is_valid_var_name(argv[i]))
		{
			print_error("unset", "not a valid identifier");
			error = 1;
		}
		else
		{
			if (env_unset_var(shell, argv[i]))
				error = 1;
		}
		i++;
	}
	return (error);
}
