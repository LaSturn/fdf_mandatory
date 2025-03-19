#include "fdf.h"

/**
 * @brief Frees the memory allocated for the map structure.
 *
 * This function deallocates the 2D arrays for z-values and colors,
 * then frees the map structure itself.
 *
 * @param map The map structure to free.
 */
void	free_map_memory(t_map *map)
{
	int	i;

	if (!map)
		return ;
	if (map->z_ij)
	{
		i = -1;
		while (++i < map->map_height)
			if (map->z_ij[i])
				free(map->z_ij[i]);
		free(map->z_ij);
	}
	if (map->color)
	{
		i = -1;
		while (++i < map->map_height)
			if (map->color[i])
				free(map->color[i]);
		free(map->color);
	}
	free(map);
}

/**
 * @brief Frees a dynamically allocated string matrix.
 *
 * This function deallocates each string in the matrix
 * and then frees the matrix itself.
 *
 * @param matrix The string matrix to free.
 */
void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	if (!matrix)
		return ;
	while (matrix[i])
		free(matrix[i++]);
	free(matrix);
}

/**
 * @brief Frees all memory allocated in the display structure.
 *
 * This function properly cleans up all resources used by the display,
 * including MLX resources, transformed points, and map data.
 *
 * @param display The display structure to free.
 */
void	free_all_struct_display(t_display *display)
{
	int	y;
	int	map_height;

	if (!display)
		return ;
	map_height = 0;
	if (display->map)
		map_height = display->map->map_height; // trop long a diviser!!!!!
	if (display->transformed)
	{
		y = 0;
		while (y < map_height)
		{
			if (display->transformed[y])
				free(display->transformed[y]);
			y++;
		}
		free(display->transformed);
		display->transformed = NULL;
	}
	if (display->mlx_ptr && display->window_ptr)
		mlx_destroy_window(display->mlx_ptr, display->window_ptr);
	if (display->mlx_ptr && display->img && display->img->img_ptr)
		mlx_destroy_image(display->mlx_ptr, display->img->img_ptr);
	if (display->img)
		free(display->img);
	if (display->map)
		free_map_memory(display->map);
	if (display->mlx_ptr)
	{
		mlx_destroy_display(display->mlx_ptr);
		free(display->mlx_ptr);
	}
	free(display);
}
