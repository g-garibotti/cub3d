/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 13:15:08 by ggaribot          #+#    #+#             */
/*   Updated: 2025/01/28 09:58:58 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		clean_exit_msg("Usage: ./cub3D <map.cub>", NULL);
	ft_memset(&game, 0, sizeof(t_game));
	printf("BEFORE PARSE FILE\n");
	if (parse_file(argv[1], &game))
	{
		printf("IN PARSE FILE BEFORE CLEAN\n");
		clean_exit_msg("Parsing error", &game);
		printf("AFTER CLEAN IN PARSE FILE\n");
	}
	printf("BEFORE CLEAN END\n");
	// Continue with game initialization...
	clean_game(&game);
	return (0);
}
