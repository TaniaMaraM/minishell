/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:15:00 by rwrobles         #+#    #+#             */
/*   Updated: 2025/09/08 21:00:00 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_expander	*expander_init(const char *input, t_shell *shell,
		t_quote_state state)
{
	return (expander_alloc(input, shell, state));
}

void	expander_destroy(t_expander *expander)
{
	if (!expander)
		return ;
	if (expander->input)
		free(expander->input);
	if (expander->result)
		free(expander->result);
	free(expander);
}

int	expander_expand(t_expander *expander)
{
	char	c;

	if (!expander)
		return (1);
	while (expander->input[expander->input_pos])
	{
		c = expander->input[expander->input_pos];
		if (expander_process_char(expander, c))
			return (1);
	}
	expander->result[expander->result_pos] = '\0';
	return (0);
}
