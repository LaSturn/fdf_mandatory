#include "fdf.h"

void ft_striter_tolower(char *str)
{
    if (!str)
        return;
    while (*str)
    {
        *str = ft_tolower(*str);
        str++;
    }
}

int	get_abs_value(int value)
{
	if (value < 0)
		return (-(value));
	return (value);
}

void get_z_min_max(t_map *map)
{
    int x, y;
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
    //ft_printf("Min z: %d, Max z: %d\n", map->min_z, map->max_z); // Debug a enlever
}