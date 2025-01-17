/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 13:00:00 by ggaribot          #+#    #+#             */
/*   Updated: 2025/01/17 13:16:00 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

static int	check_surroundings(t_map *map, int x, int y)
{
	if (x == 0 || x == map->width - 1 || y == 0 || y == map->height - 1)
		return (0);
	if (map->grid[y][x - 1] == ' ' || map->grid[y][x + 1] == ' ' || map->grid[y
		- 1][x] == ' ' || map->grid[y + 1][x] == ' ')
		return (0);
	return (1);
}

static void	set_player_direction_ns(t_player *player)
{
	if (player->orientation == 'N')
	{
		player->dir_x = 0;
		player->dir_y = -1;
		player->plane_x = 0.66;
		player->plane_y = 0;
		player->angle = 270 * RAD;
	}
	if (player->orientation == 'S')
	{
		player->dir_x = 0;
		player->dir_y = 1;
		player->plane_x = -0.66;
		player->plane_y = 0;
		player->angle = 90 * RAD;
	}
}

static void	set_player_direction_ew(t_player *player)
{
	if (player->orientation == 'E')
	{
		player->dir_x = 1;
		player->dir_y = 0;
		player->plane_x = 0;
		player->plane_y = 0.66;
		player->angle = 0;
	}
	if (player->orientation == 'W')
	{
		player->dir_x = -1;
		player->dir_y = 0;
		player->plane_x = 0;
		player->plane_y = -0.66;
		player->angle = 180 * RAD;
	}
}

static void	set_player_position(t_player *player, int x, int y)
{
	player->pos_x = x * TILE_SIZE + TILE_SIZE / 2;
	player->pos_y = y * TILE_SIZE + TILE_SIZE / 2;
	set_player_direction_ns(player);
	set_player_direction_ew(player);
}

int	validate_map(t_map *map, t_player *player)
{
	int	x;
	int	y;
	int	player_count;

	player_count = 0;
	y = -1;
	while (++y < map->height)
	{
		x = -1;
		while (++x < map->width)
		{
			if (map->grid[y][x] == '0' || ft_strchr("NSEW", map->grid[y][x]))
			{
				if (!check_surroundings(map, x, y))
					return (0);
				if (ft_strchr("NSEW", map->grid[y][x]))
				{
					player_count++;
					player->orientation = map->grid[y][x];
					set_player_position(player, x, y);
				}
			}
		}
	}
	return (player_count == 1);
}
