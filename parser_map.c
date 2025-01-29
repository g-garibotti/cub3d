/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:58:34 by ggaribot          #+#    #+#             */
/*   Updated: 2025/01/29 12:17:41 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

static int is_map_char(char c)
{
    return (c == '0' || c == '1' || c == ' ' || 
            c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

int is_map_line(char *line)
{
    while (*line && (*line == ' ' || *line == '\t'))
        line++;
    return (*line == '1' || *line == '0' || *line == ' ');
}

static int calculate_map_dimensions(t_list *map_lines, int *width, int *height)
{
    t_list *current;
    int max_width;
    int line_len;
    char *line;
    
    max_width = 0;
    *height = 0;
    
    current = map_lines;
    while (current)
    {
        line = (char *)current->content;
        // Count actual content length including spaces
        line_len = ft_strlen(line);
        if (line_len > max_width)
            max_width = line_len;
        (*height)++;
        current = current->next;
    }
    
    *width = max_width;
    return (*width > 0 && *height > 0);
}

static int validate_map_borders(char **grid, int width, int height)
{
    int x, y;

    // Check first and last rows
    for (x = 0; x < width; x++)
    {
        if (grid[0][x] != '1' && grid[0][x] != ' ')
            return (0);
        if (grid[height - 1][x] != '1' && grid[height - 1][x] != ' ')
            return (0);
    }

    // Check first and last columns
    for (y = 0; y < height; y++)
    {
        if (grid[y][0] != '1' && grid[y][0] != ' ')
            return (0);
        if (grid[y][width - 1] != '1' && grid[y][width - 1] != ' ')
            return (0);
    }

    // Check that spaces are surrounded by walls
    for (y = 1; y < height - 1; y++)
    {
        for (x = 1; x < width - 1; x++)
        {
            if (grid[y][x] == '0' || ft_strchr("NSEW", grid[y][x]))
            {
                if (grid[y-1][x] == ' ' || grid[y+1][x] == ' ' ||
                    grid[y][x-1] == ' ' || grid[y][x+1] == ' ')
                    return (0);
            }
        }
    }
    
    return (1);
}

int parse_map_line(char *line, t_map *map, int y)
{
    int x;
    char *trimmed;
    int ret;

    trimmed = trim_whitespace(line);
    if (!trimmed)
        return (0);

    x = 0;
    ret = 1;

    // Fill the line with the actual characters
    while (trimmed[x] && x < map->width)
    {
        if (!is_map_char(trimmed[x]))
        {
            ret = 0;
            break;
        }
        map->grid[y][x] = trimmed[x];
        x++;
    }

    // Fill the rest of the line with walls (1's instead of spaces)
    while (ret && x < map->width)
    {
        map->grid[y][x] = '1';  // Changed from space to '1'
        x++;
    }

    free(trimmed);
    return ret;
}

static int parse_map_content(t_list *map_lines, t_map *map)
{
    t_list *current;
    int y;
    
    if (!calculate_map_dimensions(map_lines, &map->width, &map->height))
        return (0);
        
    // Allocate grid
    map->grid = malloc(sizeof(char *) * (map->height + 1));
    if (!map->grid)
        return (0);
        
    // Initialize all lines with spaces
    for (y = 0; y < map->height; y++)
    {
        map->grid[y] = malloc(sizeof(char) * (map->width + 1));
        if (!map->grid[y])
            return (0);
        ft_memset(map->grid[y], ' ', map->width);
        map->grid[y][map->width] = '\0';
    }
    map->grid[map->height] = NULL;
    
    // Fill in map content
    current = map_lines;
    y = 0;
    while (current && y < map->height)
    {
        char *line = (char *)current->content;
        int x = 0;
        while (line[x] && x < map->width)
        {
            if (is_map_char(line[x]))
                map->grid[y][x] = line[x];
            x++;
        }
        current = current->next;
        y++;
    }
    
    return (validate_map_borders(map->grid, map->width, map->height));
}

static int validate_map_continuity(t_list *map_lines)
{
    t_list *current = map_lines;
    int found_map_start = 0;
    int found_empty_line = 0;
    
    while (current)
    {
        char *line = (char *)current->content;
        
        // Skip leading empty lines before map starts
        if (!found_map_start && is_empty_line(line))
        {
            current = current->next;
            continue;
        }
        
        // Once we find a map line, set the flag
        if (!found_map_start && !is_empty_line(line))
        {
            found_map_start = 1;
        }
        
        // After map has started, check for empty lines
        if (found_map_start)
        {
            if (is_empty_line(line))
            {
                found_empty_line = 1;
            }
            else if (found_empty_line)
            {
                // Found a non-empty line after an empty line within the map
                return 0;
            }
        }
        
        current = current->next;
    }
    
    return 1;
}

int parse_scene_elements(int fd, t_game *game)
{
    char *line;
    int parsing_map = 0;

    game->temp_map_line = NULL;
    while ((line = get_next_line(fd)) && !parsing_map)
    {
        if (is_map_line(line))
        {
            if (!check_required_elements(&game->map))
                clean_exit_msg("Missing required elements", game);
            game->temp_map_line = line;  // Store in game structure
            parsing_map = 1;
            continue;
        }
        else  // Try to parse as texture or color
        {
            if (!parse_element(line, &game->map))
                clean_exit_msg("Invalid scene element", game);
            free(line);
        }
    }
    return (0);
}

int parse_and_validate_map(int fd, t_game *game)
{
    t_list *map_lines = NULL;
    char *line;

    // Add the first map line to our list
    if (game->temp_map_line)
        ft_lstadd_back(&map_lines, ft_lstnew(ft_strdup(game->temp_map_line)));

    // Collect remaining map lines
    while ((line = get_next_line(fd)))
    {
        if (!is_empty_line(line))
            ft_lstadd_back(&map_lines, ft_lstnew(ft_strdup(line)));
        free(line);
    }

    // Validate map structure and content
    if (!validate_map_continuity(map_lines))
        clean_exit_msg("Invalid map: contains empty lines", game);
    if (!parse_map_content(map_lines, &game->map))
        clean_exit_msg("Invalid map structure", game);
    if (!validate_map(&game->map, &game->player))
        clean_exit_msg("Invalid map: player position error", game);

    ft_lstclear(&map_lines, free);
    return (0);
}

int parse_file(char *file, t_game *game)
{
    int fd;

    if (!check_file_extension(file))
        clean_exit_msg("Invalid file extension", game);

    fd = open(file, O_RDONLY);
    if (fd < 0)
        clean_exit_msg("Cannot open file", game);

    // Parse scene elements (textures and colors)
    parse_scene_elements(fd, game);
    
    // Parse and validate map
    parse_and_validate_map(fd, game);
    
    // Success cleanup
    free(game->temp_map_line);
    game->temp_map_line = NULL;
    close(fd);
    return (0);
}
