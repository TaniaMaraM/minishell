/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 12:59:22 by tmarcos           #+#    #+#             */
/*   Updated: 2024/12/04 17:12:00 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	trim_start;
	size_t	trim_end;
	size_t	len;
	char	*str_dest;

	if (s1 == NULL || set == NULL)
		return (NULL);
	trim_start = 0;
	while (s1[trim_start] && ft_strchr(set, s1[trim_start]))
	{
		trim_start ++;
	}
	trim_end = ft_strlen(s1);
	while (trim_end > trim_start && ft_strrchr(set, s1[trim_end - 1]))
	{
		trim_end--;
	}
	len = trim_end - trim_start;
	str_dest = (char *)malloc(len + 1);
	if (str_dest == NULL)
		return (NULL);
	ft_strlcpy(str_dest, s1 + trim_start, len + 1);
	return (str_dest);
}
