/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_textures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:59:15 by ggaribot          #+#    #+#             */
/*   Updated: 2025/01/17 15:23:32 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

static int	is_valid_rgb(int r, int g, int b)
{
	return (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255);
}

static int	parse_rgb_values(char *str, int *r, int *g, int *b)
{
	char	**split;
	char	*temp;
	int		valid;

	split = ft_split(str, ',');
	if (!split || !split[0] || !split[1] || !split[2] || split[3])
	{
		ft_free_split(split);
		return (0);
	}
	valid = 1;
	// Validate R
	*r = ft_atoi(split[0]);
	temp = ft_itoa(*r);
	valid = (temp && ft_strcmp(temp, split[0]) == 0);
	free(temp);
	// Validate G
	*g = ft_atoi(split[1]);
	temp = ft_itoa(*g);
	valid &= (temp && ft_strcmp(temp, split[1]) == 0);
	free(temp);
	// Validate B
	*b = ft_atoi(split[2]);
	temp = ft_itoa(*b);
	valid &= (temp && ft_strcmp(temp, split[2]) == 0);
	free(temp);
	ft_free_split(split);
	return (valid);
}

int	parse_color(char *line, t_map *map)
{
	char	**split;
	int		r;
	int		g;
	int		b;

	split = ft_split(line, ' ');
	if (!split || !split[1] || ft_strlen(split[0]) != 1)
	{
		ft_free_split(split);
		return (0);
	}
	if (!parse_rgb_values(split[1], &r, &g, &b) || !is_valid_rgb(r, g, b))
	{
		ft_free_split(split);
		return (0);
	}
	if (split[0][0] == 'F')
		map->floor_color = (r << 16) | (g << 8) | b;
	else if (split[0][0] == 'C')
		map->ceil_color = (r << 16) | (g << 8) | b;
	else
	{
		ft_free_split(split);
		return (0);
	}
	ft_free_split(split);
	return (1);
}

static int	set_texture_path(char *identifier, char *path, t_map *map)
{
	if (ft_strcmp(identifier, "NO") == 0)
		map->north.path = ft_strdup(path);
	else if (ft_strcmp(identifier, "SO") == 0)
		map->south.path = ft_strdup(path);
	else if (ft_strcmp(identifier, "WE") == 0)
		map->west.path = ft_strdup(path);
	else if (ft_strcmp(identifier, "EA") == 0)
		map->east.path = ft_strdup(path);
	else
		return (0);
	return (1);
}

int	parse_texture(char *line, t_map *map)
{
	char	**split;
	int		result;

	split = ft_split(line, ' ');
	if (!split || !split[1])
	{
		ft_free_split(split);
		return (0);
	}
	result = set_texture_path(split[0], split[1], map);
	ft_free_split(split);
	return (result);
}
