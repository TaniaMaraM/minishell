/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 13:30:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 20:07:14 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(char **argv, t_shell *shell)
{
	int	i;

	(void)argv;
	if (!shell || !shell->envp)
		return (1);
	i = 0;
	while (shell->envp[i])
	{
		ft_putendl_fd(shell->envp[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}
