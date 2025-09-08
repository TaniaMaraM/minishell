/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:15:00 by rwrobles         #+#    #+#             */
/*   Updated: 2025/08/21 16:40:00 by rwrobles        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*handle_redir_in(t_lexer *lexer)
{
	lexer->pos++;
	if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '<')
	{
		lexer->pos++;
		return (create_token(TOKEN_HEREDOC, "<<", 2));
	}
	return (create_token(TOKEN_REDIR_IN, "<", 1));
}

static t_token	*handle_redir_out(t_lexer *lexer)
{
	lexer->pos++;
	if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '>')
	{
		lexer->pos++;
		return (create_token(TOKEN_REDIR_APPEND, ">>", 2));
	}
	return (create_token(TOKEN_REDIR_OUT, ">", 1));
}

static t_token	*handle_quoted_word(t_lexer *lexer, char c)
{
	char			*value;
	t_token			*token;
	t_quote_state	quote_state;

	value = lexer_read_quoted(lexer, c);
	if (!value)
		return (create_token(TOKEN_ERROR, NULL, 0));
	if (c == '\'')
		quote_state = QUOTE_SINGLE;
	else
		quote_state = QUOTE_DOUBLE;
	token = create_quoted_token(TOKEN_WORD, value, ft_strlen(value), quote_state);
	free(value);
	return (token);
}

static t_token	*handle_unquoted_word(t_lexer *lexer)
{
	char	*value;
	t_token	*token;

	value = lexer_read_word(lexer);
	if (!value)
		return (create_token(TOKEN_ERROR, NULL, 0));
	token = create_token(TOKEN_WORD, value, ft_strlen(value));
	free(value);
	return (token);
}

t_token	*lexer_next_token(t_lexer *lexer)
{
	char	c;

	if (!lexer || lexer->pos >= lexer->len)
		return (create_token(TOKEN_EOF, NULL, 0));
	while (lexer->pos < lexer->len && is_whitespace(lexer->input[lexer->pos]))
		lexer->pos++;
	if (lexer->pos >= lexer->len)
		return (create_token(TOKEN_EOF, NULL, 0));
	c = lexer->input[lexer->pos];
	if (c == '|')
	{
		lexer->pos++;
		return (create_token(TOKEN_PIPE, "|", 1));
	}
	else if (c == '<')
		return (handle_redir_in(lexer));
	else if (c == '>')
		return (handle_redir_out(lexer));
	else if (is_quote(c))
		return (handle_quoted_word(lexer, c));
	else
		return (handle_unquoted_word(lexer));
}
