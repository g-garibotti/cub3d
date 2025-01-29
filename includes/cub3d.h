#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include "../minilibx-linux/mlx.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

/* Constants */
# define S_W 1900             // screen width
# define S_H 1000             // screen height
# define TILE_SIZE 30         // tile size
# define FOV 60               // field of view
# define ROTATION_SPEED 0.045 // rotation speed
# define PLAYER_SPEED 4       // player speed
# define PI 3.14159265359
# define RAD (PI / 180)

/* Structures */
typedef struct s_player
{
	double pos_x;     // player x position (using double for smooth movement)
	double pos_y;     // player y position (using double for smooth movement)
	double dir_x;     // direction vector x
	double dir_y;     // direction vector y
	double angle;     // player angle
	double plane_x;   // camera plane x (for raycasting)
	double plane_y;   // camera plane y (for raycasting)
	char orientation; // initial orientation (N,S,E,W)
	float fov_rd;     // field of view in radians
	int rot;          // rotation flag
	int l_r;          // left/right movement flag
	int u_d;          // up/down movement flag
}		t_player;

typedef struct s_ray
{
	double ray_ngl;  // ray angle
	double distance; // distance to wall
	int flag;        // wall flag
	double wall_x;   // wall hit position for texturing
	int wall_height; // height of wall to render
	int side;        // which side of wall was hit (N,S,E,W)
}		t_ray;

typedef struct s_texture
{
	void *img;    // texture image pointer
	char *path;   // path to texture file
	int *addr;    // texture data address
	int width;    // texture width
	int height;   // texture height
	int bpp;      // bits per pixel
	int line_len; // line length
	int endian;   // endianness
}		t_texture;

typedef struct s_map
{
	char **grid;     // 2D map array
	int width;       // map width
	int height;      // map height
	t_texture north; // north wall texture
	t_texture south; // south wall texture
	t_texture west;  // west wall texture
	t_texture east;  // east wall texture
	int floor_color; // floor color (RGB)
	int ceil_color;  // ceiling color (RGB)
}		t_map;

typedef struct s_game
{
	void *mlx;       // MLX pointer
	void *win;       // window pointer
	void *img;       // main image pointer
	int *addr;       // image data address
	int bpp;         // bits per pixel
	int line_len;    // line length
	int endian;      // endianness
	t_player player; // player data
	t_map map;       // map data
	t_ray *rays;     // array of rays
	char *temp_map_line; // temporary storage for map parsing
}		t_game;

/* Function Prototypes */

// Core parsing functions
int		parse_file(char *filename, t_game *game);
int		validate_map(t_map *map, t_player *player);
int		parse_texture(char *line, t_map *map);
int		parse_color(char *line, t_map *map);

// Map parsing functions
int		parse_map(char *filename, t_game *game);
int		parse_map_line(char *line, t_map *map, int y);
int		count_map_rows(char *filename);
int		get_map_width(char *filename);

// Parser utility functions
int		is_empty_line(char *line);
char	*trim_whitespace(char *str);
int		check_file_extension(char *filename);
int		check_texture_file(char *path);
int		is_valid_number(char *str);
int		validate_textures(t_map *map);
int		check_file_empty(char *filename);
int		check_map_size(t_map *map);
void	free_split(char **split);
int	check_required_elements(t_map *map);
int	parse_element(char *line, t_map *map);
//int is_map_line(char *line);

// Ray casting functions
void	cast_rays(t_game *game);
void	calculate_wall_height(t_game *game, t_ray *ray);
void	render_walls(t_game *game);

// Player movement functions
void	move_player(t_game *game);
void	rotate_player(t_game *game);
int		check_collision(t_game *game, double new_x, double new_y);

// Texture handling
int		load_textures(t_game *game);
void	apply_texture(t_game *game, int x, t_ray *ray);

// Utils and error handling
void	free_game(t_game *game);
void	error_exit(char *message, t_game *game);
void	init_game(t_game *game);

// Hook functions
void	set_hooks(t_game *game);

// Cleanup functions
void	clean_game(t_game *game);
int		clean_exit_msg(char *msg, t_game *game);

#endif