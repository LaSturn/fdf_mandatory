#include "fdf.h"

/**
 * @brief Displays the z-coordinate matrix data.
 *
 * @param map The map structure containing the z-coordinates.
 */
void	display_z_coordinates(t_map *map)
{
	int	x;
	int	y;

	ft_printf("\n=== Z-COORDINATES MATRIX (%d x %d) ===\n", map->map_width,
		map->map_height);
	y = 0;
	while (y < map->map_height)
	{
		x = 0;
		while (x < map->map_width)
		{
			ft_printf("%2d ", map->z_ij[y][x]);
			x++;
		}
		ft_printf("\n");
		y++;
	}
}

/**
 * @brief Displays the color matrix data.
 *
 * @param map The map structure containing the color values.
 */
void	display_color_matrix(t_map *map)
{
	int	x;
	int	y;

	ft_printf("\n=== COLOR MATRIX (%d x %d) ===\n",
		map->map_width, map->map_height);
	ft_printf("(Colors in hexadecimal, FFFFFF = white/default)\n");
	y = 0;
	while (y < map->map_height)
	{
		x = 0;
		while (x < map->map_width)
		{
			if (map->color[y][x] == WHITE)
				ft_printf("FFFFFF ");
			else
				ft_printf("%06X ", map->color[y][x]);
			x++;
		}
		ft_printf("\n");
		y++;
	}
}

/**
 * @brief Displays the map properties.
 *
 * @param map The map structure containing the properties.
 */
void	display_map_properties(t_map *map)
{
	ft_printf("\n=== MAP PROPERTIES ===\n");
	ft_printf("Width: %d\n", map->map_width);
	ft_printf("Height: %d\n", map->map_height);
	if (map->is_color)
		ft_printf("Has custom colors: Yes\n");
	else
		ft_printf("Has custom colors: No\n");
	ft_printf("============================\n\n");
}

/**
 * @brief Displays the content of the z_ij and color arrays.
 *
 * This function visualizes the input data for debugging purposes.
 *
 * @param map The map structure containing all map data.
 */
void	display_map_data(t_map *map)
{
	if (!map || !map->z_ij || !map->color)
	{
		ft_printf("Error: Cannot display map data - Map not initialized\n");
		return ;
	}
	display_z_coordinates(map);
	display_color_matrix(map);
	display_map_properties(map);
}
