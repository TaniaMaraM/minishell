/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:17:22 by tmarcos           #+#    #+#             */
/*   Updated: 2025/08/05 14:24:43 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

static int	ft_find_newline(const char *str)
{
	int	i;

	if (!str)
		return (-1);
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

static char	*ft_extract_line(char *keep)
{
	char	*line;
	int		newline_index;

	if (!keep)
		return (NULL);
	newline_index = ft_find_newline(keep);
	if (newline_index == -1)
		return (ft_substr(keep, 0, ft_strlen(keep)));
	line = ft_substr(keep, 0, newline_index + 1);
	return (line);
}

static char	*ft_update_keep(char *keep)
{
	char	*new_keep;
	int		newline_index;
	int		len;

	if (!keep)
		return (NULL);
	newline_index = ft_find_newline(keep);
	if (newline_index == -1 || keep[newline_index + 1] == '\0')
	{
		free(keep);
		return (NULL);
	}
	len = ft_strlen(keep) - (newline_index + 1);
	new_keep = ft_substr(keep, newline_index + 1, len);
	if (!new_keep)
	{
		free(keep);
		return (NULL);
	}
	free(keep);
	return (new_keep);
}

static char	*ft_read_file(int fd, char *keep)
{
	char	*buffer;
	int		bytes_read;
	char	*temp;

	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		buffer[bytes_read] = '\0';
		temp = keep;
		keep = ft_strjoin(keep, buffer);
		free(temp);
		if (!keep || ft_find_newline(keep) != -1)
			break ;
	}
	free(buffer);
	if (bytes_read < 0)
		return (free(keep), NULL);
	return (keep);
}

char	*get_next_line(int fd)
{
	static char	*keep;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	keep = ft_read_file(fd, keep);
	if (!keep)
		return (NULL);
	line = ft_extract_line(keep);
	keep = ft_update_keep(keep);
	return (line);
}
