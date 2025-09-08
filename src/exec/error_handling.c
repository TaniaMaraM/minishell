/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 11:30:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/03 14:45:00 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_command_error(const char *command, const char *error)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd((char *)error, STDERR_FILENO);
}

int	handle_execution_error(int error_code, const char *command)
{
	if (error_code == 127)
		print_command_error(command, "command not found");
	else if (error_code == 126)
		print_command_error(command, "permission denied");
	else if (error_code == 1)
		print_command_error(command, "execution failed");
	return (error_code);
}
