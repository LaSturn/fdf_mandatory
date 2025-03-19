#include "fdf.h"

void print_error_and_exit(const char *type, int use_errno)
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

int shutdown_fdf(t_display *display)
{
    free_all_struct_display(display);
    exit(EXIT_SUCCESS);
    return (0);
}

void print_error_without_exit(const char *type, int use_errno)
{
    ft_putstr_fd("Error: ", 2);// Writes "Error: " to stderr
    ft_putstr_fd((char *)type, 2);// Writes custom message to stderr
    if (use_errno) {// If use_errno is non-zero (e.g., 1)
        ft_putstr_fd(": ", 2);// Adds a separator
        ft_putstr_fd(strerror(errno), 2);// Writes system error message
    }
    ft_putstr_fd("\n", 2);// Adds a newline
}

int win_exit_and_cleanup(t_display *dispay)
{
    free_all_struct_display(dispay);
    ft_printf("Sturny's FdF was closed succefully\n");
    exit (EXIT_SUCCESS);
    return (0);
}