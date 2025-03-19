#include "fdf.h"

int	main(int ac, char **av)
{
	t_map	*map;
	char	*file;

	map = NULL;
	if (ac != 2)
		print_error_and_exit("Nombre d'arguments invalide", 1);
	check_file_readable(av[1]);
	file = av[1];
	map = parse_map_dimensions(file);
	if (!map)
		print_error_and_exit("Impossible d'initialiser la carte", 1);
	get_map(file, map);
	get_z_min_max(map);
	ft_printf("taille map: largueur %d, hauteur %d\n", map->map_width,
		map->map_height);
	start_mlx(map);
	return (0);
}
