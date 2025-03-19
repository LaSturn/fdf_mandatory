#include "fdf.h"

/**
 * @brief Creates a blue gradient color based on altitude/z-value.
 *
 * This function generates a color in the blue gradient spectrum based on the
 * z-coordinate's position within the map's altitude range.
 * It first determines where the z-value falls proportionally between the minimum
 * and maximum z values in the map, then creates a gradient from dark blue (0x000066)
 * to light blue (0x66CCFF) correspondingly.
 *
 * @param display The display structure containing map data.
 * @param z The z-coordinate (altitude) at the current point.
 * @return The calculated RGB color value as an integer.
 * @see Called by render_transformed_map when display->use_altitude_colors is set to 3.
 */
int get_blue_gradient_color(t_display *display, int z)
{
    double percentage;
    int r, g, b;

    // Calculate percentage of z within min_z and max_z range
    percentage = get_percentage_in_range(display->map->min_z, display->map->max_z, z);

    // Blue gradient: Transition from dark blue (0x000066) to light blue (0x66CCFF)
    r = interpolate_light(0x00, 0x66, percentage); // Red: 0 to 102
    g = interpolate_light(0x00, 0xCC, percentage); // Green: 0 to 204
    b = interpolate_light(0x66, 0xFF, percentage); // Blue: 102 to 255

    return (r << RS) | (g << GS) | b;
}

/**
 * @brief Assigns colors to points based on their altitude.
 *
 * This function determines which color to assign to a point based on its z-value
 * relative to the map's altitude range. It divides the altitude range into sections
 * and assigns a different color to each section, creating a topographic-like
 * visualization of the terrain.
 *
 * @param display The display structure containing map data.
 * @param curr_z_val The z-coordinate (altitude) at the current point.
 * @return An RGB color value as an integer based on altitude range.
 * @see Called by render_transformed_map when display->use_altitude_colors is set to 1.
 */
int get_color_by_altitude(t_display *display, int curr_z_val)
{
	double	percentage;
	percentage = get_percentage_in_range(display->map->min_z, display->map->max_z, curr_z_val);
	if (percentage < 0.1)
		return (D_PURPLE);
	else if (percentage < 0.3)
		return (S_BROWN);
	else if (percentage < 0.5)
		return (OLIVE);
	else if (percentage < 0.7)
		return (B_YELLOW);
	else
		return (PURPLE);
}

/**
 * @brief Calculates the relative position of a value within a range as a percentage.
 *
 * This function determines where a value falls within a given range and returns
 * the result as a percentage between 0.0 and 1.0. It's used for color interpolation
 * and positioning calculations throughout the program.
 *
 * @param start The minimum value of the range.
 * @param end The maximum value of the range.
 * @param curr_val The current value to find the position of.
 * @return A double between 0.0 and 1.0 representing the position in the range.
 * @see Called by get_blue_gradient_color, get_color_by_altitude, and interpolate_color.
 */
double get_percentage_in_range(int start, int end, int curr_val)
{
    double range = end - start;
    double position = curr_val - start;
    if (!range)
        return (1.0);
    double percentage = position / range;
    //ft_printf("Percentage: start=%d, end=%d, curr=%d, range=%.2f, pos=%.2f, pct=%.2f\n", start, end, curr_val, range, position, percentage);
    return (percentage);
}

/**
 * @brief Interpolates between two light (color component) values.
 *
 * This function calculates an intermediate value between two color component values
 * based on a percentage. It performs linear interpolation to create smooth
 * transitions between colors.
 *
 * @param start The starting color component value.
 * @param end The ending color component value.
 * @param percentage A value between 0.0 and 1.0 determining the interpolation position.
 * @return The interpolated color component value.
 * @see Called by get_blue_gradient_color and interpolate_color.
 */
int interpolate_light(int start, int end, double percentage)
{
	return ((int)((1 - percentage) * start + percentage * end));
}

/**
 * @brief Creates a color gradient between two points in a line.
 *
 * This function generates intermediate colors for a line drawn between two points.
 * It determines which axis (x or y) has the greater delta and interpolates along that
 * axis to create a smooth color transition. Used by the Bresenham line algorithm.
 *
 * @param current The current point being drawn.
 * @param start The starting point of the line.
 * @param end The ending point of the line.
 * @param delta The delta values (distance) between start and end points.
 * @return The interpolated RGB color value as an integer.
 * @see Called by the bresenham line drawing function.
 */
int interpolate_color(t_point current, t_point start, t_point end, t_point delta)
{
	int r;
	int g;
	int b;
	double percentage;
	
	if (current.color == end.color)
		return (current.color);
	if (delta.x > delta.y)
		percentage = get_percentage_in_range(start.x, end.x, current.x);
	else 
		percentage = get_percentage_in_range(start.y, end.y, current.y);
	r = interpolate_light((start.color >> RS) & BM, (end.color >> RS) & BM, percentage);
	g = interpolate_light((start.color >> GS) & BM, (end.color >> GS) & BM, percentage);
	b = interpolate_light(start.color & BM, end.color & BM, percentage);
	return ((r << RS) | (g << GS) | b);
}

/**
 * @brief Returns a string description of the current color mode.
 *
 * This function converts the integer color mode value into a human-readable string
 * description for display in the UI. Each mode represents a different coloring
 * algorithm for the wireframe.
 *
 * @param mode The color mode value (0-3).
 * @return A string description of the color mode.
 * @see Called by display_state_info to show the current color mode in the UI.
 */
char *get_color_mode_str(int mode)
{
    if (mode == 0)
        return ("Default");
    if (mode == 1)
        return ("Altitude");
    if (mode == 2)
        return ("Rainbow Pulse");
    if (mode == 3)
        return ("Blue Gradient"); // Updated from "Rainbow Shift"
    return ("Unknown");
}