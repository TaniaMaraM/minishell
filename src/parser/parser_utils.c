/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 09:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/13 14:31:14 by rwrobles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parser_is_redir_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

t_redir_type	parser_token_to_redir_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	else if (type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	else if (type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	else if (type == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_IN);
}

void	redir_destroy(t_redir *redir)
{
	if (!redir)
		return ;
	if (redir->file)
		free(redir->file);
	free(redir);
}

void	redir_destroy_list(t_redir *redir_list)
{
	t_redir	*current;
	t_redir	*next;

	current = redir_list;
	while (current)
	{
		next = current->next;
		redir_destroy(current);
		current = next;
	}
}
