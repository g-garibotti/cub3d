/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 13:00:00 by ggaribot          #+#    #+#             */
/*   Updated: 2025/01/28 16:30:27 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"
 /*
static int is_wall(char c)
{
    return (c == '1' || c == ' ');
}
*/

static int check_surroundings(t_map *map, int x, int y)
{
    size_t line_len;
    size_t wall_pos;
    const char *first_wall;

    // Check if position is at map boundaries
    if (x == 0 || x == map->width - 1 || y == 0 || y == map->height - 1)
        return (0);

    line_len = ft_strlen(map->grid[y]);
    first_wall = ft_strchr(map->grid[y], '1');
    wall_pos = first_wall ? (size_t)(first_wall - map->grid[y]) : line_len;

    // For leading spaces, we only need to check right, up, and down
    if ((size_t)x < wall_pos)
    {
        // This is in the leading spaces section, no need to check left
        return (map->grid[y][x + 1] != ' ' && 
                map->grid[y - 1][x] != ' ' && 
                map->grid[y + 1][x] != ' ');
    }

    // Normal check for non-leading spaces
    return (map->grid[y][x - 1] != ' ' && 
            map->grid[y][x + 1] != ' ' && 
            map->grid[y - 1][x] != ' ' && 
            map->grid[y + 1][x] != ' ');
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

int validate_map(t_map *map, t_player *player)
{
    int x;
    int y;
    int player_count;

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
                    return (0);  // Invalid surroundings
                
                if (ft_strchr("NSEW", map->grid[y][x]))
                {
                    player_count++;
                    player->orientation = map->grid[y][x];
                    set_player_position(player, x, y);  // Set player position
                }
            }
        }
    }
    return (player_count == 1);
}
