#include "fdf.h"

/**
 * @brief Prints an error message and exits the program.
 *
 * @param type The error message to display.
 * @param use_errno If non-zero, appends the system error message.
 */
void	print_error_and_exit(const char *type, int use_errno)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd((char *)type, 2);
	if (use_errno)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
	}
	ft_putstr_fd("\n", 2);
	exit(EXIT_FAILURE);
}

/**
 * @brief Frees all allocated memory and exits the program.
 *
 * @param display The display structure to clean up.
 * @return Always returns 0 (not reached due to exit).
 */
int	shutdown_fdf(t_display *display)
{
	free_all_struct_display(display);
	exit(EXIT_SUCCESS);
	return (0);
}

/**
 * @brief Prints an error message without exiting the program.
 *
 * @param type The error message to display.
 * @param use_errno If non-zero, appends the system error message.
 */
void	print_error_without_exit(const char *type, int use_errno)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd((char *)type, 2);
	if (use_errno)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
	}
	ft_putstr_fd("\n", 2);
}

/**
 * @brief Cleans up and exits the program when the window is closed.
 *
 * @param display The display structure to clean up.
 * @return Always returns 0 (not reached due to exit).
 */
int	win_exit_and_cleanup(t_display *display)
{
	free_all_struct_display(display);
	ft_printf("Sturny's FdF was closed successfully\n");
	exit(EXIT_SUCCESS);
	return (0);
}
