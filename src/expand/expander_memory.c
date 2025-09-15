/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_memory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:00:00 by rwrobles         #+#    #+#             */
/*   Updated: 2025/09/02 20:45:00 by rwrobles        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expander_alloc_result(t_expander *expander, const char *input)
{
	expander->result_capacity = ft_strlen(input) * 2;
	expander->result = malloc(expander->result_capacity + 1);
	if (!expander->result)
	{
		free(expander->input);
		free(expander);
		return (1);
	}
	expander->result[0] = '\0';
	return (0);
}

t_expander	*init_expander(const char *input, t_shell *shell,
		t_quote_state state)
{
	t_expander	*expander;

	if (!input || !shell)
		return (NULL);
	expander = ft_calloc(1, sizeof(t_expander));
	if (!expander)
		return (NULL);
	expander->input = ft_strdup(input);
	if (!expander->input)
	{
		free(expander);
		return (NULL);
	}
	if (expander_alloc_result(expander, input))
		return (NULL);
	expander->input_pos = 0;
	expander->result_pos = 0;
	expander->quote_state = state;
	expander->shell = shell;
	expander->error = 0;
	return (expander);
}

void	expander_set_error(t_expander *expander)
{
	if (expander)
		expander->error = 1;
}

char	*var_name_alloc(const char *input, size_t start, size_t len)
{
	char	*var_name;

	if (!input || len == 0)
		return (NULL);
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_memcpy(var_name, input + start, len);
	var_name[len] = '\0';
	return (var_name);
}
