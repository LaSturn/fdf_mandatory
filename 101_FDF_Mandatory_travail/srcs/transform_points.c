#include "fdf.h"

/**
 * Applies all transformation steps to a map point
 *
 * @param display The display structure with transformation parameters
 * @param point The original map point to transform
 * @return The fully transformed point with updated coordinates
 */
t_point apply_transformations(t_display *display, t_point point)
{
	int x = point.x * display->zoom;
	int y = point.y * display->zoom;
	int z = point.z * (display->zoom / SF) * display->flat_f;
	apply_rot_x(display, &y, &z);
	apply_rot_y(display, &x, &z);
	apply_rot_z(display, &x, &y);
	if (display->is_isometric)
		apply_isometric(display, &x, &y, z);
	else
		apply_parallel(display, &x, &y, z);
	x += display->shift_x;
	y += display->shift_y;
	point.x = x;
	point.y = y;
	return (point);
}

/**
 * Creates a point from map coordinates with appropriate color
 *
 * @param x The x-coordinate in the map grid
 * @param y The y-coordinate in the map grid
 * @param display The display structure with map and color data
 * @return The created point with position and color information
 */
t_point create_map_point(int x, int y, t_display *display)
{
	t_point point;
	point.x = x;
	point.y = y;
	point.z = display->map->z_ij[y][x];

	if (display->use_altitude_colors == 1)
		point.color = get_color_by_altitude(display, point.z);
	else if (display->use_altitude_colors == 2)
		point.color = get_blue_gradient_color(display, point.z);
	else if (display->map->is_color)
		point.color = display->map->color[y][x];
	else
		point.color = WHITE;

	return (point);
}

/**
 * @brief Calculates the center point of the map.
 *
 * @param display The display structure containing map data.
 */
void	calculate_map_center(t_display *display)
{
	display->map_center_x = (float)(display->map_width_store - 1) / 2.0f;
	display->map_center_y = (float)(display->map_height_store - 1) / 2.0f;
	display->map_center_z = (float)(display->map->min_z
		+ display->map->max_z) / 2.0f;
}

/**
 * Calculate optimal zoom factor to fit the entire map in the viewport
 *
 * @param display The display structure with map and projection settings
 * @param margin Margin percentage (0.0-1.0) to leave around the map
 * @return The optimal zoom factor
 */
float	calculate_optimal_zoom(t_display *display, float margin)
{
	int		bounds[4];
	t_point	proj_dims;
	t_point	factors;
	float	optimal_zoom;
	float	min_zoom = 0.1;
	float	max_zoom = 100.0;

	proj_dims = prepare_zoom_calculation(display, bounds);
	factors = calculate_zoom_factors(proj_dims, margin, max_zoom);

	// Use the smaller zoom factor to ensure the entire map fits
	optimal_zoom = (factors.x < factors.y) ? factors.x : factors.y;

	// Apply constraints to prevent extreme zoom values
	if (optimal_zoom < min_zoom)
		optimal_zoom = min_zoom;
	if (optimal_zoom > max_zoom)
		optimal_zoom = max_zoom;

	return (optimal_zoom);
}

