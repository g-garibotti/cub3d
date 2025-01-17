/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:14:27 by ggaribot          #+#    #+#             */
/*   Updated: 2025/01/17 14:23:42 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

static void	clean_texture(void *mlx, t_texture *texture)
{
	if (!texture)
		return ;
	if (texture->path)
	{
		free(texture->path);
		texture->path = NULL;
	}
	if (texture->img && mlx)
	{
		mlx_destroy_image(mlx, texture->img);
		texture->img = NULL;
	}
	texture->addr = NULL;
	texture->width = 0;
	texture->height = 0;
	texture->bpp = 0;
	texture->line_len = 0;
	texture->endian = 0;
}

static void	clean_map(void *mlx, t_map *map)
{
	int	i;

	if (!map)
		return ;
	if (map->grid)
	{
		i = 0;
		while (i < map->height && map->grid[i])
		{
			free(map->grid[i]);
			map->grid[i] = NULL;
			i++;
		}
		free(map->grid);
		map->grid = NULL;
	}
	clean_texture(mlx, &map->north);
	clean_texture(mlx, &map->south);
	clean_texture(mlx, &map->east);
	clean_texture(mlx, &map->west);
	map->width = 0;
	map->height = 0;
	map->floor_color = -1;
	map->ceil_color = -1;
}

static void	clean_window(t_game *game)
{
	if (!game)
		return ;
	if (game->img && game->mlx)
	{
		mlx_destroy_image(game->mlx, game->img);
		game->img = NULL;
		game->addr = NULL;
	}
	if (game->win && game->mlx)
	{
		mlx_destroy_window(game->mlx, game->win);
		game->win = NULL;
	}
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		game->mlx = NULL;
	}
}

void	clean_game(t_game *game)
{
	if (!game)
		return ;
	if (game->rays)
	{
		free(game->rays);
		game->rays = NULL;
	}
	clean_map(game->mlx, &game->map);
	clean_window(game);
	game->bpp = 0;
	game->line_len = 0;
	game->endian = 0;
}

int	clean_exit_msg(char *msg, t_game *game)
{
	if (msg)
	{
		ft_putstr_fd(msg, 2);
		ft_putstr_fd("\n", 2);
	}
	if (game)
		clean_game(game);
	exit(1);
	return (1);
}
