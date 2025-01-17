/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:58:34 by ggaribot          #+#    #+#             */
/*   Updated: 2025/01/17 15:27:12 by ggaribot         ###   ########.fr       */
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

int	parse_map_line(char *line, t_map *map, int y)
{
	int		x;
	char	*trimmed;
	int		ret;

	x = 0;
	trimmed = trim_whitespace(line);
	if (!trimmed)
		return (0);
	ret = 1;
	while (trimmed[x] && x < map->width)
	{
		if (!is_valid_char(trimmed[x]))
		{
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

static int	init_map(t_map *map, char *filename)
{
	map->height = count_map_rows(filename);
	if (map->height <= 0)
		return (0);
	map->width = get_map_width(filename);
	if (map->width <= 0)
		return (0);
	if (!allocate_map(map))
		return (0);
	return (1);
}

int	parse_map(char *filename, t_game *game)
{
	int		fd;
	char	*line;
	int		y;

	if (!init_map(&game->map, filename))
		return (0);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	y = 0;
	while ((line = get_next_line(fd)) && y < game->map.height)
	{
		if (!is_empty_line(line))
		{
			if (!parse_map_line(line, &game->map, y))
			{
				free(line);
				close(fd);
				return (0);
			}
			y++;
		}
		free(line);
	}
	close(fd);
	return (1);
}
