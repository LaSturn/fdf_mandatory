#include "fdf.h"

/**
 * @brief Draws a line between two points using Bresenham's algorithm.
 *
 * This function implements the Bresenham line drawing algorithm to create a
 * visually continuous line between two points with color interpolation.
 *
 * @param display The display structure for rendering data.
 * @param start The starting point of the line.
 * @param end The ending point of the line.
 */
void bresenham(t_display *display, t_point start, t_point end)
{
	t_point current;
	t_point sign;
	t_point delta;
	int line;
	int tmp;

	initialize_bresenham(&start, &end, &delta, &sign);
	line = delta.x - delta.y;
	current = start;
	while (current.x != end.x || current.y != end.y) {
		current.color = interpolate_color(current, start, end, delta);
		put_pixel_to_img(display, current.x, current.y, current.color);
		tmp = line * 2;
		if (tmp > -delta.y) {
			line -= delta.y;
			current.x += sign.x;
		}
		if (tmp < delta.x) {
			line += delta.x;
			current.y += sign.y;
		}
	}
}

/**
 * @brief Initializes parameters for the Bresenham line algorithm.
 *
 * This function calculates the delta values and direction signs needed
 * for the Bresenham algorithm.
 *
 * @param start The starting point of the line.
 * @param end The ending point of the line.
 * @param delta Pointer to store delta values between points.
 * @param sign Pointer to store direction signs.
 */
void	initialize_bresenham(t_point *start, t_point *end,	t_point *delta, t_point *sign)
{
	delta->x = get_abs_value(end->x - start->x);
	delta->y = get_abs_value(end->y - start->y);
	sign->x = -1;
	sign->y = -1;
	if (start->x < end->x)
		sign->x = 1;
	if (start->y < end->y)
		sign->y = 1;
}

/**
 * @brief Places a pixel in the image buffer.
 *
 * This function sets the color of a specific pixel in the image buffer,
 * with bounds checking to prevent buffer overflows.
 *
 * @param display The display structure containing the image buffer.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color value to set.
 */
void put_pixel_to_img(t_display *display, int x, int y, int color)
{
	int position;
	char *addr;

	if (!display || !display->img || !display->img->img_addr)
		return ;
	if (x < 0 || y < 0 || x >= WIN_WIDTH || y >= WIN_HEIGHT)
		return ;
	position = (y * display->img->size_line) + (x * (display->img->bpp / 8));
	if (position < 0 || position >= WIN_HEIGHT * WIN_WIDTH * (display->img->bpp / 8))
		return ;
	addr = display->img->img_addr + position;
	*(int *)addr = color;
}

/**
 * @brief Draws a point with configurable size.
 *
 * This function renders a square point of specified size at the given
 * coordinates, used in the points-only rendering mode.
 *
 * @param display The display structure containing rendering data.
 * @param x The x-coordinate of the point center.
 * @param y The y-coordinate of the point center.
 * @param color The color value to use for the point.
 */
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

