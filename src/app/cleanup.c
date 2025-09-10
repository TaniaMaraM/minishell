/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:30:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/10 11:10:01 by tmarcos          ###   ########.fr       */
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
	if (shell->stdin_backup != -1)
		close(shell->stdin_backup);
	clear_history();
}
