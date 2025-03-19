#include "fdf.h"

/**
 * @brief Assigns colors to points based on their altitude.
 *
 * This function determines which color to assign to a point based on its z-value
 * relative to the map's altitude range.
 *
 * @param display The display structure containing map data.
 * @param curr_z_val The z-coordinate (altitude) at the current point.
 * @return An RGB color value as an integer based on altitude range.
 */
int	get_color_by_altitude(t_display *display, int curr_z_val)
{
	double	percentage;

	percentage = get_percentage_in_range(display->map->min_z,
			display->map->max_z, curr_z_val);
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
 * @brief Creates a blue gradient color based on altitude/z-value.
 *
 * This function generates a color in the blue gradient spectrum based on the
 * z-coordinate's position within the map's altitude range.
 *
 * @param display The display structure containing map data.
 * @param z The z-coordinate (altitude) at the current point.
 * @return The calculated RGB color value as an integer.
 */
int	get_blue_gradient_color(t_display *display, int z)
{
	double	percentage;
	int		r;
	int		g;
	int		b;

	percentage = get_percentage_in_range(display->map->min_z,
			display->map->max_z, z);
	r = interpolate_light(0x00, 0x66, percentage);
	g = interpolate_light(0x00, 0xCC, percentage);
	b = interpolate_light(0x66, 0xFF, percentage);
	return ((r << RS) | (g << GS) | b);
}

/**
 * @brief Calculates the relative position of a value within
 * a range as a percentage.
 *
 * @param start The minimum value of the range.
 * @param end The maximum value of the range.
 * @param curr_val The current value to find the position of.
 * @return A double between 0.0 and 1.0 representing the position in the range.
 */
double	get_percentage_in_range(int start, int end, int curr_val)
{
	double	range;
	double	position;
	double	percentage;

	range = end - start;
	position = curr_val - start;
	if (!range)
		return (1.0);
	percentage = position / range;
	return (percentage);
}

/**
 * @brief Interpolates between two light (color component) values.
 *
 * @param start The starting color component value.
 * @param end The ending color component value.
 * @param percentage A value between 0.0 and 1.0 determining the
 * 		interpolation position.
 * @return The interpolated color component value.
 */
int	interpolate_light(int start, int end, double percentage)
{
	return ((int)((1 - percentage) * start + percentage * end));
}

/**
 * @brief Creates a color gradient between two points in a line.
 *
 * @param current The current point being drawn.
 * @param start The starting point of the line.
 * @param end The ending point of the line.
 * @param delta The delta values (distance) between start and end points.
 * @return The interpolated RGB color value as an integer.
 */
int	interpolate_color(t_point current, t_point start, t_point end, t_point del)
{
	int		r;
	int		g;
	int		b;
	double	percentage;

	if (current.color == end.color)
		return (current.color);
	if (del.x > del.y)
		percentage = get_percentage_in_range(start.x, end.x, current.x);
	else
		percentage = get_percentage_in_range(start.y, end.y, current.y);
	r = interpolate_light((start.color >> RS) & BM,
			(end.color >> RS) & BM, percentage);
	g = interpolate_light((start.color >> GS) & BM,
			(end.color >> GS) & BM, percentage);
	b = interpolate_light(start.color & BM, end.color & BM, percentage);
	return ((r << RS) | (g << GS) | b);
}
