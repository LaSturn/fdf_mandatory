#include "fdf.h"

/**
 * @brief Initializes MLX and launches the visualization.
 *
 * @param map The map data structure to visualize.
 * @return Always returns 0 upon completion.
 */
int	start_mlx(t_map *map)
{
	t_display	*display;

	display = ft_calloc(1, sizeof(t_display));
	if (!display)
		print_error_and_exit("Display allocation failed", 0);
	if (!map)
	{
		free(display);
		print_error_and_exit("NULL map passed to start_mlx", 0);
	}
	initialize_display(display, map);
	initialize_mlx(display);
	configure_default_display(display);
	mlx_hook(display->window_ptr, KPE, (1L << 0), handel_key_in, display);
	mlx_hook(display->window_ptr, CE, (1L << 17), shutdown_fdf, display);
	render_transformed_map(display);
	mlx_loop(display->mlx_ptr);
	free_all_struct_display(display);
	return (0);
}

/**
 * @brief Initializes the display structure with map data.
 *
 * @param display The display structure to initialize.
 * @param map The map data to associate with the display.
 */
void	initialize_display(t_display *display, t_map *map)
{
	if (display == NULL)
		print_error_and_exit("display is NULL", 1);
	if (!map || map->map_height <= 0 || map->map_width <= 0)
	{
		free(display);
		print_error_and_exit("Invalid map dimensions", 1);
	}
	display->map = map;
	display->map_width_store = map->map_width;
	display->map_height_store = map->map_height;
	display->img = ft_calloc(1, sizeof(t_imagedata));
	if (!display->img)
	{
		free_all_struct_display(display);
		print_error_and_exit("img is NULL", 1);
	}
	allocate_transformed_arrays(display, map);
}

/**
 * @brief Initializes the MLX graphics context.
 *
 * @param display The display structure to initialize.
 * @return 1 on success, 0 on failure.
 */
int	initialize_mlx(t_display *display)
{
	display->mlx_ptr = mlx_init();
	if (display->mlx_ptr == NULL)
	{
		print_error_without_exit("mlx_init() a échoué", 0);
		return (0);
	}
	display->img->img_ptr = mlx_new_image(display->mlx_ptr,
			WIN_WIDTH, WIN_HEIGHT);
	if (display->img->img_ptr == NULL)
	{
		print_error_without_exit("mlx_new_image() a échoué", 0);
		mlx_destroy_display(display->mlx_ptr);
		free(display->mlx_ptr);
		return (0);
	}
	return (init_mlx_data(display));
}

/**
 * @brief Initializes MLX image data and window.
 *
 * @param display The display structure to initialize.
 * @return 1 on success, 0 on failure.
 */
int	init_mlx_data(t_display *display)
{
	display->img->img_addr = mlx_get_data_addr(display->img->img_ptr,
			&display->img->bpp, &display->img->size_line,
			&display->img->endianness);
	if (display->img->img_addr == NULL)
	{
		print_error_without_exit("mlx_get_data_addr() a échoué", 0);
		mlx_destroy_image(display->mlx_ptr, display->img->img_ptr);
		mlx_destroy_display(display->mlx_ptr);
		free(display->mlx_ptr);
		return (0);
	}
	display->window_ptr = mlx_new_window(display->mlx_ptr,
			WIN_WIDTH, WIN_HEIGHT, "Sturny's FdF");
	if (display->window_ptr == NULL)
	{
		print_error_without_exit("mlx_new_window() a échoué", 0);
		mlx_destroy_image(display->mlx_ptr, display->img->img_ptr);
		mlx_destroy_display(display->mlx_ptr);
		free(display->mlx_ptr);
		return (0);
	}
	return (1);
}

/**
 * @brief Allocates transformed point arrays for the display.
 *
 * @param display The display structure to initialize.
 * @param map The map containing dimension information.
 */
void	allocate_transformed_arrays(t_display *display, t_map *map)
{
	int	y;

	display->transformed = ft_calloc(map->map_height, sizeof(t_point *));
	if (!display->transformed)
	{
		free_all_struct_display(display);
		print_error_and_exit("Failed to allocate transformed array", 1);
	}
	y = -1;
	while (++y < map->map_height)
	{
		display->transformed[y] = ft_calloc(map->map_width, sizeof(t_point));
		if (!display->transformed[y])
		{
			ft_printf("Failed to allocate row %d of transformed\n", y);
			free_all_struct_display(display);
			print_error_and_exit("Failed to allocate transformed row", 1);
		}
	}
}
