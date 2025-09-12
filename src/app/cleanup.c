/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:30:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/12 18:38:02 by rwrobles         ###   ########.fr       */
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
