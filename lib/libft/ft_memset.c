/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:17:32 by tmarcos           #+#    #+#             */
/*   Updated: 2024/12/04 17:08:26 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int ch, size_t len)
{
	unsigned char	*ptr;

	ptr = (unsigned char *) str;
	while (len > 0)
	{
		*ptr = (unsigned char) ch;
		ptr++;
		len--;
	}
	return (str);
}
