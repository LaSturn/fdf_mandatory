#include "fdf.h"

/**
 * @brief Processes the map file to extract data.
 *
 * This function coordinates the allocation of memory and extraction of values
 * from the map file.
 *
 * @param file_name The path to the map file.
 * @param map The map structure to fill with data.
 */
void	get_map(char *file_name, t_map *map)
{
	alloc_memory_for_map(map);
	stock_map_values(file_name, map);
}

/**
 * @brief Allocates memory for the map's data arrays.
 *
 * This function allocates 2D arrays for z-values and colors.
 *
 * @param map The map structure to allocate memory for.
 */
void	alloc_memory_for_map(t_map *map)
{
	int	i;

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

/**
 * @brief Opens the map file and extracts values.
 *
 * This function opens the file and passes the file descriptor to
 * the extraction function.
 *
 * @param file_name The path to the map file.
 * @param map The map structure to fill with data.
 */
void	stock_map_values(char *file_name, t_map *map)
{
	int	fd;

	fd = open(file_name, O_RDONLY);
	if (fd < 0)
	{
		free_map_memory(map);
		print_error_and_exit("Error opening file", 1);
	}
	extract_map_data_z_color(fd, map);
	close(fd);
}

/**
 * @brief Processes a single line from the map file.
 *
 * @param line The line to process.
 * @param map The map structure to update.
 * @param i The current row index.
 */
void	process_map_line(char *line, t_map *map, int i)
{
	char	**split;
	int		j;

	split = ft_split(line, ' ');
	if (!split)
	{
		free(line);
		free_map_memory(map);
		print_error_and_exit("Memory allocation failed while splitting", 1);
	}
	j = -1;
	while (++j < map->map_width && split[j])
	{
		map->z_ij[i][j] = ft_atoi(split[j]);
		map->color[i][j] = get_color(split[j], map);
	}
	free_matrix(split);
}

/**
 * @brief Extracts z-values and colors from the map file.
 *
 * This function reads the file line by line and processes each line
 * to extract altitude and color data.
 *
 * @param fd File descriptor of the open map file.
 * @param map The map structure to fill with data.
 */
void	extract_map_data_z_color(int fd, t_map *map)
{
	char	*line;
	int		i;

	i = -1;
	while (++i < map->map_height)
	{
		line = get_next_line(fd);
		if (!line)
		{
			free_map_memory(map);
			print_error_and_exit("Unexpected end of file", 1);
		}
		process_map_line(line, map, i);
		free(line);
	}
	line = get_next_line(fd);
	free(line);
}
