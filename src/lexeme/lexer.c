/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwrobles <rwrobles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 09:00:00 by rwrobles         #+#    #+#             */
/*   Updated: 2025/09/02 16:30:00 by rwrobles        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "minishell.h"

t_lexer	*lexer_init(const char *input)
{
	t_lexer	*lexer;

	if (!input)
		return (NULL);
	lexer = ft_calloc(1, sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->input = ft_strdup(input);
	if (!lexer->input)
	{
		free(lexer);
		return (NULL);
	}
	lexer->pos = 0;
	lexer->len = ft_strlen(lexer->input);
	return (lexer);
}

void	lexer_destroy(t_lexer *lexer)
{
	if (lexer)
	{
		if (lexer->input)
			free((char *)lexer->input);
		free(lexer);
	}
}
