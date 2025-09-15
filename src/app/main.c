/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 09:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/15 14:04:56 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;
	int		exit_status;

	(void)argc;
	(void)argv;
	if (shell_init(&sh, envp) != 0) //inicializa envp mas nao da free nunca deveria free no shell_cleanup
	{
		print_error("initialization", "Failed to initialize shell");
		return (1);
	}
	exit_status = shell_loop(&sh);
	shell_cleanup(&sh);
	if (sh.should_exit && sh.exit_code != 0)
		return (sh.exit_code);
	return (exit_status);
}
