#include "fdf.h"

/**
 * @brief Sets up default display configuration for rendering.
 *
 * @param display The display structure to configure.
 */
void	configure_default_display(t_display *display)
{
	int	bounds[4];
	int	margins[4];
	int	available_width;
	int	available_height;

	set_initial_values(display);
	calculate_projected_bounds(display, bounds, 1);
	display->zoom = calculate_optimal_zoom(display, 0.1);
	calculate_projected_bounds(display, bounds, 1);
	compute_map_dimensions(display, bounds, margins);
	adjust_zoom_if_needed(display, bounds, margins);
	available_width = WIN_WIDTH - 2 * margins[2];
	available_height = WIN_HEIGHT - 2 * margins[3];
	display->shift_x = margins[2] + (available_width - margins[0]) / 2
		- bounds[0];
	display->shift_y = margins[3] + (available_height - margins[1]) / 2
		- bounds[1];
	ft_printf("Initial display settings: zoom=%.2f, shift_x=%d, shift_y=%d\n",
		display->zoom, display->shift_x, display->shift_y);
}

/**
 * @brief Sets initial map transformation values.
 *
 * @param display The display structure to configure.
 */
void	set_initial_values(t_display *display)
{
	calculate_map_center(display);
	display->is_isometric = 1;
	display->flat_f = 1;
	display->rot_x = 0;
	display->rot_y = 0;
	display->rot_z = 0;
	display->shift_x = 0;
	display->shift_y = 0;
	display->use_altitude_colors = 1;
	display->needs_redraw = 1;
	display->point_size = 1;
}

/**
 * @brief Computes map dimensions and margins.
 *
 * @param display The display structure.
 * @param bounds The bounds array [min_x, min_y, max_x, max_y].
 * @param margins The margins array [width, height, min_x, min_y].
 */
void	compute_map_dimensions(t_display *display,
		int bounds[4], int margins[4])
{
	(void)display;
	margins[0] = bounds[2] - bounds[0];
	margins[1] = bounds[3] - bounds[1];
	margins[2] = WIN_WIDTH * 0.1;
	margins[3] = WIN_HEIGHT * 0.1;
}

/**
 * @brief Adjusts zoom to make the map fill the available area
 * 			with proper margins.
 *
 * @param display The display structure to adjust.
 * @param bounds The bounds array [min_x, min_y, max_x, max_y].
 * @param margins The margins array [map_width, map_height,
 * 				min_margin_x, min_margin_y].
 */
void	adjust_zoom_if_needed(t_display *display,
		int bounds[4], int margins[4])
{
	float	zoom_factor_x;
	float	zoom_factor_y;
	float	new_zoom;
	int		available_width;
	int		available_height;

	available_width = WIN_WIDTH - 2 * margins[2];
	available_height = WIN_HEIGHT - 2 * margins[3];
	zoom_factor_x = (float)available_width / margins[0];
	zoom_factor_y = (float)available_height / margins[1];
	if (zoom_factor_x < zoom_factor_y)
		new_zoom = zoom_factor_x;
	else
		new_zoom = zoom_factor_y;
	if ((new_zoom - 1.0 > 0.05) || (1.0 - new_zoom > 0.05))
	{
		display->zoom *= new_zoom;
		calculate_projected_bounds(display, bounds, 1);
		margins[0] = bounds[2] - bounds[0];
		margins[1] = bounds[3] - bounds[1];
		ft_printf("Zoom adjusted to %.2f to fill window with 10%% margin\n",
			display->zoom);
	}
}
