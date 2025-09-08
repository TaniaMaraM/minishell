/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:13:36 by tmarcos           #+#    #+#             */
/*   Updated: 2024/12/10 23:16:54 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;

	if (s == NULL)
		return (NULL);
	if (start >= ft_strlen(s))
	{
		substr = (char *)malloc(1);
		if (substr == NULL)
			return (NULL);
		substr[0] = '\0';
		return (substr);
	}
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	substr = (char *)malloc(len + 1);
	if (substr == NULL)
		return (NULL);
	ft_strlcpy(substr, s + start, len + 1);
	return (substr);
}
