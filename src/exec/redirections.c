/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 21:01:36 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_redirection(t_redir *redir)
{
	if (redir->type == REDIR_IN)
		return (handle_input_redirection(redir->file));
	else if (redir->type == REDIR_OUT)
		return (handle_output_redirection(redir->file));
	else if (redir->type == REDIR_APPEND)
		return (handle_append_redirection(redir->file));
	else if (redir->type == REDIR_HEREDOC)
		return (handle_heredoc_redirection(redir->file));
	return (0);
}

int	setup_redirections(t_redir *redirs)
{
	t_redir	*current;

	if (!redirs)
		return (0);
	current = redirs;
	while (current)
	{
		if (process_redirection(current))
			return (1);
		current = current->next;
	}
	return (0);
}
