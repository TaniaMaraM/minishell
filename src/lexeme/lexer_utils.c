/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 13:00:00 by rwrobles         #+#    #+#             */
/*   Updated: 2025/09/08 21:30:00 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "minishell.h"

t_token	*create_token(t_token_type type, const char *value, size_t length)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->quote_state = QUOTE_NONE;
	if (value)
	{
		token->value = ft_substr(value, 0, length);
		if (!token->value)
		{
			free(token);
			return (NULL);
		}
		token->length = length;
	}
	else
	{
		token->value = NULL;
		token->length = 0;
	}
	return (token);
}

t_token	*create_quoted_token(t_token_type type, const char *value,
		size_t length, t_quote_state quote_state)
{
	t_token	*token;

	token = create_token(type, value, length);
	if (token)
		token->quote_state = quote_state;
	return (token);
}

void	token_destroy(t_token *token)
{
	if (token)
	{
		if (token->value)
			free(token->value);
		free(token);
	}
}
