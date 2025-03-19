#include "fdf.h"

int start_mlx(t_map *map)
{
    t_display *display;

    display = ft_calloc(1, sizeof(t_display));
    if (!display)
        print_error_and_exit("Display allocation failed", 0);
    if (!map)
    {
        free(display);
        print_error_and_exit("NULL map passed to start_mlx", 0);
    }
    initialize_display(display, map);
    initialize_mlx(display);
    configure_default_display(display);
    mlx_hook(display->window_ptr, KPE, (1L << 0), handel_key_in, display);
    mlx_hook(display->window_ptr, CE, (1L << 17), shutdown_fdf, display); // ne comprend pas c est quoi CE
    render_transformed_map(display);
    mlx_loop(display->mlx_ptr);
    free_all_struct_display(display);
    return (0);
}
/*
// nouvelle devrait marcher
void initialize_display(t_display *display, t_map *map)
{
    int y;
    
    display->map = map;
    display->img = ft_calloc(1, sizeof(t_imagedata));
    if (!display->img)
    {
        free_all_struct_display(display);
        print_error_and_exit("Image allocation failed", 1);
    }
    display->transformed = ft_calloc(map->map_height, sizeof(t_point *));
    if (!display->transformed)
    {
        free_all_struct_display(display);
        print_error_and_exit("Failed to allocate transformed array", 1);
    }
    y = -1;
    while (++y < map->map_height)
    {
        display->transformed[y] = ft_calloc(map->map_width, sizeof(t_point));
        if (!display->transformed[y])
        {
            ft_printf("Failed to allocate row %d of transformed\n", y); // debugg
            free_all_struct_display(display);
            print_error_and_exit("Failed to allocate transformed row", 1);
        }
    }
}
    */

// version bonus
void initialize_display(t_display *display, t_map *map)
{
    int y;
    if (display == NULL)
        print_error_and_exit("disolay is NULL", 1);
    if (!map || map->map_height <= 0 || map->map_width <= 0)
    {
        free(display);
		print_error_and_exit("Invalid map dimensions", 1);
    }
    display->map = map;
    display->map_width_store = map->map_width;  // Store width
    display->map_height_store = map->map_height; // Store height
    //ft_printf("Map height: %d, width: %d\n", map->map_height, map->map_width);
    display->img = ft_calloc(1, sizeof(t_imagedata));
    if (!display->img)
    {
        free_all_struct_display(display);
        print_error_and_exit("img is NULL", 1);
    }
    display->transformed = ft_calloc(map->map_height, sizeof(t_point *));
    if (!display->transformed)
    {
        free_all_struct_display(display);
		print_error_and_exit("Failed to allocate transformed array", 1);
    }
    y = -1;
    while (++y < map->map_height)
    {
        display->transformed[y] = ft_calloc(map->map_width, sizeof(t_point));
        if (!display->transformed[y])
        {
            ft_printf("Failed to allocate row %d of transformed\n", y);
            free_all_struct_display(display);
			print_error_and_exit("Failed to allocate transformed row", 1);
        }
    }
    //ft_printf("Transformed array allocated: %d rows, %d cols\n", map->map_height, map->map_width);
}

