/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_argument_process.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 21:45:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 21:30:32 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_token_spacing(t_parser *parser, size_t last_token_end)
{
	size_t	next_token_start;

	next_token_start = last_token_end;
	while (next_token_start < parser->lexer->len
		&& is_whitespace(parser->lexer->input[next_token_start]))
		next_token_start++;
	return (next_token_start == last_token_end);
}

char	*get_token_value(t_parser *parser)
{
	if (parser->current_token->value)
		return (parser->current_token->value);
	return ("");
}

char	*process_token_join(t_parser *parser, char *result)
{
	char	*temp;
	char	*expanded;

	if (expand_token(parser->current_token, parser->shell))
	{
		free(result);
		parser->error = 1;
		return (NULL);
	}
	expanded = get_token_value(parser);
	temp = ft_strjoin(result, expanded);
	free(result);
	if (!temp)
	{
		parser->error = 1;
		return (NULL);
	}
	return (temp);
}

int	process_additional_tokens(t_parser *parser, char **result,
		size_t *last_token_end)
{
	while (parser->current_token && parser->current_token->type == TOKEN_WORD)
	{
		if (check_token_spacing(parser, *last_token_end) == 0)
			break ;
		*result = process_token_join(parser, *result);
		if (*result == NULL)
			return (0);
		*last_token_end = parser->lexer->pos;
		if (parser_advance(parser) == 0)
		{
			free(*result);
			*result = NULL;
			return (0);
		}
	}
	return (1);
}
