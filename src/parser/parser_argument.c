/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_argument.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 21:40:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 21:32:52 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_token_spacing(t_parser *parser, size_t last_token_end);
char	*get_token_value(t_parser *parser);
char	*process_token_join(t_parser *parser, char *result);
int		process_additional_tokens(t_parser *parser, char **result,
			size_t *last_token_end);

static char	*init_argument_result(t_parser *parser)
{
	char	*result;

	if (parser->current_token->value)
		result = ft_strdup(parser->current_token->value);
	else
		result = ft_strdup("");
	if (!result)
	{
		parser->error = 1;
		return (NULL);
	}
	return (result);
}

char	*process_single_argument(t_parser *parser)
{
	char	*result;
	size_t	last_token_end;

	if (!parser->current_token || parser->current_token->type != TOKEN_WORD)
		return (NULL);
	if (expand_token(parser->current_token, parser->shell))
	{
		parser->error = 1;
		return (NULL);
	}
	result = init_argument_result(parser);
	if (!result)
		return (NULL);
	last_token_end = parser->lexer->pos;
	if (!parser_advance(parser) || !process_additional_tokens(parser, &result,
			&last_token_end))
		return (NULL);
	return (result);
}

int	parser_parse_arguments(t_parser *parser, t_cmd *cmd)
{
	char	*processed_arg;

	while (parser->current_token && parser->current_token->type == TOKEN_WORD)
	{
		processed_arg = process_single_argument(parser);
		if (!processed_arg)
			return (0);
		if (!cmd_add_arg(cmd, processed_arg))
		{
			free(processed_arg);
			parser->error = 1;
			return (0);
		}
		free(processed_arg);
	}
	return (1);
}
