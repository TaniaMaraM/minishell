/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/16 21:05:37 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_quotes(char *str, t_shell *sh)
{
	char	*result;

	(void)sh;
	if (!str)
		return (NULL);
	result = ft_strdup(str);
	return (result);
}

int	has_unclosed_quotes(char *str)
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single || in_double);
}
