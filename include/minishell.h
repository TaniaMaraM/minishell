/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 08:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/08 20:53:07 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <dirent.h>
# include <termios.h>
# include <curses.h>
# include <term.h>

// --- Custom Libraries ---
# include "../lib/get_next_line/get_next_line.h"
# include "../lib/libft/libft.h"

// --- Project Headers ---
# include "tokens.h"
# include "signals.h"
# include "cmd.h"
# include "expand.h"
# include "builtin.h"
# include "exec.h"

/* Shell state structure */
typedef struct s_shell
{
	char		**envp;			/* Environment variables array */
	int			last_status;	/* Last command exit status for $? */
	int			is_interactive;	/* Whether running in interactive mode */
	char		*prompt;		/* Current prompt string */
	int			should_exit;	/* Flag to indicate shell should exit */
	int			exit_code;		/* Exit code for shell termination */
	int			stdin_backup;	/* Backup of the original terminal STDIN */
}	t_shell;

/* Function prototypes */
int		shell_init(t_shell *shell, char **envp);
void	shell_cleanup(t_shell *shell);
int		shell_loop(t_shell *shell);
char	*read_command_line(void);
void	process_line(char *input, t_shell *sh);
void	print_error(const char *context, const char *message);

/* Quote processing functions */
char	*process_quotes(char *str, t_shell *sh);
int		has_unclosed_quotes(char *str);


#endif
