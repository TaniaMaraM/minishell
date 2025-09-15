/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:45:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/15 16:02:05 by rwrobles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Read input from stdin without showing prompt (non-interactive)
 * @return Dynamically allocated string or NULL on EOF
 */
static char	*read_non_interactive(void)
{
	char	*line;
	int		fd;

	fd = STDIN_FILENO;
	line = get_next_line(fd);
	if (line && line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	return (line);
}

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
		line = read_non_interactive();
	}
	return (line);
}

static int	validate_and_process_quotes(char *input, char **processed,
		t_shell *sh)
{
	if (has_unclosed_quotes(input))
	{
		print_error("syntax", "unclosed quotes");
		sh->last_status = EXIT_STATUS_SYNTAX_ERROR;
		return (1);
	}
	*processed = process_quotes(input, sh);
	if (!*processed)
	{
		print_error("quotes", "Failed to process quotes");
		sh->last_status = EXIT_FAILURE;
		return (1);
	}
	return (0);
}

static int	init_lexer_parser(char *processed, t_lexer **lexer,
		t_parser **parser, t_shell *sh)
{
	*lexer = lexer_init(processed);
	if (!*lexer)
	{
		free(processed);
		return (1);
	}
	*parser = parser_init(*lexer, sh);
	if (!*parser)
	{
		lexer_destroy(*lexer);
		free(processed);
		return (1);
	}
	return (0);
}

/**
 * @brief Process a single input line through tokenizing, parsing and execution
 * @param input Input line from user
 * @param sh Shell context
 */
void	process_line(char *input, t_shell *sh)
{
	char		*processed_input;
	t_lexer		*lexer;
	t_parser	*parser;
	t_cmd		*cmd_list;
	int			parser_error;

	if (validate_and_process_quotes(input, &processed_input, sh))
		return ;
	if (init_lexer_parser(processed_input, &lexer, &parser, sh))
	{
		free(processed_input);
		return ;
	}
	free(processed_input);
	cmd_list = parser_parse(parser);
	parser_error = parser->error;
	parser_destroy(parser);
	lexer_destroy(lexer);
	if (parser_error || !cmd_list)
	{
		print_error("parser", "Syntax error");
		sh->last_status = EXIT_STATUS_SYNTAX_ERROR;
	}
	else
		sh->last_status = execute_command_list(cmd_list, sh);
	if (cmd_list)
		cmd_destroy_list(cmd_list);
}
