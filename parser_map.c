/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:58:34 by ggaribot          #+#    #+#             */
/*   Updated: 2025/01/28 10:58:50 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

static int	allocate_map(t_map *map)
{
	int	i;

	map->grid = malloc(sizeof(char *) * (map->height + 1));
	if (!map->grid)
		return (0);
	i = 0;
	while (i < map->height)
	{
		map->grid[i] = malloc(sizeof(char) * (map->width + 1));
		if (!map->grid[i])
		{
			while (--i >= 0)
				free(map->grid[i]);
			free(map->grid);
			return (0);
		}
		map->grid[i][map->width] = '\0';
		i++;
	}
	map->grid[map->height] = NULL;
	return (1);
}

static int	is_valid_char(char c)
{
	if (c == '0' || c == '1' || c == ' ' || c == 'N' || c == 'S' || c == 'E'
		|| c == 'W')
		return (1);
	return (0);
}

// In parse_map_line:
int	parse_map_line(char *line, t_map *map, int y)
{
	int		x;
	char	*trimmed;
	int		ret;

	x = 0;
	printf("Original line: '%s'\n", line);
	trimmed = trim_whitespace(line);
	if (!trimmed)
		return (0);
	printf("Trimmed line: '%s'\n", trimmed);
	ret = 1;
	while (trimmed[x] && x < map->width)
	{
		printf("Checking char '%c' at position %d\n", trimmed[x], x);
		if (!is_valid_char(trimmed[x]))
		{
			printf("Invalid character found: '%c'\n", trimmed[x]);
			ret = 0;
			break ;
		}
		map->grid[y][x] = trimmed[x];
		x++;
	}
	while (ret && x < map->width)
		map->grid[y][x++] = ' ';
	free(trimmed);
	return (ret);
}

// In init_map:
static int	init_map(t_map *map, char *filename)
{
	map->height = count_map_rows(filename);
	printf("Map height: %d\n", map->height);
	if (map->height <= 0)
		return (0);
	map->width = get_map_width(filename);
	printf("Map width: %d\n", map->width);
	if (map->width <= 0)
		return (0);
	if (!allocate_map(map))
		return (0);
	return (1);
}

int parse_map(char *filename, t_game *game)
{
    int     fd;
    char    *line;
    int     y;
    int     map_started;
    int     found_empty;

    if (!init_map(&game->map, filename))
        return (0);
    
    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (0);

    // Skip texture and color definitions
    map_started = 0;
    found_empty = 0;
    y = 0;
    
    while ((line = get_next_line(fd)) && y < game->map.height)
    {
        if (is_empty_line(line))
        {
            if (map_started) // If we find an empty line after map started, it's invalid
                found_empty = 1;
        }
        else
        {
            char *trimmed = trim_whitespace(line);
            if (trimmed)
            {
                // Check if this line starts with a valid map character
                if (trimmed[0] == '1' || trimmed[0] == '0' || trimmed[0] == ' ')
                {
                    if (found_empty)  // If we found an empty line before this map line
                    {
                        free(trimmed);
                        free(line);
                        close(fd);
                        return (0);    // Map is invalid
                    }
                    map_started = 1;
                    if (!parse_map_line(line, &game->map, y))
                    {
                        free(trimmed);
                        free(line);
                        close(fd);
                        return (0);
                    }
                    y++;
                }
                else if (map_started) // Invalid character found after map started
                {
                    free(trimmed);
                    free(line);
                    close(fd);
                    return (0);
                }
                free(trimmed);
            }
        }
        free(line);
    }
    close(fd);
    return (y == game->map.height); // Ensure we read all expected lines
}
