/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 19:42:51 by tmarcos           #+#    #+#             */
/*   Updated: 2025/01/16 11:05:45 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	is_placeholder(char c)
{
	char	*valid;

	valid = "cspdiuxX%";
	while (*valid)
	{
		if (c == *valid)
			return (1);
		valid++;
	}
	return (-1);
}

int	ft_format(char specifier, va_list *args)
{
	int	len;

	len = 0;
	if (specifier == 'c')
		len = ft_printchar(va_arg(*args, int));
	else if (specifier == 's')
		len = ft_printstr(va_arg(*args, char *));
	else if (specifier == 'p')
		len = ft_printptr(va_arg(*args, void *));
	else if (specifier == 'd' || specifier == 'i')
		len = ft_printnbr(va_arg(*args, int));
	else if (specifier == 'u')
		len = ft_print_unsigned(va_arg(*args, unsigned int));
	else if (specifier == 'x')
		len = ft_printhex(va_arg(*args, unsigned int), "0123456789abcdef");
	else if (specifier == 'X')
		len = ft_printhex(va_arg(*args, unsigned int), "0123456789ABCDEF");
	else if (specifier == '%')
		len = ft_printchar('%');
	return (len);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		len;

	i = 0;
	len = 0;
	if (!format)
		return (-1);
	va_start (args, format);
	while (format[i] != '\0')
	{
		if (format[i] == '%' && is_placeholder(format[i + 1]))
		{
			len += ft_format(format[i + 1], &args);
			i++;
		}
		else
		{
			ft_printchar(format[i]);
			len++;
		}
		i++;
	}
	va_end (args);
	return (len);
}
