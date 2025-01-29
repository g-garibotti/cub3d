/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 13:15:08 by ggaribot          #+#    #+#             */
/*   Updated: 2025/01/29 11:00:00 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		clean_exit_msg("Usage: ./cub3D <map.cub>", NULL);
	ft_memset(&game, 0, sizeof(t_game));
	//PARSING
	if (parse_file(argv[1], &game))
		clean_exit_msg("Parsing error", &game);
	printf("PARSING SUCCES\n");
	// Continue with game initialization...
	clean_game(&game);
	return (0);
}
