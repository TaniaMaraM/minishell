/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 09:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 11:30:00 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;
	int		exit_status;

	(void)argc;
	(void)argv;
	if (shell_init(&sh, envp) != 0)
	{
		print_error("initialization", "Failed to initialize shell");
		return (1);
	}
	exit_status = shell_loop(&sh);
	shell_cleanup(&sh);
	return (exit_status);
}
