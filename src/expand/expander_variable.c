/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_variable.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:45:00 by rwrobles         #+#    #+#             */
/*   Updated: 2025/09/02 22:10:00 by rwrobles        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expander_handle_exit_status(t_expander *expander)
{
	char	*var_value;

	expander->input_pos++;
	var_value = resolve_exit_status(expander->shell);
	if (var_value)
	{
		if (expander_append_string(expander, var_value))
		{
			free(var_value);
			return (1);
		}
		free(var_value);
	}
	return (0);
}

int	handle_braced_var(t_expander *expander);

static int	handle_regular_var(t_expander *expander)
{
	char	*var_name;
	char	*var_value;

	var_name = parse_variable_name(expander->input, &expander->input_pos);
	if (!var_name)
	{
		if (expander_append_char(expander, '$'))
			return (1);
		return (0);
	}
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
	return (0);
}

static int	expander_handle_env_var(t_expander *expander)
{
	if (expander->input[expander->input_pos] == '{')
		return (handle_braced_var(expander));
	return (handle_regular_var(expander));
}

int	expander_handle_variable(t_expander *expander)
{
	if (!expander)
		return (1);
	expander->input_pos++;
	if (expander->input[expander->input_pos] == '?')
		return (expander_handle_exit_status(expander));
	else
		return (expander_handle_env_var(expander));
}
