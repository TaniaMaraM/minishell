/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 11:34:38 by tmarcos           #+#    #+#             */
/*   Updated: 2025/07/29 12:32:51 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*str_dest;

	if (!s1)
		s1 = "";
	if (!s2)
		s2 = "";
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str_dest = (char *)malloc(len1 + len2 + 1);
	if (!str_dest)
		return (NULL);
	ft_strlcpy(str_dest, s1, len1 + 1);
	ft_strlcpy(str_dest + len1, s2, len2 + 1);
	return (str_dest);
}
