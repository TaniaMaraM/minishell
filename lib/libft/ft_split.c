/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:02:36 by tmarcos           #+#    #+#             */
/*   Updated: 2024/12/10 23:17:35 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	words_count(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
		{
			in_word = 0;
		}
		s++;
	}
	return (count);
}

static char	*allocate_word(const char *s, int start, int len)
{
	char	*word;

	word = (char *)malloc((len + 1) * sizeof(char));
	if (!word)
		return (NULL);
	ft_strlcpy (word, s + start, len + 1);
	return (word);
}

static void	free_all(char **split, int words)
{
	int	i;

	i = 0;
	while (i < words)
	{
		free (split[i]);
		i++;
	}
	free(split);
}

static int	copy_to_split(char **split, const char *s, char c, int count_word)
{
	int	i;
	int	j;
	int	start;
	int	len;

	i = 0;
	j = 0;
	while (s[i] && j < count_word)
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		len = i - start;
		split[j] = allocate_word(s, start, len);
		if (!split[j])
		{
			free_all(split, j);
			return (0);
		}
		j++;
	}
	split[j] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	int		count_word;
	char	**split;

	if (!s)
		return (NULL);
	count_word = words_count(s, c);
	split = (char **)malloc((count_word + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	if (!copy_to_split (split, s, c, count_word))
	{
		return (NULL);
	}
	return (split);
}
