/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 13:15:08 by ggaribot          #+#    #+#             */
/*   Updated: 2025/01/17 15:28:03 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

static void	run_test(char *test_file, char *test_name, int expected_result)
{
	t_game	game;
	int		result;

	ft_putstr_fd("\nRunning test: ", 1);
	ft_putstr_fd(test_name, 1);
	ft_putstr_fd("\nTest file: ", 1);
	ft_putstr_fd(test_file, 1);
	ft_putstr_fd("\n", 1);
	ft_memset(&game, 0, sizeof(t_game));
	result = parse_file(test_file, &game);
	ft_putstr_fd("Expected result: ", 1);
	ft_putnbr_fd(expected_result, 1);
	ft_putstr_fd("\nActual result: ", 1);
	ft_putnbr_fd(result, 1);
	ft_putstr_fd("\nTest ", 1);
	if (result == expected_result)
		ft_putstr_fd("PASSED ✅\n", 1);
	else
		ft_putstr_fd("FAILED ❌\n", 1);
	clean_game(&game);
}

int	main(void)
{
	ft_putstr_fd("=== Starting Cub3D Parser Tests ===\n", 1);
	run_test("tests/test_valid.cub", "Valid Basic Map", 0);
	run_test("tests/test_invalid_texture.cub", "Invalid Texture Paths", 1);
	run_test("tests/test_invalid_rgb.cub", "Invalid RGB Values", 1);
	run_test("tests/test_invalid_map_open.cub", "Invalid Map Layout", 1);
	run_test("tests/test_multiple_players.cub", "Multiple Players", 1);
	run_test("tests/test_no_player.cub", "Missing Player", 1);
	run_test("tests/test_invalid_chars.cub", "Invalid Characters", 1);
	run_test("tests/test_empty_lines.cub", "Empty Lines in Map", 1);
	run_test("tests/test_missing_config.cub", "Missing Configuration", 1);
	ft_putstr_fd("\n=== Testing Complete ===\n", 1);
	return (0);
}

/*
int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		return (clean_exit_msg("Usage: ./cub3D <map.cub>", NULL));
	ft_memset(&game, 0, sizeof(t_game));
	if (parse_file(argv[1], &game))
		return (clean_exit_msg("Parsing error", &game));
	// Continue with game initialization...
	clean_game(&game);
	return (0);
}
*/
