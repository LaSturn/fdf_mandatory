#include "fdf.h"

/**
 * @brief Handles keyboard input events for the application.
 *
 * This function processes key press events and performs the appropriate actions
 * based on which key was pressed.
 *
 * @param keycode The code of the key that was pressed.
 * @param display The display structure containing all visualization data.
 * @return Always returns 0 to indicate successful handling.
 */
int	handel_key_in(int keycode, t_display *display)
{
	if (keycode == KC_ESC)
	{
		win_exit_and_cleanup(display);
		return (0);
	}
	if (keycode == KC_R)
		configure_default_display(display);
	else if (keycode == KC_TAB)
		handle_projection_toggle(display);
	else if (keycode == KC_7 || keycode == KC_8)
	{
		activ_ff(keycode, display);
		display->needs_redraw = 1;
	}
	else if (keycode == KC_P)
		handle_point_mode_toggle(display);
	else if (keycode == KC_C)
		handle_color_toggle(display);
	else if (keycode == KC_V)
		handle_color_cycle(display);
	else if (keycode == KC_F)
		handle_flat_toggle(display);
	render_transformed_map(display);
	return (0);
}

/**
 * @brief Toggles between isometric and parallel projection modes.
 *
 * @param display The display structure to update.
 */
void	handle_projection_toggle(t_display *display)
{
	display->is_isometric = !display->is_isometric;
	display->needs_redraw = 1;
}

/**
 * @brief Toggles between points-only and wireframe rendering modes.
 *
 * @param display The display structure to update.
 */
void	handle_point_mode_toggle(t_display *display)
{
	display->points_only = !display->points_only;
	display->needs_redraw = 1;
}

/**
 * @brief Toggles between normal and flat elevation modes.
 *
 * @param display The display structure to update.
 */
void	handle_flat_toggle(t_display *display)
{
	if (display->flat_f == 1.0)
		display->flat_f = 0.0;
	else
		display->flat_f = 1.0;
	ft_printf("Flattening factor set to: %.2f\n", display->flat_f);
	display->needs_redraw = 1;
}

/**
 * @brief Adjusts the flat factor value based on key input.
 *
 * This function modifies the flat factor setting in the display structure
 * when the user presses the '7' or '8' keys. It increases the flat factor when
 * '8' is pressed and decreases it when '7' is pressed, maintaining the value
 * within reasonable bounds.
 *
 * @param keycode The code of the key that was pressed (KC_7 or KC_8).
 * @param display The display structure to update.
 * @see Called by handel_key_in when '7' or '8' keys are detected.
 */
void	activ_ff(int keycode, t_display *display)
{
	if (keycode == KC_8)
	{
		if (display->flat_f < 10)
			display->flat_f += 5 * FF;
	}
	if (keycode == KC_7)
	{
		if (display->flat_f > 0)
			display->flat_f -= 5 * FF;
	}
}
