/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:57:09 by ggaribot          #+#    #+#             */
/*   Updated: 2025/01/28 10:08:03 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

int	is_empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n'))
		i++;
	return (line[i] == '\0');
}

char	*trim_whitespace(char *str)
{
	char	*start;
	char	*end;
	char	*result;
	int		len;

	if (!str)
		return (NULL);
	// Find start
	start = str;
	while (*start && (*start == ' ' || *start == '\t'))
		start++;
	// Find end
	end = start + ft_strlen(start) - 1;
	while (end > start && (*end == ' ' || *end == '\t' || *end == '\n'))
		end--;
	// Calculate length and allocate
	len = (end - start) + 2;
	result = malloc(len);
	if (!result)
		return (NULL);
	// Copy and null terminate
	ft_strlcpy(result, start, len);
	return (result);
}

int	check_file_extension(char *filename)
{
	char	*dot;

	dot = ft_strrchr(filename, '.');
	if (!dot || ft_strcmp(dot, ".cub") != 0)
		return (0);
	return (1);
}

int	count_map_rows(char *filename)
{
	int		fd;
	int		height;
	char	*line;

	height = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (-1);
	while ((line = get_next_line(fd)))
	{
		if (!is_empty_line(line))
			height++;
		free(line);
	}
	close(fd);
	return (height);
}

int	get_map_width(char *filename)
{
	int		fd;
	int		width;
	int		len;
	char	*line;

	width = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (-1);
	while ((line = get_next_line(fd)))
	{
		if (!is_empty_line(line))
		{
			len = ft_strlen(line);
			if (len > width)
				width = len;
		}
		free(line);
	}
	close(fd);
	return (width);
}
