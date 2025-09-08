/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_detection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 18:00:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/02 18:18:47 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(const char *command)
{
	if (!command)
		return (0);
	if (ft_strcmp((char *)command, "echo") == 0)
		return (1);
	if (ft_strcmp((char *)command, "cd") == 0)
		return (1);
	if (ft_strcmp((char *)command, "pwd") == 0)
		return (1);
	if (ft_strcmp((char *)command, "export") == 0)
		return (1);
	if (ft_strcmp((char *)command, "unset") == 0)
		return (1);
	if (ft_strcmp((char *)command, "env") == 0)
		return (1);
	if (ft_strcmp((char *)command, "exit") == 0)
		return (1);
	return (0);
}
