/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_escape.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 21:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 21:00:00 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_double_quote_escape(t_expander *expander, char next_char)
{
	if (next_char == '$' || next_char == '"' || next_char == '\\')
	{
		if (expander_append_char(expander, next_char))
			return (1);
		expander->input_pos++;
	}
	else
	{
		if (expander_append_char(expander, '\\'))
			return (1);
		if (expander_append_char(expander, next_char))
			return (1);
		expander->input_pos++;
	}
	return (0);
}

int	handle_normal_escape(t_expander *expander, char next_char)
{
	if (next_char == '$')
	{
		if (expander_append_char(expander, next_char))
			return (1);
		expander->input_pos++;
	}
	else
	{
		if (expander_append_char(expander, '\\'))
			return (1);
		if (expander_append_char(expander, next_char))
			return (1);
		expander->input_pos++;
	}
	return (0);
}

int	handle_backslash_escape(t_expander *expander)
{
	char	next_char;

	expander->input_pos++;
	if (expander->input_pos >= ft_strlen(expander->input))
		return (0);
	next_char = expander->input[expander->input_pos];
	if (expander->quote_state == QUOTE_DOUBLE)
		return (handle_double_quote_escape(expander, next_char));
	else
		return (handle_normal_escape(expander, next_char));
}