int initialize_mlx(t_display *display)
{
    display->mlx_ptr = mlx_init();
    if (display->mlx_ptr == NULL)
    {
		print_error_without_exit("mlx_init() a échoué", 0);
		return (0);
	}
    display->img->img_ptr = mlx_new_image(display->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
    if (!display->img->img_ptr)
    if (display->img->img_ptr == NULL)                                                
	{
		print_error_without_exit("mlx_new_image() a échoué", 0);
		mlx_destroy_display(display->mlx_ptr);
		free(display->mlx_ptr);
		return (0);
	}
    display->img->img_addr = mlx_get_data_addr(display->img->img_ptr, &display->img->bpp, &display->img->size_line, &display->img->endianness);
    if (display->img->img_addr == NULL)
	{
		print_error_without_exit("mlx_get_data_addr() a échoué", 0);
		mlx_destroy_image(display->mlx_ptr, display->img->img_ptr); 
		mlx_destroy_display(display->mlx_ptr); 
		free(display->mlx_ptr); 
		return (0);
	}
    display->window_ptr = mlx_new_window(display->mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "Sturny's FdF");
    if (display->window_ptr == NULL)
	{
		print_error_without_exit("mlx_new_window() a échoué", 0);
		mlx_destroy_image(display->mlx_ptr, display->img->img_ptr); 
		mlx_destroy_display(display->mlx_ptr); 
		free(display->mlx_ptr); 
		return (0);
	}
    return (1);
}

void configure_default_display(t_display *display)
{
    // for first render (centering incl. z)
    int bounds[4]; // [min_x, min_y, max_x, max_y]
    int map_width, map_height;
    int min_margin_x, min_margin_y;
    
    
    // Calculate the center of the map (for transformations)
    calculate_map_center(display);
    
    // Set initial values
    display->is_isometric = 1;
    display->flat_f = 1;
    display->rot_x = 0;
    display->rot_y = 0;
    display->rot_z = 0;
    
    
    // Set initial zoom to 1.0 for proper calculations
    //display->zoom = 1.0;
    display->shift_x = 0;
    display->shift_y = 0;
    
    // Calculate initial projected bounds at zoom=1.0
    calculate_projected_bounds(display, bounds, 1);
    
    // Calculate optimal zoom with 10% margin
    display->zoom = calculate_optimal_zoom(display, 0.1);
    
    // Calculate bounds again with the new optimal zoom
    calculate_projected_bounds(display, bounds, 1);
    
    // Calculate map dimensions
    map_width = bounds[2] - bounds[0];
    map_height = bounds[3] - bounds[1];
    
    // Calculate minimum margins (10% of screen dimensions)
    min_margin_x = WIN_WIDTH * 0.1;
    min_margin_y = WIN_HEIGHT * 0.1;
    
    // Calculate available display area after margins
    int available_width = WIN_WIDTH - 2 * min_margin_x;
    int available_height = WIN_HEIGHT - 2 * min_margin_y;
    
    // Check if map fits within available area with current zoom
    if (map_width > available_width || map_height > available_height)
    {
        // Recalculate zoom to fit within available area
        float zoom_factor_x = (float)available_width / map_width;
        float zoom_factor_y = (float)available_height / map_height;
        float new_zoom = (zoom_factor_x < zoom_factor_y) ? zoom_factor_x : zoom_factor_y;
        
        // Apply the reduced zoom
        display->zoom *= new_zoom;
        
        // Recalculate bounds with adjusted zoom
        calculate_projected_bounds(display, bounds, 1);
        
        // Update map dimensions
        map_width = bounds[2] - bounds[0];
        map_height = bounds[3] - bounds[1];
        
        ft_printf("Zoom adjusted to fit within margins: %.2f\n", display->zoom);
    }
    
    // Center the map in the available area between margins
    display->shift_x = min_margin_x + (available_width - map_width) / 2 - bounds[0];
    display->shift_y = min_margin_y + (available_height - map_height) / 2 - bounds[1];
    
    ft_printf("Map positioning: Adding 10%% margin (x=%d, y=%d) on all sides\n",
              min_margin_x, min_margin_y);
    ft_printf("Adjusted shift values: shift_x=%d, shift_y=%d\n",
              display->shift_x, display->shift_y);
    
    // Set other default values
    display->use_altitude_colors = 0;
    
    ft_printf("Initial display settings: zoom=%.2f, shift_x=%d, shift_y=%d\n", 
              display->zoom, display->shift_x, display->shift_y);
}

void calculate_map_center(t_display *display)
{
    display->map_center_x = (float)(display->map_width_store - 1) / 2.0f;
    display->map_center_y = (float)(display->map_height_store - 1) / 2.0f;
    
    // Use middle of min_z and max_z for z-center
    display->map_center_z = (float)(display->map->min_z + display->map->max_z) / 2.0f;
    
    ft_printf("Map center calculated at (%.2f, %.2f, %.2f)\n", 
              display->map_center_x, display->map_center_y, display->map_center_z);
}


/**
 * Calculate the bounds of the map after projection
 * This helps determine the optimal position and zoom for initial display
 * 
 * @param display The display structure with transformation parameters
 * @param bounds Array to store the results [min_x, min_y, max_x, max_y]
 * @param use_current_zoom If 1, uses current zoom; if 0, uses zoom=1 for calculation
 */
void calculate_projected_bounds(t_display *display, int bounds[4], int use_current_zoom)
{
    int x, y;
    t_point point, transformed;
    float original_zoom = display->zoom;
    float original_shift_x = display->shift_x;
    float original_shift_y = display->shift_y;
    
    // Initialize bounds to extreme values
    bounds[0] = INT_MAX;  // min_x
    bounds[1] = INT_MAX;  // min_y
    bounds[2] = INT_MIN;  // max_x
    bounds[3] = INT_MIN;  // max_y
    
    // Temporarily set shift to 0 for pure projection calculations
    display->shift_x = 0;
    display->shift_y = 0;
    
    // If requested, temporarily set zoom to 1 to get "natural" size
    if (!use_current_zoom)
        display->zoom = 1.0;
    
    // Process every point in the map
    y = 0;
    while (y < display->map_height_store)
    {
        x = 0;
        while (x < display->map_width_store)
        {
            // Create point with current coordinates
            point = create_map_point(x, y, display);
            
            // Apply transformation but don't render
            transformed = apply_transformations(display, point);
            
            // Update bounds if needed
            if (transformed.x < bounds[0])
                bounds[0] = transformed.x;
            if (transformed.y < bounds[1])
                bounds[1] = transformed.y;
            if (transformed.x > bounds[2])
                bounds[2] = transformed.x;
            if (transformed.y > bounds[3])
                bounds[3] = transformed.y;
            
            x++;
        }
        y++;
    }
    
    // Restore original parameters
    display->zoom = original_zoom;
    display->shift_x = original_shift_x;
    display->shift_y = original_shift_y;
    
    ft_printf("Projected bounds: X[%d,%d] width=%d, Y[%d,%d] height=%d\n", 
             bounds[0], bounds[2], bounds[2]-bounds[0],
             bounds[1], bounds[3], bounds[3]-bounds[1]);
}