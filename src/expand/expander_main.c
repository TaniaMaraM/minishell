/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 09:00:00 by rwrobles         #+#    #+#             */
/*   Updated: 2025/09/02 14:25:00 by rwrobles        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_string(const char *input, t_shell *shell, t_quote_state state)
{
	t_expander	*expander;
	char		*result;

	if (!input || !shell)
		return (NULL);
	expander = init_expander(input, shell, state);
	if (!expander)
		return (NULL);
	if (expander_expand(expander))
	{
		expander_destroy(expander);
		return (NULL);
	}
	result = ft_strdup(expander->result);
	expander_destroy(expander);
	return (result);
}

int	expand_token(t_token *token, t_shell *shell)
{
	char	*expanded;

	if (!token || !shell)
		return (1);
	if (token->type == TOKEN_WORD)
	{
		if (token->quote_state == QUOTE_SINGLE)
			return (0);
		expanded = expand_string(token->value, shell, token->quote_state);
		if (!expanded)
			return (1);
		free(token->value);
		token->value = expanded;
	}
	return (0);
}

int	expand_command(t_cmd *cmd, t_shell *shell)
{
	int		i;
	char	*expanded;

	if (!cmd || !shell)
		return (1);
	i = 0;
	while (cmd->argv && cmd->argv[i])
	{
		expanded = expand_string(cmd->argv[i], shell, QUOTE_NONE);
		if (!expanded)
			return (1);
		free(cmd->argv[i]);
		cmd->argv[i] = expanded;
		i++;
	}
	return (0);
}

int	expand_command_list(t_cmd *cmd_list, t_shell *shell)
{
	t_cmd	*current;

	if (!cmd_list || !shell)
		return (1);
	current = cmd_list;
	while (current)
	{
		if (expand_command(current, shell))
			return (1);
		current = current->next;
	}
	return (0);
}
