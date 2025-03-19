#include "fdf.h"

/**
 * @brief Handles keyboard input events for the application.
 *
 * This function processes key press events and performs the appropriate actions
 * based on which key was pressed. It handles program termination, display mode
 * toggling, projection switching, and color mode cycling.
 *
 * @param keycode The code of the key that was pressed.
 * @param display The display structure containing all visualization data.
 * @return Always returns 0 to indicate successful handling.
 * @see Called by the MLX library when a key press event occurs.
 */
int handel_key_in(int keycode, t_display *display)
{
    if (keycode == KC_ESC) 
    {
        win_exit_and_cleanup(display);
        return (0);
    }
    if (keycode == KC_R) // Réinitialiser les variables d'affichage si la touche R est enfoncée
        configure_default_display(display);
    else if (keycode == KC_TAB) // Basculer entre la vue isométrique et la vue parallèle avec Tab
    {
        //ft_printf("Toggling projection: isometric %d -> %d\n", display->is_isometric, !display->is_isometric);
        display->is_isometric =! display->is_isometric; // je ne comprends pas exactement ce que c est
        display->needs_redraw = 1;
    }
    else if (keycode == KC_P) 
    {
        //ft_printf("Toggling points_only: %d -> %d\n", display->points_only, !display->points_only);
        display->points_only = !display->points_only;
        display->needs_redraw = 1;
    }
    else if (keycode == KC_C)
    {
        display->use_altitude_colors = !display->use_altitude_colors;
        //ft_printf("Toggling color mode: use_altitude_colors = %d\n", display->use_altitude_colors);
        display->needs_redraw = 1;
    }
    else if (keycode == KC_V)
    {
        display->use_altitude_colors = (display->use_altitude_colors + 1) % 4;
        //ft_printf("Color mode: %d (0=default, 1=altitude, 2=rainbow pulse, 3=rainbow shift)\n", display->use_altitude_colors);
        display->needs_redraw = 1;
        render_transformed_map(display); // Immediate redraw
    }
    //ft_printf("Dans hook, handel_key_in: je suis avant render_transformed_map\n");
    render_transformed_map(display);
    //ft_printf("Dans hook, handel_key_in: j'ai passe render_transformed_map\n");
    return (0); // pas besoin je pense
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
void display_state_info(t_display *display, int *vertical_pos)
{
    char *temp_str;
    
    mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos, WHITE, 
                  "--- CURRENT STATE ---");
    *vertical_pos += 20;

    temp_str = ft_strjoin("Projection: ", display->is_isometric ? "Isometric" : "Parallel");
    if (!temp_str)
        print_error_and_exit("Failed to allocate string in display_control_info", 0);
    mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos, WHITE, temp_str);
    free(temp_str);
    *vertical_pos += 20;

    temp_str = ft_strjoin("Color mode: ", get_color_mode_str(display->use_altitude_colors));
    if (!temp_str)
        print_error_and_exit("Failed to allocate string in display_control_info", 0);
    mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos, WHITE, temp_str);
    free(temp_str);
    *vertical_pos += 20;
}

/**
 * @brief Displays the control instructions header in the UI.
 *
 * This function renders the header text for the controls section of the
 * user interface, providing a clear separation between state information
 * and available controls.
 *
 * @param display The display structure containing all visualization data.
 * @param vertical_pos Pointer to the current vertical position for text rendering.
 * @see Called by display_control_info to organize the UI sections.
 */
void display_controls_header(t_display *display, int *vertical_pos)
{
    mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos, WHITE, 
                  "--- CONTROLS ---");
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
void display_control_keys(t_display *display, int *vertical_pos)
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

    mlx_string_put(display->mlx_ptr, display->window_ptr, 10, *vertical_pos, WHITE, CLOSE);
}

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
void display_control_info(t_display *display)
{
    int vertical_pos = 20; // Starting position for text
    
    display_state_info(display, &vertical_pos);
    display_controls_header(display, &vertical_pos);
    display_control_keys(display, &vertical_pos);
}
