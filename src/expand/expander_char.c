/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 21:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 21:14:24 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quoted_char(t_expander *expander, char c)
{
	if (c == '\'' && expander->quote_state == QUOTE_DOUBLE)
	{
		if (expander_append_char(expander, c))
			return (1);
		expander->input_pos++;
	}
	else if (c == '"' && expander->quote_state == QUOTE_SINGLE)
	{
		if (expander_append_char(expander, c))
			return (1);
		expander->input_pos++;
	}
	else
	{
		expander->quote_state
            = update_quote_state(expander->quote_state, c);
		expander->input_pos++;
	}
	return (0);
}

int	handle_regular_char(t_expander *expander, char c)
{
	if (expander_append_char(expander, c))
		return (1);
	expander->input_pos++;
	return (0);
}

int	expander_process_char(t_expander *expander, char c)
{
	if (c == '\\' && (expander->quote_state == QUOTE_DOUBLE
			|| expander->quote_state == QUOTE_NONE))
		return (handle_backslash_escape(expander));
	else if (c == '$' && should_expand_in_context(expander->quote_state))
		return (expander_handle_variable(expander));
	else if (is_quote_char(c))
		return (handle_quoted_char(expander, c));
	else
		return (handle_regular_char(expander, c));
}
