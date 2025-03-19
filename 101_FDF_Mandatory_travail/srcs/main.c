#include "fdf.h"

int	main(int ac, char **av)
{
	t_map	*map;
	char	*file;

	map = NULL;
	if (ac != 2)
		print_error_and_exit("Nombre d'arguments invalide", 1);
	//ft_printf("Arguments corrects !\n");
	//ft_printf("Initialisation du processus de lecture des données de la carte...\n");
	check_file_readable(av[1]); // attention peut etre a double car dans init_map je fait aussi un check!!!
	//ft_printf("Le fichier %s existe et est lisible !\n", av[1]);
	file = av[1];
	map = parse_map_dimensions(file);
	if (!map)
		print_error_and_exit("Impossible d'initialiser la carte", 1);
	//ft_printf("Carte initialisée avec succès.\n");
	get_map(file, map);
	get_z_min_max(map); // pour obtenir min_z et max_z (pour gerer affichage en suite)
    ft_printf("taille map: largueur %d, hauteur %d\n", map->map_width, map->map_height);
	//ft_printf("Mon min_z est : %d\n", map->min_z);
	//ft_printf("Mon man_z est : %d\n", map->max_z);
	//ft_printf("Dans le main: je suis avant start_mlx");
	start_mlx(map);
	//ft_printf("Dans le main: je suis apres start_mlx");
	//ft_printf("Je suis a la fin de mon main");
	return (0);
}
