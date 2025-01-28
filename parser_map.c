/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:58:34 by ggaribot          #+#    #+#             */
/*   Updated: 2025/01/28 16:02:04 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

static int is_map_character(char c)
{
    return (c == '0' || c == '1' || c == ' ' || 
            c == 'N' || c == 'S' || c == 'E' || c == 'W');
}


static int is_valid_char(char c)
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
    
    max_width = 0;
    *height = 0;
    
    current = map_lines;
    while (current)
    {
        char *line = (char *)current->content;
        int line_len = ft_strlen(line);
        max_width = line_len > max_width ? line_len : max_width;
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
        if (!is_valid_char(trimmed[x]))
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
            if (is_map_character(line[x]))
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

int parse_file(char *filename, t_game *game)
{
    int fd;
    char *line;
    t_list *map_lines = NULL;
    int parsing_map = 0;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (clean_exit_msg("Cannot open file", NULL));

    while ((line = get_next_line(fd)))
    {
        if (!parsing_map)
        {
            if (is_map_line(line))
            {
                if (!check_required_elements(&game->map))
                {
                    free(line);
                    return (clean_exit_msg("Missing required elements", game));
                }
                parsing_map = 1;
            }
            else
            {
                if (!parse_element(line, &game->map))
                {
                    free(line);
                    return (clean_exit_msg("Invalid element", game));
                }
            }
        }

        if (parsing_map)
            ft_lstadd_back(&map_lines, ft_lstnew(ft_strdup(line)));

        free(line);
    }

    // Validate map continuity before parsing content
    if (!validate_map_continuity(map_lines))
    {
        ft_lstclear(&map_lines, free);
        return (clean_exit_msg("Invalid map: contains empty lines", game));
    }

    // Parse map content only if continuity check passes
    if (parsing_map && map_lines)
    {
        if (!parse_map_content(map_lines, &game->map))
        {
            ft_lstclear(&map_lines, free);
            return (clean_exit_msg("Invalid map", game));
        }
        
        // Add player validation check
        if (!validate_map(&game->map, &game->player))
        {
            ft_lstclear(&map_lines, free);
            return (clean_exit_msg("Invalid map: missing or invalid player position", game));
        }
    }

    ft_lstclear(&map_lines, free);
    close(fd);
    return (0);
}