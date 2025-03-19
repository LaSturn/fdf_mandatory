#include "fdf.h"

/**
 * Applies isometric projection transformation to coordinates
 *
 * @param display The display structure (unused but kept for consistency)
 * @param x Pointer to x-coordinate to transform
 * @param y Pointer to y-coordinate to transform
 * @param z The z-coordinate affecting the projection
 */
void apply_isometric(t_display *display, int *x, int *y, int z)
{
	(void)display;
	int prev_x = *x;
	int prev_y = *y;
	*x = (prev_x - prev_y) * cos(0.52359877559);
	*y = -z + (prev_x + prev_y) * sin(0.52359877559);
}

/**
 * Applies parallel projection transformation to coordinates
 *
 * @param display The display structure (unused but kept for consistency)
 * @param x Pointer to x-coordinate (unchanged in parallel projection)
 * @param y Pointer to y-coordinate to transform
 * @param z The z-coordinate affecting the projection
 */
void apply_parallel(t_display *display, int *x, int *y, int z)
{
	(void)display;
	*x = *x;
	*y = *y - z;
}

/**
 * @brief Prepares projection dimensions for zoom calculation.
 *
 * @param display The display structure.
 * @param bounds Output array for projection bounds.
 * @return Tuple of width and height as projected dimensions.
 */
t_point	prepare_zoom_calculation(t_display *display, int bounds[4])
{
	t_point	dimensions;
	float	original_zoom;

	original_zoom = display->zoom;
	display->zoom = 1.0;
	calculate_projected_bounds(display, bounds, 1);
	dimensions.x = bounds[2] - bounds[0];
	dimensions.y = bounds[3] - bounds[1];
	if (dimensions.x < 1.0)
		dimensions.x = 1.0;
	if (dimensions.y < 1.0)
		dimensions.y = 1.0;

	display->zoom = original_zoom;
	return (dimensions);
}

/**
 * @brief Calculates optimal zoom factors based on available space.
 *
 * @param proj_dims Projected dimensions of the map.
 * @param margin Margin percentage to leave around the map.
 * @param max_zoom Maximum allowed zoom value.
 * @return Tuple containing x and y zoom factors.
 */
t_point	calculate_zoom_factors(t_point proj_dims, float margin, float max_zoom)
{
	t_point	factors;
	float	available_width;
	float	available_height;

	available_width = WIN_WIDTH * (1.0 - margin);
	available_height = WIN_HEIGHT * (1.0 - margin);

	factors.x = (proj_dims.x > 0) ? available_width / proj_dims.x : max_zoom;
	factors.y = (proj_dims.y > 0) ? available_height / proj_dims.y : max_zoom;

	return (factors);
}
