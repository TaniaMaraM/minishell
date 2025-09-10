/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 18:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/10 15:04:10 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_exit_code(const char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **argv, t_shell *shell)
{
	int	exit_code;

	if (!argv || !shell)
		return (1);
	if (!argv[1])
		exit_code = 0;
	else if (argv[2])
	{
		print_error("exit", "too many arguments");
		shell->should_exit = 0;
		return (1);
	}
	else if (!is_valid_exit_code(argv[1]))
	{
		print_error("exit", "numeric argument required");
		shell->should_exit = 1;
		shell->exit_code = 255;
		shell->last_status = 255;
		return (255);
	}
	else
		exit_code = ft_atoi(argv[1]);
	if (shell->is_interactive)
		ft_putendl_fd("exit", STDOUT_FILENO);
	shell->should_exit = 1;
	shell->exit_code = exit_code;
	shell->last_status = exit_code;
	return (0);
}
