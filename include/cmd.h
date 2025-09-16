/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 08:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/16 21:18:38 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_H
# define CMD_H

# include "tokens.h"

/* Forward declaration */
typedef struct s_shell	t_shell;

/* Redirection types */
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}						t_redir_type;

/* Redirection structure */
typedef struct s_redir
{
	t_redir_type		type;
	char				*file;
	int					fd;
	int					expand;
	struct s_redir		*next;
}						t_redir;

/* Command structure */
typedef struct s_cmd
{
	char				**argv;
	t_redir				*redirs;
	struct s_cmd		*next;
	struct s_cmd		*prev;
}						t_cmd;

/* Parser structure */
typedef struct s_parser
{
	t_lexer				*lexer;
	t_token				*current_token;
	t_cmd				*cmd_list;
	int					error;
	t_shell				*shell;
}						t_parser;

/* Parser functions */
t_parser				*parser_init(t_lexer *lexer, t_shell *shell);
void					parser_destroy(t_parser *parser);
t_cmd					*parser_parse(t_parser *parser);
int						parser_parse_pipeline(t_parser *parser);
t_cmd					*parser_parse_command(t_parser *parser);
int						parser_parse_redirections(t_parser *parser, t_cmd *cmd);
int						parser_parse_arguments(t_parser *parser, t_cmd *cmd);
int						parser_parse_single_redir(t_parser *parser, t_cmd *cmd);
int						parser_parse_single_arg(t_parser *parser, t_cmd *cmd);
char					*process_single_argument(t_parser *parser);

/* Command functions */
t_cmd					*init_cmd(void);
void					cmd_destroy(t_cmd *cmd);
void					cmd_destroy_list(t_cmd *cmd_list);
int						cmd_add_arg(t_cmd *cmd, const char *arg);
int						cmd_add_redir(t_cmd *cmd, t_redir_type type,
							const char *file);
int						cmd_add_redir_with_quote(t_cmd *cmd, t_redir_type type,
							const char *file, t_quote_state quote_state);

/* Redirection functions */
t_redir					*init_redir(t_redir_type type, const char *file);
void					redir_destroy(t_redir *redir);
void					redir_destroy_list(t_redir *redir_list);

/* Parser utilities */
int						parser_advance(t_parser *parser);
int						parser_expect(t_parser *parser, t_token_type expected);
int						parser_is_redir_token(t_token_type type);
t_redir_type			parser_token_to_redir_type(t_token_type type);

/* Parser argument helpers */
int						check_token_spacing(t_parser *parser,
							size_t last_token_end);
char					*get_token_value(t_parser *parser);
char					*process_token_join(t_parser *parser, char *result);
int						process_additional_tokens(t_parser *parser,
							char **result, size_t *last_token_end);

/* Integration function */
int						parse_and_process(const char *input, t_shell *shell);

#endif
