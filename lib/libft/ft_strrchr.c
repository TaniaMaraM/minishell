/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:56:56 by tmarcos           #+#    #+#             */
/*   Updated: 2024/12/06 12:02:38 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int				i;
	unsigned char	ch;

	i = 0;
	ch = (unsigned char)c;
	while (str[i] != '\0')
	{
		i++;
	}
	while (i >= 0)
	{
		if ((unsigned char)str[i] == ch)
			return ((char *)&str[i]);
		i--;
	}
	return (NULL);
}
