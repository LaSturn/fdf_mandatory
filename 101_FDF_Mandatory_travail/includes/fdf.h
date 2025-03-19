#ifndef FDF_H
# define FDF_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <math.h>
# include "mlx.h"
# include "libft.h"

# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080
# define WHITE 0xFFFFFF

typedef struct s_point
{
    int x;
    int y;
    int z;
    int color;
} t_point;

typedef struct s_map
{
    int map_width;
    int map_height;
    int **z_ij;
    int **color;
    int is_color;
} t_map;

typedef struct s_imagedata
{
    void *img_ptr;
    char *img_addr;
    int bpp;
    int size_line;
    int endianness;
} t_imagedata;

typedef struct s_display
{
    void *mlx_ptr;
    void *window_ptr;
    t_imagedata *img;
    t_map *map;
    t_point **transformed;
} t_display;

/* Function prototypes */
void print_error_and_exit(const char *type, int use_errno);
int shutdown_fdf(t_display *display);
void check_file_readable(const char *filename);
t_map *parse_map_dimensions(char *file_name);
int nbr_in_a_line(t_map *map, char *line);
void get_map(char *file_name, t_map *map);
void alloc_memory_for_map(t_map *map);
void stock_map_values(char *file_name, t_map *map);
void extract_map_data_z_color(int fd, t_map *map);
int get_color(char *str, t_map *map);
void free_map_memory(t_map *map);
void free_matrix(char **matrix);
void free_all_struct_display(t_display *display);
int start_mlx(t_map *map);
void initialize_display(t_display *display, t_map *map);
int initialize_mlx(t_display *display);
void render_transformed_map(t_display *display);
void bresenham(t_display *display, t_point start, t_point end);
void initialize_bresenham(t_point *start, t_point *end, t_point *delta, t_point *sign);
void put_pixel_to_img(t_display *display, int x, int y, int color);
t_point apply_transformations(t_point point);
t_point create_map_point(int x, int y, t_map *map);
void apply_isometric(int *x, int *y, int z);
void ft_striter_tolower(char *str);
int get_abs_value(int value);

/* Debug functions */
void display_map_data(t_map *map);

#endif