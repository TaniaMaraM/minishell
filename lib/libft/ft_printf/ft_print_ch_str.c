/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ch_str.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:13:52 by tmarcos           #+#    #+#             */
/*   Updated: 2025/01/16 11:01:14 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_printstr(const char *str)
{
	int	len;

	len = 0;
	if (!str)
		str = "(null)";
	while (str[len] != '\0')
		ft_printchar(str[len++]);
	return (len);
}
