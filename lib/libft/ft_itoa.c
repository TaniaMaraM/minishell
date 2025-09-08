/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 18:34:12 by tmarcos           #+#    #+#             */
/*   Updated: 2024/12/04 17:05:19 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digits(int n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n /= 10;
		count ++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	int		negative_n;

	negative_n = (n < 0);
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (negative_n)
		n = -n;
	len = count_digits(n) + negative_n;
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len > 0)
	{
		len--;
		str[len] = (n % 10) + '0';
		n /= 10;
	}
	if (negative_n)
		str[0] = '-';
	return (str);
}
