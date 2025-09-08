/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_iuptr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 11:26:30 by tmarcos           #+#    #+#             */
/*   Updated: 2025/01/16 11:56:05 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printnbr(int n)
{
	char	c;
	int		len;

	len = 0;
	if (n == -2147483648)
		return (ft_printstr("-2147483648"));
	if (n < 0)
	{
		len += ft_printchar('-');
		n = -n;
	}
	if (n >= 10)
		len += ft_printnbr(n / 10);
	c = (n % 10) + '0';
	len += ft_printchar(c);
	return (len);
}

int	ft_print_unsigned(unsigned int n)
{
	char	c;
	int		len;

	len = 0;
	if (n >= 10)
		len += ft_print_unsigned(n / 10);
	c = (n % 10) + '0';
	len += ft_printchar(c);
	return (len);
}

int	ft_printhex(unsigned long n, const char *base)
{
	int				len;
	unsigned int	base_len;

	len = 0;
	base_len = 16;
	if (n >= base_len)
	{
		len += ft_printhex(n / base_len, base);
	}
	len += ft_printchar(base[n % base_len]);
	return (len);
}

int	ft_printptr(void *ptr)
{
	unsigned long	addr;
	int				len;

	addr = (unsigned long)ptr;
	len = 0;
	if (!ptr)
		len += ft_printstr("(nil)");
	else
	{
		len += ft_printstr("0x");
		addr = (unsigned long)ptr;
		len += ft_printhex(addr, "0123456789abcdef");
	}
	return (len);
}
