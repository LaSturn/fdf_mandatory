#include "fdf.h"


void render_transformed_map(t_display *display)
{
    int x;
    int y;
    /*
    if (!display->needs_redraw)
        return;
    */
    ft_bzero(display->img->img_addr, WIN_HEIGHT * WIN_WIDTH * (display->img->bpp / 8));
    if (display->zoom) {
        update_transformed_points(display);
        y = -1;
        while (++y < display->map_height_store) {
            x = -1;
            while (++x < display->map_width_store)
            {
                if (display->points_only)
                    plot_point(display, display->transformed[y][x].x, display->transformed[y][x].y, display->transformed[y][x].color);
                else {
                    if (x < display->map_width_store - 1)
                        bresenham(display, display->transformed[y][x], display->transformed[y][x + 1]);
                    if (y < display->map_height_store - 1)
                        bresenham(display, display->transformed[y][x], display->transformed[y + 1][x]);
                }
            }
        }
    }
    // Debug prints...
    mlx_put_image_to_window(display->mlx_ptr, display->window_ptr, display->img->img_ptr, 0, 0);
    // More debug prints...
    display_control_info(display);
    //display->needs_redraw = 0;
    //ft_printf("je suis dans render_transformed_map: a la fin\n");
}

void bresenham(t_display *display, t_point start, t_point end)
{
    t_point current;
    t_point sign;
    t_point delta;
    int line;
    int tmp;
    int steps;
    int i;

    if (!display)
        return;
        
    initialize_bresenham(&start, &end, &delta, &sign);
    line = delta.x - delta.y;
    current = start;
    
    // Calculate the maximum number of steps to avoid infinite loops
    steps = (delta.x > delta.y) ? delta.x : delta.y;
    if (steps == 0)  // Handle the case when start == end
    {
        put_pixel_to_img(display, start.x, start.y, start.color);
        return;
    }
    
    for (i = 0; i <= steps && i < WIN_WIDTH * 2; i++)  // Add safety limit
    {
        put_pixel_to_img(display, current.x, current.y, current.color);
        
        if (current.x == end.x && current.y == end.y)
            break;
            
        tmp = line * 2;
        if (tmp > -delta.y)
        {
            line -= delta.y;
            current.x += sign.x;
        }
        if (tmp < delta.x)
        {
            line += delta.x;
            current.y += sign.y;
        }
    }
}

void initialize_bresenham(t_point *start, t_point *end, t_point *delta, t_point *sign)
{
    if (!start || !end || !delta || !sign)
        return;
        
    delta->x = get_abs_value(end->x - start->x);
    delta->y = get_abs_value(end->y - start->y);
    sign->x = (start->x < end->x) ? 1 : -1;
    sign->y = (start->y < end->y) ? 1 : -1;
}

void put_pixel_to_img(t_display *display, int x, int y, int color)
{
    int position;
    char *addr;

    if (!display || !display->img || !display->img->img_addr)
        return;
        
    if (x < 0 || y < 0 || x >= WIN_WIDTH || y >= WIN_HEIGHT)
        return;
        
    position = (y * display->img->size_line) + (x * (display->img->bpp / 8));
    
    // Additional boundary check
    if (position < 0 || position >= WIN_HEIGHT * WIN_WIDTH * (display->img->bpp / 8))
        return;
        
    addr = display->img->img_addr + position;
    
    // Use a safer way to set the color
    *(int *)addr = color;
}

void	update_transformed_points(t_display *display) // Precomputes points.
{
	int y;
	int x;

	y = -1;
	//ft_printf("Updating transformed points: height %d, width %d\n", display->map->map_height, display->map->map_width); //debugg
	while (++y < display->map->map_height)
	{
		x = -1;
		if (!display->transformed[y]) 
		{
            ft_printf("Error: transformed[%d] is NULL\n", y);
			print_error_and_exit("Invalid transformed row", 1);
        }
		while (++x < display->map->map_width)
		{
			display->transformed[y][x] = apply_transformations(display, create_map_point(x, y, display));
		}
	}
	//ft_printf("Transformed points updated\n");
}

void plot_point(t_display *display, int x, int y, int color)
{
	int	size;
	int dy;
	int dx;

	size = display->point_size;
	dy = -size;	
	while (dy <= size)
	{
		dx = -size;
		while (dx <= size)
		{
			if (x + dx >= 0 && x + dx < WIN_WIDTH && y + dy >= 0 && y +dy < WIN_HEIGHT)
				put_pixel_to_img(display, x + dx, y + dy, color);
			dx++;
		}
		dy++;
	}	
}