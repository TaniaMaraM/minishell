/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:29:23 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/09 19:29:51 by rwrobles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

# include <stddef.h>

// --- Custom Libraries ---
# include "../lib/get_next_line/get_next_line.h"
# include "../lib/libft/libft.h"

/* Token types */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF,
	TOKEN_ERROR
}					t_token_type;

/* Quote state for tokens */
typedef enum e_quote_state
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}					t_quote_state;

/* Token structure */
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	size_t			length;
	t_quote_state	quote_state;
}					t_token;

/* Lexer state structure */
typedef struct s_lexer
{
	const char		*input;
	size_t			pos;
	size_t			len;
}					t_lexer;

/* Lexer functions */
t_lexer				*lexer_init(const char *input);
void				lexer_destroy(t_lexer *lexer);
t_token				*lexer_next_token(t_lexer *lexer);
t_token				*create_token(t_token_type type, const char *value,
						size_t length);
t_token				*create_quoted_token(t_token_type type, const char *value,
						size_t length, t_quote_state quote_state);
void				token_destroy(t_token *token);

/* Tokenizer functions */
int					tokenize_and_process(const char *input);

/* Helper functions */
int					is_whitespace(char c);
int					is_metacharacter(char c);
int					is_quote(char c);
char				*lexer_read_word(t_lexer *lexer);
char				*lexer_read_quoted(t_lexer *lexer, char quote);

#endif
