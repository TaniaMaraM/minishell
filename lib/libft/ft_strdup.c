/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:58:00 by tmarcos           #+#    #+#             */
/*   Updated: 2024/12/04 17:09:39 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*copy;

	len = ft_strlen(s) + 1;
	copy = (char *)malloc(len);
	if (copy == NULL)
	{
		return (NULL);
	}
	ft_strlcpy (copy, s, len);
	return (copy);
}
