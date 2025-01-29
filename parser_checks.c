/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:27:04 by ggaribot          #+#    #+#             */
/*   Updated: 2025/01/29 11:49:37 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

int check_texture_file(char *path)
{
    int     fd;
    char    *extension;

    if (!path)
        return (0);

    // Check file extension
    extension = ft_strrchr(path, '.');
    if (!extension || ft_strcmp(extension, ".xpm") != 0)
        return (0);

    // Try to open the file
    fd = open(path, O_RDONLY);
    if (fd < 0)
        return (0);
    
    close(fd);
    return (1);
}

int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (!ft_isdigit(str[i]) && str[i] != ' ' && str[i] != '\t')
			return (0);
		i++;
	}
	return (i > 0);
}

int	validate_textures(t_map *map)
{
	if (!map->north.path || !map->south.path || !map->east.path
		|| !map->west.path)
		return (clean_exit_msg("Missing texture path", NULL));
	if (!check_texture_file(map->north.path))
		return (clean_exit_msg("Invalid north texture", NULL));
	if (!check_texture_file(map->south.path))
		return (clean_exit_msg("Invalid south texture", NULL));
	if (!check_texture_file(map->east.path))
		return (clean_exit_msg("Invalid east texture", NULL));
	if (!check_texture_file(map->west.path))
		return (clean_exit_msg("Invalid west texture", NULL));
	return (1);
}
/*
int	check_file_empty(char *filename)
{
	int		fd;
	char	buffer[1];
	ssize_t	bytes_read;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (1);
	bytes_read = read(fd, buffer, 1);
	close(fd);
	if (bytes_read <= 0)
		return (clean_exit_msg("File is empty", NULL));
	return (0);
}*/

int	check_map_size(t_map *map)
{
	if (map->width <= 2 || map->height <= 2)
		return (clean_exit_msg("Map too small", NULL));
	if (map->width > 100 || map->height > 100)
		return (clean_exit_msg("Map too large", NULL));
	return (1);
}
