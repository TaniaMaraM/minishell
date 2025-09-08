/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 17:20:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/02 17:18:13 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(const char *context, const char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (context)
	{
		ft_putstr_fd((char *)context, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (message)
		ft_putstr_fd((char *)message, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}
