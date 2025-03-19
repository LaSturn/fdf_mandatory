#ifndef FDF_H
# define FDF_H

# include "../libft/libft.h"
# include "../printf/ft_printf.h"
# include "../mlx/mlx.h"
# include <fcntl.h>
# include <stdlib.h>
# include <math.h>
# include <errno.h>
# include <string.h> // For strerror()

/* Window Dimensions */
# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080

/* Key and Event Codes */
# define KC_ESC 65307  // Escape key
# define KPE 2         // KeyPress event
# define CE 17         // Close event (DestroyNotify)

/* ------------  INT MAX DEFINITION  ---------------------------------------- */
# define INT_MAX 2147483647

/* ------------  INT min DEFINITION  ---------------------------------------- */
# define INT_MIN -2147483648


/* Color Definitions */

/** @def B_YELLOW
 *  @brief This is a macro that holds the hexadecimal value for the color bright 
 * yellow.
 */
# define B_YELLOW	0xFFFF00

/** @def D_PURPLE
 *  @brief This is a macro that holds the hexadecimal value for the color dark 
 * purple.
 */
# define D_PURPLE	0x301934

/** @def OLIVE
 *  @brief This is a macro that holds the hexadecimal value for the color 
 * olive.
 */
# define OLIVE		0x808000

/** @def S_BROWN
 *  @brief This is a macro that holds the hexadecimal value for the color 
 * saddle brown.
 */
# define S_BROWN	0x8B4513

/** @def PURPLE
 *  @brief This is a macro that holds the hexadecimal value for the color 
 * purple.
 */
# define PURPLE		0x800080

/** @def WHITE
 *  @brief This is a macro that holds the hexadecimal value for the color 
 * white.
 */
# define WHITE		0xFFFFFF

# define BLUE 0x66CCFF // Pure blue

/** 
 * @brief The number of bits to shift to the right to isolate the red 
 * component of a color.
 */
# define RS			16

/** 
 * @brief The number of bits to shift to the right to isolate the green 
 * component of a color.
 */
# define GS			8

/** 
 * @brief The bitmask to apply to isolate the blue component of a color.
 */
# define BM			0xFF // vslrut s la base apres j ai change
//#define BM 8  // ✅ Au lieu de 0xFF (255), qui est trop grand // pas sur que ok!!!!!!


/**
 * @brief The number of bits to shift to isolate the alpha component 
 * in a 32-bit color value.
 * 
 * This macro represents the number of bits to shift a 32-bit color 
 * value to the right to isolate the alpha component. 
 * The color value is assumed to be in the format 0xRRGGBBAA, where RR 
 * represents the red component, GG the green component, 
 * BB the blue component, and AA the alpha component.
 */
# define AS			24

/* Key and Event Codes */
/* The key codes are used in X11 windowing system. So this is valid in Ubuntu
 * and other Linux distributions. 
*/

# define KC_ESC 65307  // Escape key

/** 
 * @brief Close application command description.
 */
# define CLOSE	"Close: Press 'ESC' or Click on 'X'"

# define KPE 2         // KeyPress event

/**
 * @brief The event code for a window close event (also known as
 *  the destroy notify event) in the MiniLibX library.
 */
# define CE 17         // Close event (DestroyNotify)
/** 
 * @brief Key code for the Escape key. 
*/
# define KC_ESC	65307

/*
 * @brief Key code for 'R' key.
 */
# define KC_R		114

/*
 * @brief Key code for 'TAB'. 
 */

 # define KC_TAB		65289
/**
 * @brief The event code for a key press event in the MiniLibX library.
 */

 # define KPE			2
// P  pour point only

# define KC_P		112 // ok

// C use_altitude_color pour mettre en mode mettre set de couleur (pas couleur de base de la carte)

# define KC_C		99 // a tester
// rainbow mode

# define KC_V 118 // 'V' key

/*
 * @brief Key code for 'F'. 
 */
# define KC_F        102 // lowercase f key for resetting flattening factor

/** 
 * @brief Flattening factor command description.
 */
# define FLAT	"Flattening Factor: Use '7' / '8'"

/* Mouvements configuration */
# define SF		10

/* Structures */
typedef struct s_point
{
    int x;      // x-coordinate
    int y;      // y-coordinate
    int z;      // z-coordinate (altitude)
    int color;  // color of the point
} t_point;

typedef struct s_map
{
    int map_width;  // Number of columns
    int map_height; // Number of rows
    int min_z; // min_z is the minimum z-coordinate (or elevation) of the map.
	int max_z; // max_z is the maximum z-coordinate (or elevation) of the map.
    int **z_ij;     // 2D array of z-coordinates
    int **color;    // 2D array of colors
    int is_color;   // Flag indicating if the map has color data
} t_map;

