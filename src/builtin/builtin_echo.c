/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 20:56:43 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int	is_valid_n_option(char *arg)
{
	int	j;

	if (!arg || arg[0] != '-')
		return (0);
	j = 1;
	while (arg[j])
	{
		if (arg[j] != 'n')
			return (0);
		j++;
	}
	return (j > 1);
}

int	builtin_echo(char **argv, t_shell *shell)
{
	int	i;
	int	print_newline;

	(void)shell;
	if (!argv)
		return (1);
	i = 1;
	print_newline = 1;
	while (argv[i] && is_valid_n_option(argv[i]))
	{
		print_newline = 0;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (print_newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
