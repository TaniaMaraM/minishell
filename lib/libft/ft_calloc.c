/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:57:24 by tmarcos           #+#    #+#             */
/*   Updated: 2024/12/04 17:35:14 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total_bytes;
	void	*ptr;

	total_bytes = nmemb * size;
	if (nmemb != 0 && total_bytes / nmemb != size)
		return (NULL);
	ptr = malloc(total_bytes);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, total_bytes);
	return (ptr);
}
