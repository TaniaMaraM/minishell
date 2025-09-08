#ifndef TOKENS_H
# define TOKENS_H

# include <stddef.h>

// --- Custom Libraries ---
# include "../lib/get_next_line/get_next_line.h"
# include "../lib/libft/libft.h"

/* Token types */
typedef enum e_token_type
{
	TOKEN_WORD,		/* Regular word/argument */
	TOKEN_PIPE,		/* | */
	TOKEN_REDIR_IN,		/* < */
	TOKEN_REDIR_OUT,	/* > */
	TOKEN_REDIR_APPEND,	/* >> */
	TOKEN_HEREDOC,		/* << */
	TOKEN_EOF,		/* End of input */
	TOKEN_ERROR		/* Invalid token */
}	t_token_type;

/* Quote state for tokens */
typedef enum e_quote_state
{
	QUOTE_NONE,		/* No quotes */
	QUOTE_SINGLE,	/* Single quotes - no expansion */
	QUOTE_DOUBLE	/* Double quotes - allow expansion */
}	t_quote_state;

/* Token structure */
typedef struct s_token
{
	t_token_type	type;		/* Type of token */
	char		*value;		/* String value of token */
	size_t		length;		/* Length of value */
	t_quote_state	quote_state;	/* Quote context for expansion */
}	t_token;

/* Lexer state structure */
typedef struct s_lexer
{
	const char	*input;		/* Input string to tokenize */
	size_t		pos;		/* Current position in input */
	size_t		len;		/* Length of input string */
}	t_lexer;

/* Lexer functions */
t_lexer	*lexer_init(const char *input);
void	lexer_destroy(t_lexer *lexer);
t_token	*lexer_next_token(t_lexer *lexer);
t_token	*create_token(t_token_type type, const char *value, size_t length);
t_token	*create_quoted_token(t_token_type type, const char *value, size_t length, t_quote_state quote_state);
void	token_destroy(t_token *token);

/* Tokenizer functions */
int		tokenize_and_process(const char *input);

/* Helper functions */
int		is_whitespace(char c);
int		is_metacharacter(char c);
int		is_quote(char c);
char	*lexer_read_word(t_lexer *lexer);
char	*lexer_read_quoted(t_lexer *lexer, char quote);

#endif
