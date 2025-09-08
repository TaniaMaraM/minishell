/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_char_checks.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 21:30:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/08 21:21:38 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

int	is_metacharacter(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}
