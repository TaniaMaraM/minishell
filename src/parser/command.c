/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 09:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/08 11:31:21 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*cmd_create(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}

void	cmd_destroy(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->argv)
		ft_strarr_free(cmd->argv);
	if (cmd->redirs)
		redir_destroy_list(cmd->redirs);
	free(cmd);
}

void	cmd_destroy_list(t_cmd *cmd_list)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmd_list;
	while (current)
	{
		next = current->next;
		cmd_destroy(current);
		current = next;
	}
}

int	cmd_add_arg(t_cmd *cmd, const char *arg)
{
	char	**new_argv;
	int		argc;
	int		i;

	if (!arg)
		return (0);
	argc = 0;
	if (cmd->argv)
		while (cmd->argv[argc])
			argc++;
	new_argv = malloc(sizeof(char *) * (argc + 2));
	if (!new_argv)
		return (0);
	i = 0;
	while (i < argc)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[argc] = ft_strdup(arg);
	new_argv[argc + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
	return (1);
}
