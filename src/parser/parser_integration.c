/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_integration.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 09:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/11 17:49:06 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_setup(const char *input, t_lexer **lexer,
				t_parser **parser, t_shell *shell)
{
	*lexer = lexer_init(input);
	if (!*lexer)
		return (1);
	*parser = parser_init(*lexer, shell);
	if (!*parser)
	{
		lexer_destroy(*lexer);
		return (1);
	}
	return (0);
}

static int	parse_execute(t_parser *parser)
{
	t_cmd	*cmd_list;
	int		status;

	cmd_list = parser_parse(parser);
	if (parser->error || !cmd_list)
	{
		print_error("parser", "Syntax error");
		status = 1;
	}
	else
	{
               status = execute_command_list(cmd_list, parser->shell);
       }
       return (status);
}

int	parse_and_process(const char *input, t_shell *shell)
{
	t_lexer		*lexer;
	t_parser	*parser;
	int			status;

	if (parse_setup(input, &lexer, &parser, shell))
		return (1);
	status = parse_execute(parser);
	parser_destroy(parser);
	lexer_destroy(lexer);
	return (status);
}
