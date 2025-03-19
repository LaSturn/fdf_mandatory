#include "fdf.h"

/**
 * @brief Returns a string description of the current color mode.
 *
 * @param mode The color mode value (0-3).
 * @return A string description of the color mode.
 */
char	*get_color_mode_str(int mode)
{
	if (mode == 0)
		return ("Default");
	if (mode == 1)
		return ("Altitude");
	if (mode == 2)
		return ("Rainbow Pulse");
	if (mode == 3)
		return ("Blue Gradient");
	return ("Unknown");
}

/**
 * @brief Toggles altitude-based coloring on and off.
 *
 * @param display The display structure to update.
 */
void	handle_color_toggle(t_display *display)
{
	display->use_altitude_colors = !display->use_altitude_colors;
	display->needs_redraw = 1;
}

/**
 * @brief Cycles through available color modes.
 *
 * @param display The display structure to update.
 */
void	handle_color_cycle(t_display *display)
{
	display->use_altitude_colors = (display->use_altitude_colors + 1) % 4;
	display->needs_redraw = 1;
	render_transformed_map(display);
}
