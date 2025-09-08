/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 16:45:04 by tmarcos           #+#    #+#             */
/*   Updated: 2025/01/07 19:09:06 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>

int	is_placeholder(char c);
int	ft_format(char specifier, va_list *args);
int	ft_printf(const char *format, ...);
int	ft_printchar(char c);
int	ft_printstr(const char *str);
int	ft_printnbr(int n);
int	ft_print_unsigned(unsigned int n);
int	ft_printhex(unsigned long n, const char *base);
int	ft_printptr(void *ptr);
int	is_placeholder(char c);
int	ft_format(char specifier, va_list *args);
int	ft_printf(const char *format, ...);
int	ft_printchar(char c);
int	ft_printstr(const char *str);
int	ft_printnbr(int n);
int	ft_print_unsigned(unsigned int n);
int	ft_printhex(unsigned long n, const char *base);
int	ft_printptr(void *ptr);

#endif
