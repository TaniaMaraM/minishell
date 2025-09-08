/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:26:53 by tmarcos           #+#    #+#             */
/*   Updated: 2025/07/04 11:26:38 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	len_dest;
	size_t	len_src;
	size_t	i;

	len_dest = ft_strlen(dest);
	len_src = ft_strlen(src);
	if (size <= len_dest)
		return (size + len_src);
	i = 0;
	while (src[i] != '\0' && (len_dest + i) < (size - 1))
	{
		dest[len_dest + i] = src[i];
		i++;
	}
	dest [len_dest + i] = '\0';
	return (len_dest + len_src);
}
