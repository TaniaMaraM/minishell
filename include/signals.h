/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 08:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/09 13:32:08 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "minishell.h"
# include "../lib/get_next_line/get_next_line.h"
# include "../lib/libft/libft.h"

/* Global signal variable - subject requirement */
extern volatile sig_atomic_t	g_signal;

/* Signal handling functions */
void	signal_setup_interactive(void);
void	signal_setup_non_interactive(void);
void	signal_restore_defaults(void);
void	signal_handler(int sig);
void	signal_handler_heredoc(int sig);

#endif