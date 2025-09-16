/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:45:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/16 15:36:20 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief      Reads a line of input from the user.
 * @details    In interactive mode, displays a prompt and uses readline.
 * In non-interactive mode, reads from stdin without prompt.
 * @return     A dynamically allocated string containing the user's input,
 * or NULL if an EOF is encountered.
 */
char	*read_command_line(void)
{
	char	*line;

	if (isatty(STDIN_FILENO))
	{
		line = readline("minishell$ ");
	}
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (line && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
	}
	return (line);
}

/**
 * @brief Handles parser lifecycle: init, parse, cleanup
 * @param processed_input Processed input string
 * @param sh Shell context
 * @param parse_status Output parameter for parsing result
 * @return Command list (NULL on failure)
 */
static t_cmd	*handle_parser_lifecycle(char *processed_input, t_shell *sh,
		int *parse_status)
{
	t_lexer		*lexer;
	t_parser	*parser;
	t_cmd		*cmd_list;
	int			parser_error;

	if (init_lexer_parser(processed_input, &lexer, &parser, sh))
		return (*parse_status = 1, NULL);
	cmd_list = parser_parse(parser);
	parser_error = parser->error;
	parser_destroy(parser);
	lexer_destroy(lexer);
	if (parser_error || !cmd_list)
	{
		print_error("parser", "Syntax error");
		sh->last_status = EXIT_STATUS_SYNTAX_ERROR;
		return (*parse_status = 1, NULL);
	}
	return (*parse_status = 0, cmd_list);
}

/**
 * @brief Handles parsing pipeline: quotes, lexer, parser, cleanup
 * @param input Input line from user
 * @param sh Shell context
 * @param parse_status Output parameter for parsing result
 * @return Command list (NULL on failure)
 */
static t_cmd	*parse_user_input(char *input, t_shell *sh, int *parse_status)
{
	char	*processed_input;
	t_cmd	*cmd_list;

	*parse_status = 0;
	if (has_unclosed_quotes(input))
	{
		print_error("syntax", "unclosed quotes");
		sh->last_status = EXIT_STATUS_SYNTAX_ERROR;
		*parse_status = 1;
		return (NULL);
	}
	processed_input = process_quotes(input, sh);
	if (!processed_input)
	{
		print_error("quotes", "Failed to process quotes");
		sh->last_status = EXIT_FAILURE;
		*parse_status = 1;
		return (NULL);
	}
	cmd_list = handle_parser_lifecycle(processed_input, sh, parse_status);
	free(processed_input);
	return (cmd_list);
}

/**
 * @brief Handles command execution and cleanup
 * @param cmd_list Command list to execute
 * @param sh Shell context
 * @return Execution status
 */
static int	execute_and_cleanup(t_cmd *cmd_list, t_shell *sh)
{
	int	execution_status;

	if (!cmd_list)
		return (EXIT_FAILURE);
	sh->current_cmd_list = cmd_list;
	execution_status = execute_command_list(cmd_list, sh);
	sh->current_cmd_list = NULL;
	cmd_destroy_list(cmd_list);
	return (execution_status);
}

/**
 * @brief Process a single input line through tokenizing, parsing and execution
 * @param input Input line from user
 * @param sh Shell context
 */
void	process_line(char *input, t_shell *sh)
{
	t_cmd	*cmd_list;
	int		parse_status;

	cmd_list = parse_user_input(input, sh, &parse_status);
	if (parse_status)
		return ;
	sh->last_status = execute_and_cleanup(cmd_list, sh);
}
