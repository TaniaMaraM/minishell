/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:30:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 11:30:00 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_cleanup(t_shell *shell)
{
	if (!shell)
		return ;
	signal_restore_defaults();
	if (shell->envp)
		ft_strarr_free(shell->envp);
	clear_history();
}
