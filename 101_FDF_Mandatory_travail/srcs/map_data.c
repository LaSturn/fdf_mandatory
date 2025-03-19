#include "fdf.h"

void get_map(char *file_name, t_map *map)
{
    alloc_memory_for_map(map);
    stock_map_values(file_name, map);
}

void alloc_memory_for_map(t_map *map)
{
    int i;

    map->z_ij = ft_calloc(map->map_height, sizeof(int *));
    map->color = ft_calloc(map->map_height, sizeof(int *));
    if (!map->z_ij || !map->color)
    {
        free_map_memory(map);
        print_error_and_exit("Memory allocation failed", 1);
    }
    i = -1;
    while (++i < map->map_height)
    {
        map->z_ij[i] = ft_calloc(map->map_width, sizeof(int));
        map->color[i] = ft_calloc(map->map_width, sizeof(int));
        if (!map->z_ij[i] || !map->color[i])
        {
            free_map_memory(map);
            print_error_and_exit("Memory allocation failed", 1);
        }
    }
}

void stock_map_values(char *file_name, t_map *map)
{
    int fd = open(file_name, O_RDONLY);
    if (fd < 0)
    {
        free_map_memory(map);
        print_error_and_exit("Error opening file", 1);
    }
    extract_map_data_z_color(fd, map);
    close(fd);
}

void extract_map_data_z_color(int fd, t_map *map)
{
    char *line;
    char **split;
    int i = -1;
    int j;

    while (++i < map->map_height)
    {
        line = get_next_line(fd);
        if (!line)
        {
            free_map_memory(map);
            print_error_and_exit("Unexpected end of file", 1);
        }
        
        // Debug the raw line
        ft_printf("Raw line %d: '%s'\n", i, line);
        
        split = ft_split(line, ' ');
        if (!split)
        {
            free(line);
            free_map_memory(map);
            print_error_and_exit("Memory allocation failed while splitting line", 1);
        }
        
        j = -1;
        while (++j < map->map_width && split[j])
        {
            // Debug the token before conversion
            ft_printf("Token [%d,%d]: '%s'\n", i, j, split[j]);
            
            map->z_ij[i][j] = ft_atoi(split[j]);
            map->color[i][j] = get_color(split[j], map);
            
            // Debug after conversion
            ft_printf("Converted: z=%d, color=%06X\n", map->z_ij[i][j], map->color[i][j]);
        }
        
        free_matrix(split);
        free(line);
    }
}

int get_color(char *str, t_map *map)
{
    while (*str && (ft_isdigit(*str) || *str == '-' || *str == '+' || *str == ','))
        str++;
    if (*str && (*str == 'x' || *str == 'X'))
    {
        map->is_color = 1;
        ft_striter_tolower(str + 1);
        return (ft_atoi_base(str + 1, 16));
    }
    return (WHITE);
}