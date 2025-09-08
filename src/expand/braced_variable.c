/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   braced_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 20:30:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 20:30:28 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_no_closing_brace(t_expander *expander)
{
	expander->input_pos--;
	if (expander_append_char(expander, '$'))
		return (1);
	if (expander_append_char(expander, '{'))
		return (1);
	return (0);
}

static int	extract_and_expand_var(t_expander *expander, size_t start,
	size_t len, size_t i)
{
	char	*var_name;
	char	*var_value;

	var_name = ft_substr(expander->input, start, len);
	if (!var_name)
		return (1);
	var_value = resolve_variable(var_name, expander->shell);
	free(var_name);
	if (var_value)
	{
		if (expander_append_string(expander, var_value))
		{
			free(var_value);
			return (1);
		}
		free(var_value);
	}
	expander->input_pos = i + 1;
	return (0);
}

int	handle_braced_var(t_expander *expander)
{
	size_t	start;
	size_t	len;
	size_t	i;

	expander->input_pos++;
	start = expander->input_pos;
	len = 0;
	i = start;
	while (expander->input[i] && expander->input[i] != '}')
	{
		len++;
		i++;
	}
	if (!expander->input[i])
		return (handle_no_closing_brace(expander));
	return (extract_and_expand_var(expander, start, len, i));
}
