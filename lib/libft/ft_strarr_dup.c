/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strarr_dup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 19:21:54 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/02 19:21:55 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_strarr_dup(char **arr)
{
	char	**new_arr;
	int		len;
	int		i;

	if (!arr)
		return (NULL);
	len = 0;
	while (arr[len])
		len++;
	new_arr = ft_calloc(len + 1, sizeof(char *));
	if (!new_arr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_arr[i] = ft_strdup(arr[i]);
		if (!new_arr[i])
		{
			ft_strarr_free(new_arr);
			return (NULL);
		}
		i++;
	}
	return (new_arr);
}
