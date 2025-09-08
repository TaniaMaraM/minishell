/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 09:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/08 11:32:28 by tmarcos          ###   ########.fr       */
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
