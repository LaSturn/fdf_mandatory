#include "fdf.h"

/*
** Displays the content of the z_ij and color arrays
** This helps to debug by visualizing the input data
*/
void display_map_data(t_map *map)
{
    int x, y;
    
    if (!map || !map->z_ij || !map->color)
    {
        ft_printf("Error: Cannot display map data - Map not properly initialized\n");
        return;
    }

    ft_printf("\n=== Z-COORDINATES MATRIX (%d x %d) ===\n", map->map_width, map->map_height);
    
    y = 0;
    while (y < map->map_height)
    {
        x = 0;
        while (x < map->map_width)
        {
            ft_printf("%2d ", map->z_ij[y][x]); // Changed from %3d to %2d for better formatting
            x++;
        }
        ft_printf("\n");
        y++;
    }
    
    ft_printf("\n=== COLOR MATRIX (%d x %d) ===\n", map->map_width, map->map_height);
    ft_printf("(Colors in hexadecimal, FFFFFF = white/default)\n");
    
    y = 0;
    while (y < map->map_height)
    {
        x = 0;
        while (x < map->map_width)
        {
            if (map->color[y][x] == WHITE)
                ft_printf("FFFFFF ");
            else
                ft_printf("%06X ", map->color[y][x]);
            x++;
        }
        ft_printf("\n");
        y++;
    }
    
    ft_printf("\n=== MAP PROPERTIES ===\n");
    ft_printf("Width: %d\n", map->map_width);
    ft_printf("Height: %d\n", map->map_height);
    ft_printf("Has custom colors: %s\n", map->is_color ? "Yes" : "No");
    ft_printf("============================\n\n");
}
