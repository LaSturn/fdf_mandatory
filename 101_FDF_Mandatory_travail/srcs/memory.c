#include "fdf.h"

void free_map_memory(t_map *map)
{
    int i;

    if (!map)
        return;
    if (map->z_ij)
    {
        i = -1;
        while (++i < map->map_height)
            if (map->z_ij[i])
                free(map->z_ij[i]);
        free(map->z_ij);
    }
    if (map->color)
    {
        i = -1;
        while (++i < map->map_height)
            if (map->color[i])
                free(map->color[i]);
        free(map->color);
    }
    free(map);
}

void free_matrix(char **matrix)
{
    int i = 0;

    if (!matrix)
        return;
    while (matrix[i])
        free(matrix[i++]);
    free(matrix);
}

void free_all_struct_display(t_display *display)
{
    int y;
    int map_height = 0;

    if (!display)
        return;
        
    // First, save any information we need before freeing memory
    if (display->map)
        map_height = display->map->map_height;
    
    // Free transformed array using the saved map_height
    if (display->transformed)
    {
        y = 0;
        while (y < map_height)
        {
            if (display->transformed[y])
                free(display->transformed[y]);
            y++;
        }
        free(display->transformed);
        display->transformed = NULL;
    }
    
    // Free MLX resources
    if (display->mlx_ptr && display->window_ptr)
        mlx_destroy_window(display->mlx_ptr, display->window_ptr);
    
    if (display->mlx_ptr && display->img && display->img->img_ptr)
        mlx_destroy_image(display->mlx_ptr, display->img->img_ptr);
    
    // Free img struct
    if (display->img)
    {
        free(display->img);
        display->img = NULL;
    }
    
    // Free map
    if (display->map)
    {
        free_map_memory(display->map);
        display->map = NULL;
    }
    
    // Free MLX pointer last
    if (display->mlx_ptr)
    {
        mlx_destroy_display(display->mlx_ptr);
        free(display->mlx_ptr);
        display->mlx_ptr = NULL;
    }
    
    // Finally, free the display struct itself
    free(display);
}