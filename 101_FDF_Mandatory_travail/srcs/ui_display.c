#include "fdf.h"

/**
 * @brief Displays all control information and current state on the window.
 *
 * This function serves as the main UI renderer, showing the current application
 * state and available keyboard controls. It organizes the information into
 * clear sections and manages the vertical positioning of text elements.
 *
 * @param display The display structure containing all visualization data.
 * @see Called after rendering the map to provide user guidance.
 */
void	display_control_info(t_display *display)
{
	int	vertical_pos;

	vertical_pos = 20;
	display_state_info(display, &vertical_pos);
	display_controls_header(display, &vertical_pos);
	display_control_keys(display, &vertical_pos);
}

/**
 * @brief Displays the current state information on the window.
 *
 * This function renders text showing the current projection mode and color mode
 * in the user interface. It handles dynamic string creation and memory management
 * for the displayed text.
 *
 * @param display The display structure containing all visualization data.
 * @param vertical_pos Pointer to the current vertical position for text rendering.
 * @see Called by display_control_info to show the current application state.
 */
void	display_state_info(t_display *display, int *vertical_pos)
{
	char	*temp_str;

	mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos, WHITE,
				  "--- CURRENT STATE ---");
	*vertical_pos += 20;
	temp_str = ft_strjoin("Projection: ", display->is_isometric ? "Isometric" : "Parallel");
	if (!temp_str)
		print_error_and_exit("Failed to allocate string in display_info", 0);
	mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos,
				  WHITE, temp_str);
	free(temp_str);
	*vertical_pos += 20;
	temp_str = ft_strjoin("Color mode: ",
						get_color_mode_str(display->use_altitude_colors));
	if (!temp_str)
		print_error_and_exit("Failed to allocate string in display_info", 0);
	mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos,
				  WHITE, temp_str);
	free(temp_str);
	*vertical_pos += 20;
	display_flat_factor_info(display, vertical_pos);
}

/**
 * @brief Displays the flat factor information on the window.
 *
 * This function renders text showing the current flat factor value in the UI.
 * It converts the numeric value to a string and handles memory management for
 * the displayed text.
 *
 * @param display The display structure containing all visualization data.
 * @param vertical_pos Pointer to the current vertical position for text rendering.
 * @see Called by display_state_info to show the current flat factor.
 */
void	display_flat_factor_info(t_display *display, int *vertical_pos)
{
	char	*temp_str;
	char	*temp_itoa;

	temp_itoa = ft_itoa((int)(display->flat_f * 100));
	if (!temp_itoa)
		print_error_and_exit("Failed to allocate itoa string", 0);
	temp_str = ft_strjoin("Flat factor: ", temp_itoa);
	if (!temp_str)
	{
		free(temp_itoa);
		print_error_and_exit("Failed to allocate string in display_info", 0);
	}
	mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos,
				  WHITE, temp_str);
	free(temp_str);
	free(temp_itoa);
	*vertical_pos += 20;
}

/**
 * @brief Displays the controls header section in the UI.
 *
 * This function renders the header text for the controls section of the
 * user interface, providing a clear separation between state information
 * and available controls.
 *
 * @param display The display structure containing all visualization data.
 * @param vertical_pos Pointer to the current vertical position for text rendering.
 * @see Called by display_control_info to organize the UI sections.
 */
void	display_controls_header(t_display *display, int *vertical_pos)
{
	mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos, WHITE,
				  "--- CONTROLS ---");
	*vertical_pos += 20;
	mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos,
				  WHITE, RESET);
	*vertical_pos += 20;
	mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos,
				  WHITE, PERSP);
	*vertical_pos += 20;
}

/**
 * @brief Displays information about available keyboard controls.
 *
 * This function renders text describing all available keyboard shortcuts
 * and their functions in the user interface. It provides guidance to users
 * on how to interact with the application.
 *
 * @param display The display structure containing all visualization data.
 * @param vertical_pos Pointer to the current vertical position for text rendering.
 * @see Called by display_control_info to show available controls.
 */
void	display_control_keys(t_display *display, int *vertical_pos)
{
	mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos, WHITE,
				  "Toggle Point/Wire: Press 'P'");
	*vertical_pos += 20;
	mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos, WHITE,
				  "Toggle Color On/Off: Press 'C'");
	*vertical_pos += 20;
	mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos, WHITE,
				  "Toggle Color Mode: Press 'V'");
	*vertical_pos += 20;
	mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos,
				  WHITE, FLAT);
	*vertical_pos += 20;
	mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos, WHITE,
				  "Toggle Normal/Flat: Press 'F'");
	*vertical_pos += 20;
	mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos,
				  WHITE, CLOSE);
}
