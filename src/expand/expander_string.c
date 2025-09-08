/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:30:00 by rwrobles         #+#    #+#             */
/*   Updated: 2025/09/02 16:15:00 by rwrobles        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expander_append_char(t_expander *expander, char c)
{
	if (!expander)
		return (1);
	if (expander->result_pos >= expander->result_capacity)
	{
		if (expander_resize_result(expander))
			return (1);
	}
	expander->result[expander->result_pos] = c;
	expander->result_pos++;
	return (0);
}

int	expander_append_string(t_expander *expander, const char *str)
{
	size_t	i;

	if (!expander || !str)
		return (1);
	i = 0;
	while (str[i])
	{
		if (expander_append_char(expander, str[i]))
			return (1);
		i++;
	}
	return (0);
}

int	expander_resize_result(t_expander *expander)
{
	char	*new_result;
	size_t	new_capacity;

	if (!expander)
		return (1);
	new_capacity = expander->result_capacity * 2;
	new_result = malloc(new_capacity + 1);
	if (!new_result)
		return (1);
	ft_memcpy(new_result, expander->result, expander->result_pos);
	free(expander->result);
	expander->result = new_result;
	expander->result_capacity = new_capacity;
	return (0);
}
