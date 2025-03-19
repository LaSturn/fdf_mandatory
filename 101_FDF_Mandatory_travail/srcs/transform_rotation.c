#include "fdf.h"

/**
 * Applies rotation around the x-axis
 *
 * @param display The display structure with rotation angles
 * @param y Pointer to y-coordinate to transform
 * @param z Pointer to z-coordinate to transform
 */
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

/**
 * Applies rotation around the y-axis
 *
 * @param display The display structure with rotation angles
 * @param x Pointer to x-coordinate to transform
 * @param z Pointer to z-coordinate to transform
 */
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

/**
 * Applies rotation around the z-axis
 *
 * @param display The display structure with rotation angles
 * @param x Pointer to x-coordinate to transform
 * @param y Pointer to y-coordinate to transform
 */
void	apply_rot_z(t_display *display, int *x, int *y)
{
	float	prev_x;

	prev_x = *x;
	*x = (*x * cos(display->rot_z)) + (*y * sin(display->rot_z));
	*y = (prev_x * sin(display->rot_z)) + (*y * cos(display->rot_z));
}
