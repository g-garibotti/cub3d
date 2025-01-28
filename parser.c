/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 13:16:23 by ggaribot          #+#    #+#             */
/*   Updated: 2025/01/28 10:09:53 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

static int	check_required_elements(t_map *map)
{
	if (!map->north.path || !map->south.path || !map->west.path
		|| !map->east.path)
		return (0);
	if (map->floor_color == -1 || map->ceil_color == -1)
		return (0);
	return (1);
}

static int	init_parse_vars(t_map *map)
{
	map->north.path = NULL;
	map->south.path = NULL;
	map->west.path = NULL;
	map->east.path = NULL;
	map->floor_color = -1;
	map->ceil_color = -1;
	map->width = 0;
	map->height = 0;
	map->grid = NULL;
	return (1);
}

static int	parse_element(char *line, t_map *map)
{
	char	*trimmed;
	int		ret;

	if (is_empty_line(line))
		return (1);
	trimmed = trim_whitespace(line);
	if (!trimmed)
		return (0);
	ret = 0;
	if (trimmed[0] == 'N' || trimmed[0] == 'S' || trimmed[0] == 'W'
		|| trimmed[0] == 'E')
		ret = parse_texture(trimmed, map);
	else if (trimmed[0] == 'F' || trimmed[0] == 'C')
		ret = parse_color(trimmed, map);
	else if (trimmed[0] == '1' || trimmed[0] == '0' || trimmed[0] == ' ')
		ret = 2;
	free(trimmed);
	return (ret);
}

static int	parse_elements(int fd, t_map *map)
{
	char	*line;
	int		ret;
	int		map_started;

	map_started = 0;
	while ((line = get_next_line(fd)))
	{
		ret = parse_element(line, map);
		if (ret == 0)
		{
			free(line);
			return (0);
		}
		else if (ret == 2)
		{
			map_started = 1;
			break ;
		}
		free(line);
	}
	if (!map_started || !check_required_elements(map))
		return (0);
	return (1);
}

static int	init_parsing(char *filename, t_game *game)
{
	if (!check_file_extension(filename))
		return (clean_exit_msg("Invalid file extension", NULL));
	if (check_file_empty(filename))
		return (1);
	init_parse_vars(&game->map);
	return (0);
}

static int	parse_game_elements(char *filename, t_game *game)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (clean_exit_msg("Cannot open file", NULL));
	if (!parse_elements(fd, &game->map))
	{
		close(fd);
		return (clean_exit_msg("Invalid configuration", game));
	}
	close(fd);
	return (0);
}

static int	parse_game_map(char *filename, t_game *game)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (clean_exit_msg("Cannot open file", game));
	if (!parse_map(filename, game))
	{
		close(fd);
		return (clean_exit_msg("Invalid map", game));
	}
	close(fd);
	if (!validate_map(&game->map, &game->player))
		return (clean_exit_msg("Invalid map layout", game));
	return (0);
}

int	parse_file(char *filename, t_game *game)
{
	if (!filename || !game)
		clean_exit_msg("Invalid parameters", NULL);
	if (init_parsing(filename, game))
		return (1);
	printf("IN PARSE FILE1\n");
	if (parse_game_elements(filename, game))
		return (1);
	printf("IN PARSE FILE2\n");
	if (!validate_textures(&game->map))
		return (1);
	printf("IN PARSE FILE3\n");
	if (parse_game_map(filename, game))
		return (1);
	printf("IN PARSE FILE4\n");
	if (!check_map_size(&game->map))
		return (1);
	printf("IN PARSE FILE5\n");
	return (0);
}
