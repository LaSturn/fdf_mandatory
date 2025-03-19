#include "fdf.h"


/**
 * @brief Returns the absolute value of an integer.
 *
 * This function calculates the absolute value of the input parameter.
 *
 * @param value The integer value to convert to absolute.
 * @return The absolute value of the input.
 */
int	get_abs_value(int value)
{
	if (value < 0)
		return (-value);
	return (value);
}

/**
 * @brief Determines the minimum and maximum z-values in the map.
 *
 * This function scans the entire map to find the highest and lowest
 * altitude points for color gradient calculations.
 *
 * @param map The map structure to analyze.
 */
void	get_z_min_max(t_map *map)
{
	int	x;
	int	y;

	map->min_z = INT_MAX;
	map->max_z = INT_MIN;
	y = 0;
	while (y < map->map_height)
	{
		x = 0;
		while (x < map->map_width)
		{
			if (map->z_ij[y][x] < map->min_z)
				map->min_z = map->z_ij[y][x];
			if (map->z_ij[y][x] > map->max_z)
				map->max_z = map->z_ij[y][x];
			x++;
		}
		y++;
	}
}

/**
 * @brief Extracts color information from a string.
 *
 * This function parses a string to find hexadecimal color notation.
 * If found, it sets the is_color flag and returns the color value.
 *
 * @param str The string to parse.
 * @param map The map structure to update the is_color flag.
 * @return The parsed color or WHITE if no color specified.
 */
int	get_color(char *str, t_map *map)
{
	while (*str && (ft_isdigit(*str) || *str == '-' || *str == '+'
		|| *str == ','))
		str++;
	if (*str && (*str == 'x' || *str == 'X'))
	{
		map->is_color = 1;
		ft_striter_tolower(str + 1);
		return (ft_atoi_base(str + 1, 16));
	}
	return (WHITE);
}

/**
 * @brief Converts all characters in a string to lowercase.
 *
 * This function modifies a string in-place, converting all uppercase
 * letters to lowercase.
 *
 * @param str The string to convert to lowercase.
 */
void	ft_striter_tolower(char *str)
{
	if (!str)
		return ;
	while (*str)
	{
		*str = ft_tolower(*str);
		str++;
	}
}

/**
 * Returns the maximum of two integer values
 *
 * @param v1 First integer value
 * @param v2 Second integer value
 * @return The larger of the two values
 */
int get_max_int(int v1, int v2)
{

	if(v1 > v2)
		return (v1);
	return (v2);
}
