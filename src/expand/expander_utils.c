/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 09:30:00 by rwrobles         #+#    #+#             */
/*   Updated: 2025/09/03 08:20:00 by rwrobles        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_quote_state	update_quote_state(t_quote_state current, char c)
{
	if (c == '\'')
	{
		if (current == QUOTE_SINGLE)
			return (QUOTE_NONE);
		else if (current == QUOTE_NONE)
			return (QUOTE_SINGLE);
	}
	else if (c == '"')
	{
		if (current == QUOTE_DOUBLE)
			return (QUOTE_NONE);
		else if (current == QUOTE_NONE)
			return (QUOTE_DOUBLE);
	}
	return (current);
}

int	is_quote_char(char c)
{
	return (c == '\'' || c == '"');
}

int	should_expand_in_context(t_quote_state state)
{
	return (state != QUOTE_SINGLE);
}
