/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:57:09 by ggaribot          #+#    #+#             */
/*   Updated: 2025/01/28 14:08:39 by ggaribot         ###   ########.fr       */
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

int count_map_rows(char *filename)
{
    int     fd;
    int     height;
    char    *line;
    int     map_section;
    int     empty_after_map;

    height = 0;
    map_section = 0;
    empty_after_map = 0;
    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (-1);

    // Skip non-map content (textures and colors)
    while ((line = get_next_line(fd)))
    {
        // Skip empty lines and texture/color configurations
        if (is_empty_line(line))
        {
            if (map_section)
                empty_after_map = 1;
            free(line);
            continue;
        }

        char *trimmed = trim_whitespace(line);
        if (trimmed)
        {
            // Check if this is a map line (starts with 0, 1, or space)
            if (trimmed[0] == '1' || trimmed[0] == '0' || trimmed[0] == ' ')
            {
                if (empty_after_map) // Found a map line after empty line
                {
                    free(trimmed);
                    free(line);
                    close(fd);
                    return (-1);
                }
                map_section = 1;
                height++;
            }
            else if (map_section) // Found non-map line after map started
            {
                free(trimmed);
                free(line);
                close(fd);
                return (-1);
            }
            free(trimmed);
        }
        free(line);
    }
    close(fd);
    return (height > 0 ? height : -1);
}

int get_map_width(char *filename)
{
    int     fd;
    int     width;
    int     len;
    char    *line;
    char    *trimmed;
    int     in_map_section;
    int     line_count;

    width = 0;
    in_map_section = 0;
    line_count = 0;
    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (-1);

    // Skip non-map content
    while ((line = get_next_line(fd)))
    {
        if (is_empty_line(line))
        {
            free(line);
            continue;
        }

        trimmed = trim_whitespace(line);
        if (trimmed)
        {
            // Check if this is the start of map section
            if ((trimmed[0] == '1' || trimmed[0] == '0' || trimmed[0] == ' ') && !in_map_section)
            {
                in_map_section = 1;
                len = ft_strlen(trimmed);
                if (len > width)
                    width = len;
                line_count++;
            }
            // Continue counting width if we're in map section
            else if (in_map_section)
            {
                len = ft_strlen(trimmed);
                if (len > width)
                    width = len;
                line_count++;
            }
            free(trimmed);
        }
        free(line);
    }
    close(fd);
    return (line_count > 0 ? width : -1);
}
