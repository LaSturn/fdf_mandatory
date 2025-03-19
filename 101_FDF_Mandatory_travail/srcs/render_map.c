#include "fdf.h"

/**
 * @brief Renders the transformed map to the window.
 *
 * This function clears the image buffer, transforms and renders all map points,
 * and displays the result on the window. It handles both point and wireframe
 * rendering modes.
 *
 * @param display The display structure containing rendering data.
 */
void render_transformed_map(t_display *display)
{
	int x;
	int y;

	ft_bzero(display->img->img_addr, WIN_HEIGHT * WIN_WIDTH * (display->img->bpp / 8));
	if (display->zoom) {
		ft_printf("Rendering map with color mode: %d\n", display->use_altitude_colors);
		update_transformed_points(display);
		y = -1;
		while (++y < display->map_height_store) {
			x = -1;
			while (++x < display->map_width_store)
			{
				if (display->points_only)
					plot_point(display, display->transformed[y][x].x, display->transformed[y][x].y, display->transformed[y][x].color);
				else {
					if (x < display->map_width_store - 1)
						bresenham(display, display->transformed[y][x], display->transformed[y][x + 1]);
					if (y < display->map_height_store - 1)
						bresenham(display, display->transformed[y][x], display->transformed[y + 1][x]);
				}
			}
		}
	}
	mlx_put_image_to_window(display->mlx_ptr, display->window_ptr, display->img->img_ptr, 0, 0);
	display_control_info(display);
}

/**
 * @brief Updates the array of transformed 3D points.
 *
 * This function applies all transformations to each map point and stores
 * the results in the transformed points array for rendering.
 *
 * @param display The display structure containing map and transformation data.
 */
void	update_transformed_points(t_display *display) // Precomputes points.
{
	int y;
	int x;

	y = -1;
	while (++y < display->map->map_height)
	{
		x = -1;
		if (!display->transformed[y])
		{
			ft_printf("Error: transformed[%d] is NULL\n", y);
			print_error_and_exit("Invalid transformed row", 1);
		}
		while (++x < display->map->map_width)
		{
			display->transformed[y][x] = apply_transformations(display, create_map_point(x, y, display));
		}
	}
}

/**
 * @brief Calculates the map's boundaries after projection.
 *
 * @param display The display structure with transformation parameters.
 * @param bounds Array to store results [min_x, min_y, max_x, max_y].
 * @param use_current_zoom Whether to use current zoom or zoom=1.
 */
void	calculate_projected_bounds(t_display *display, int bounds[4],
		int use_current_zoom)
{
	int		x;
	int		y;
	float	original_zoom;
	float	original_shift_x;
	float	original_shift_y;

	original_zoom = display->zoom;
	original_shift_x = display->shift_x;
	original_shift_y = display->shift_y;
	bounds[0] = INT_MAX;
	bounds[1] = INT_MAX;
	bounds[2] = INT_MIN;
	bounds[3] = INT_MIN;
	display->shift_x = 0;
	display->shift_y = 0;
	if (!use_current_zoom)
		display->zoom = 1.0;
	y = 0;
	while (y < display->map_height_store)
	{
		x = 0;
		while (x < display->map_width_store)
		{
			update_point_bounds(display, bounds, x, y);
			x++;
		}
		y++;
	}
	display->zoom = original_zoom;
	display->shift_x = original_shift_x;
	display->shift_y = original_shift_y;
}

/**
 * @brief Updates point and calculates bounds for given coordinate.
 *
 * @param display The display structure.
 * @param bounds The bounds array to update.
 * @param x The x-coordinate to process.
 * @param y The y-coordinate to process.
 */
void	update_point_bounds(t_display *display, int bounds[4], int x, int y)
{
	t_point	point;
	t_point	transformed;

	point = create_map_point(x, y, display);
	transformed = apply_transformations(display, point);
	if (transformed.x < bounds[0])
		bounds[0] = transformed.x;
	if (transformed.y < bounds[1])
		bounds[1] = transformed.y;
	if (transformed.x > bounds[2])
		bounds[2] = transformed.x;
	if (transformed.y > bounds[3])
		bounds[3] = transformed.y;
}
