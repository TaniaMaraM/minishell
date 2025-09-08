/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:15:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 19:42:46 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**duplicate_env(char **envp)
{
	char	**new_envp;
	int		count;
	int		i;

	count = 0;
	while (envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			ft_strarr_free(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

int	shell_init(t_shell *shell, char **envp)
{
	if (!shell || !envp)
		return (1);
	shell->last_status = 0;
	shell->is_interactive = isatty(STDIN_FILENO);
	shell->prompt = "minishell$ ";
	shell->should_exit = 0;
	shell->exit_code = 0;
	shell->envp = duplicate_env(envp);
	if (!shell->envp)
	{
		print_error("initialization", "Failed to copy environment");
		return (1);
	}
	if (shell->is_interactive)
		signal_setup_interactive();
	else
		signal_setup_non_interactive();
	return (0);
}
