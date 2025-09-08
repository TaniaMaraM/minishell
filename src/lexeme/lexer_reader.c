/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:45:00 by rwrobles         #+#    #+#             */
/*   Updated: 2025/08/23 11:30:00 by rwrobles        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "minishell.h"

char	*lexer_read_word(t_lexer *lexer)
{
	size_t	start;
	size_t	len;

	start = lexer->pos;
	while (lexer->pos < lexer->len
		&& !is_whitespace(lexer->input[lexer->pos])
		&& !is_metacharacter(lexer->input[lexer->pos])
		&& !is_quote(lexer->input[lexer->pos]))
	{
		lexer->pos++;
	}
	len = lexer->pos - start;
	return (ft_substr(lexer->input, start, len));
}

char	*lexer_read_quoted(t_lexer *lexer, char quote)
{
	size_t	start;
	size_t	len;

	lexer->pos++;
	start = lexer->pos;
	while (lexer->pos < lexer->len && lexer->input[lexer->pos] != quote)
		lexer->pos++;
	if (lexer->pos >= lexer->len)
		return (NULL);
	len = lexer->pos - start;
	lexer->pos++;
	return (ft_substr(lexer->input, start, len));
}