typedef struct s_imagedata
{
    void *img_ptr;   // Image pointer
    char *img_addr;  // Image data address
    int bpp;         // Bits per pixel
    int size_line;   // Size of a line in bytes
    int endianness;  // Endianness of the image
} t_imagedata;

typedef struct s_display
{
    void *mlx_ptr;      // MLX pointer
    void *window_ptr;   // Window pointer
    t_map *map;         // Map data
    t_imagedata *img;   // Image data
    t_point **transformed; // Precomputed transformed points
    int shift_x; // shift_x is the horizontal shift of the map.
	int shift_y; // shift_y is the vertical shift of the map.
    float rot_x; // rot_x is the rotation angle around the x-axis.
	float rot_y; // rot_y is the rotation angle around the y-axis.
	float rot_z; // rot_z is the rotation angle around the z-axis.
	float zoom; // zoom is the zoom factor of the map.
	int is_isometric; // is_isometric is a flag to indicate if the map is in isometric projection.
	float flat_f; // "ff" for flat factor, is a factor to flatten the map.
    int map_width_store;  // Add this line
    int map_height_store; // Add this line
    // Map center coordinates (for centered zoom and rotation)
    float map_center_x;     // Center of the map in model coordinates
    float map_center_y;     
    float map_center_z;  
    int use_altitude_colors; // 0 = default, 1 = altitude, 2 = blue_gradient
    int points_only;      // Point cloud toggle
    int point_size;       // Adjustable point size
} t_display;

/* Function Prototypes */

/* main.c */
int	main(int ac, char **av);

/* map_parse.c */
t_map *parse_map_dimensions(char *file_name);
int nbr_in_a_line(t_map *map, char *line);
void check_file_readable(const char *filename);

/* map_data.c */
void get_map(char *file_name, t_map *map);
void alloc_memory_for_map(t_map *map);
void stock_map_values(char *file_name, t_map *map);
void extract_map_data_z_color(int fd, t_map *map);
int get_color(char *str, t_map *map);

/* display_init.c */
int start_mlx(t_map *map);
void initialize_display(t_display *display, t_map *map);
int initialize_mlx(t_display *display);
void configure_default_display(t_display *display); // ajouté
void calculate_map_center(t_display *display);

/* render_lines.c */
void bresenham(t_display *display, t_point start, t_point end);
void initialize_bresenham(t_point *start, t_point *end, t_point *delta, t_point *sign);
void render_transformed_map(t_display *display);
void put_pixel_to_img(t_display *display, int x, int y, int color); // Added prototype
void    update_transformed_points(t_display *display); // Precomputes points.
void plot_point(t_display *display, int x, int y, int color); // bonus


/* transform_points.c */
t_point apply_transformations(t_display *display, t_point point);   // bonus
t_point create_map_point(int x, int y, t_display *display); // bonus
void	apply_isometric(t_display *display, int *x, int *y, int z); // bonus 
void    calculate_projected_bounds(t_display *display, int bounds[4], int use_current_zoom); // bonus
float   calculate_optimal_zoom(t_display *display, float margin);   
void	apply_parallel(t_display *display, int *x, int *y, int z);  // bonus
/* memory.c */
void free_map_memory(t_map *map);
void free_matrix(char **matrix);
void free_all_struct_display(t_display *display);

/* errors_exit.c */
void print_error_and_exit(const char *type, int use_errno);
int shutdown_fdf(t_display *display);
void print_error_without_exit(const char *type, int use_errno);
int win_exit_and_cleanup(t_display *dispay);

/* map_utils.c */
void ft_striter_tolower(char *str);
int get_abs_value(int value);
void get_z_min_max(t_map *map);

/* map_utils.c */
void display_map_data(t_map *map);

/* color.c */
int get_blue_gradient_color(t_display *display, int z);
int get_color_by_altitude(t_display *display, int curr_z_val);
double get_percentage_in_range(int start, int end, int curr_val);
int interpolate_light(int start, int end, double percentage);
int interpolate_color(t_point current, t_point start, t_point end, t_point delta);
char *get_color_mode_str(int mode);

/* key.c */
int handel_key_in(int keycode, t_display *display);
void display_control_info(t_display *display);

/*---- transorm_rotation.c ----*/
void	apply_rot_x(t_display *display, int *y, int *z);
void	apply_rot_y(t_display *display, int *x, int *z);
void	apply_rot_z(t_display *display, int *x, int *y);
int get_max_int(int v1, int v2);

#endif