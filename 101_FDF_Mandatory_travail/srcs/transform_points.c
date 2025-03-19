#include "fdf.h"

t_point apply_transformations(t_display *display, t_point point)
{
    int x = point.x * display->zoom;
    int y = point.y * display->zoom;
    int z = point.z * (display->zoom / SF) * display->flat_f;
    apply_rot_x(display, &y, &z);
    apply_rot_y(display, &x, &z);
    apply_rot_z(display, &x, &y);
    if (display->is_isometric)
        apply_isometric(display, &x, &y, z);
    else
        apply_parallel(display, &x, &y, z);
    x += display->shift_x;
    y += display->shift_y;
    point.x = x;
    point.y = y;
    // Keep original z for color, don’t overwrite
    // point.z = z; // Remove this line
    //ft_printf("Transform: orig_z=%d, new_x=%d, new_y=%d\n", point.z, x, y);
    return point;
}

t_point create_map_point(int x, int y, t_display *display)
{
    t_point point;
    point.x = x;
    point.y = y;
    point.z = display->map->z_ij[y][x];
    if (display->use_altitude_colors == 2)
        point.color = get_blue_gradient_color(display, point.z); // Replace with blue gradient
    else if (display->use_altitude_colors == 1)
        point.color = get_color_by_altitude(display, point.z);
    else if (display->map->is_color)
        point.color = display->map->color[y][x];
    else
        point.color = get_color_by_altitude(display, point.z);
    return point;
}

void apply_isometric(t_display *display, int *x, int *y, int z) 
{
    (void)display;
	int prev_x = *x;
    int prev_y = *y;
    *x = (prev_x - prev_y) * cos(0.52359877559); // 30°
    *y = -z + (prev_x + prev_y) * sin(0.52359877559);
}

/**
 * Calculate optimal zoom factor to fit the entire map in the viewport
 * 
 * @param display The display structure with map and projection settings
 * @param margin Margin percentage (0.0-1.0) to leave around the map
 * @return The optimal zoom factor
 */
float calculate_optimal_zoom(t_display *display, float margin)
{
    int bounds[4]; // [min_x, min_y, max_x, max_y]
    float original_zoom;
    float proj_width, proj_height;
    float zoom_factor_x, zoom_factor_y;
    float available_width, available_height;
    float optimal_zoom, min_zoom = 0.1, max_zoom = 100.0;
    
    // Save original zoom to restore later
    original_zoom = display->zoom;
    
    // First calculate the projected bounds with zoom = 1.0
    display->zoom = 1.0;
    calculate_projected_bounds(display, bounds, 1);
    
    // Calculate the projected dimensions at zoom = 1.0
    proj_width = bounds[2] - bounds[0];
    proj_height = bounds[3] - bounds[1];
    
    // Add additional precaution for very small dimensions
    if (proj_width < 1.0) proj_width = 1.0;
    if (proj_height < 1.0) proj_height = 1.0;
    
    // Calculate available space (with margin)
    available_width = WIN_WIDTH * (1.0 - margin);
    available_height = WIN_HEIGHT * (1.0 - margin);
    
    // Calculate zoom factors for width and height
    zoom_factor_x = (proj_width > 0) ? available_width / proj_width : max_zoom;
    zoom_factor_y = (proj_height > 0) ? available_height / proj_height : max_zoom;
    
    // Use the smaller zoom factor to ensure the entire map fits
    optimal_zoom = (zoom_factor_x < zoom_factor_y) ? zoom_factor_x : zoom_factor_y;
    
    // Apply constraints to prevent extreme zoom values
    if (optimal_zoom < min_zoom)
        optimal_zoom = min_zoom;
    if (optimal_zoom > max_zoom)
        optimal_zoom = max_zoom;
    
    // Restore the original zoom
    display->zoom = original_zoom;
    
    // Add more detailed logging about the center points
    int center_x = (bounds[0] + bounds[2]) / 2;
    int center_y = (bounds[1] + bounds[3]) / 2;
    ft_printf("Map projected center at zoom=1.0: (%d, %d)\n", center_x, center_y);
    
    ft_printf("Optimal zoom calculation: width=%.1f height=%.1f -> zoom=%.2f\n", 
             proj_width, proj_height, optimal_zoom);
    
    return optimal_zoom;
}

/* ci-dessous transform_rotation.c*/

// ok verifie
// rotation in place
// rotation around x-axis

void	apply_rot_x(t_display *display, int *y, int *z)
{
	float	prev_y;

	prev_y = *y;
	*y = (prev_y * cos(display->rot_x)) + (*z * sin(display->rot_x));
	*z = (-prev_y * sin(display->rot_x)) + (*z * cos(display->rot_x));
}
// ok verifie
// rotation in place
// rotation around y-axis
void	apply_rot_y(t_display *display, int *x, int *z)
{
	float	prev_x;

	prev_x = *x;
	*x = (*x * cos(display->rot_y)) + (*z * sin(display->rot_y));
	*z = (-prev_x * sin(display->rot_y)) + (*z * cos(display->rot_y));
}
// ok verifie
// rotation in place
// rotation around z-axis
void	apply_rot_z(t_display *display, int *x, int *y)
{
	float	prev_x;

	prev_x = *x;
	*x = (*x * cos(display->rot_z)) + (*y * sin(display->rot_z));
	*y = (prev_x * sin(display->rot_z)) + (*y * cos(display->rot_z));
}

int get_max_int(int v1, int v2)
{

	if(v1 > v2)
		return (v1);
	return (v2);
}

void apply_parallel(t_display *display, int *x, int *y, int z) 
{
    (void)display;
	*x = *x; // No change
    *y = *y - z; // Shift y by z
}