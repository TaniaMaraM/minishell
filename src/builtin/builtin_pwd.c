/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 18:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/02 18:08:49 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(char **argv, t_shell *shell)
{
	char	*cwd;

	(void)argv;
	(void)shell;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		print_error("pwd", strerror(errno));
		return (1);
	}
	ft_putendl_fd(cwd, STDOUT_FILENO);
	free(cwd);
	return (0);
}
