/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 18:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/12 19:22:10 by rwrobles         ###   ########.fr       */
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

static int	handle_invalid_exit_arg(t_shell *shell)
{
	print_error("exit", "numeric argument required");
	shell->should_exit = 1;
	shell->exit_code = 2;
	shell->last_status = 2;
	return (2);
}

static int	handle_too_many_args(t_shell *shell)
{
	print_error("exit", "too many arguments");
	shell->should_exit = 0;
	return (1);
}

static void	finalize_exit(t_shell *shell, int exit_code)
{
	if (shell->is_interactive)
		ft_putendl_fd("exit", STDOUT_FILENO);
	shell->should_exit = 1;
	shell->exit_code = exit_code;
	shell->last_status = exit_code;
}

int	builtin_exit(char **argv, t_shell *shell)
{
	int	exit_code;

	if (!argv || !shell)
		return (1);
	if (!argv[1])
		exit_code = 0;
	else if (argv[2])
		return (handle_too_many_args(shell));
	else if (!is_valid_exit_code(argv[1]))
		return (handle_invalid_exit_arg(shell));
	else
		exit_code = ft_atoi(argv[1]);
	finalize_exit(shell, exit_code);
	return (0);
}
