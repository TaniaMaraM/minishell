/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 08:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/10 14:25:55 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <curses.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

// --- Custom Libraries ---
# include "../lib/get_next_line/get_next_line.h"
# include "../lib/libft/libft.h"

// --- Project Headers ---
# include "builtin.h"
# include "cmd.h"
# include "exec.h"
# include "expand.h"
# include "signals.h"
# include "tokens.h"

/* Exit status constants */
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_STATUS_SYNTAX_ERROR 2
# define EXIT_STATUS_SIGINT 130
# define EXIT_STATUS_SIGNAL_BASE 128
# define CMD_NOT_FOUND 127
# define CMD_PERMISSION_DENIED 126

/* Shell state structure */
typedef struct s_shell
{
	char	**envp;
	int		last_status;
	int		is_interactive;
	char	*prompt;
	int		should_exit;
	int		exit_code;
	int		stdin_backup;
	t_cmd	*current_cmd_list;
}			t_shell;

/* Function prototypes */
int			shell_init(t_shell *shell, char **envp);
void		shell_cleanup(t_shell *shell);
int			shell_loop(t_shell *shell);
char		*read_command_line(void);
void		process_line(char *input, t_shell *sh);
void		print_error(const char *context, const char *message);

/* Quote processing functions */
char		*process_quotes(char *str, t_shell *sh);
int			has_unclosed_quotes(char *str);

#endif
