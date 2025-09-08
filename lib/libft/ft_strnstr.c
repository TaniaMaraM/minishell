/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:18:32 by tmarcos           #+#    #+#             */
/*   Updated: 2024/12/04 17:11:28 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	little_len;

	little_len = ft_strlen(little);
	i = 0;
	if (little_len == 0)
		return ((char *)big);
	while (i < len && big[i] != '\0')
	{
		j = 0;
		while (j < little_len && i + j < len && big[i + j] == little[j])
		{
			j++;
		}
		if (j == little_len)
			return ((char *)(big + i));
		i++;
	}
	return (NULL);
}
