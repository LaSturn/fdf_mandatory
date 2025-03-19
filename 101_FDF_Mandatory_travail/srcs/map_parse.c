#include "fdf.h"

/**
 * @brief Parses a map file to determine dimensions.
 *
 * This function opens the specified file and reads its contents to determine
 * the map's width (number of columns) and height (number of rows).
 *
 * @param file_name Path to the map file.
 * @return A pointer to the initialized map structure.
 */
t_map	*parse_map_dimensions(char *file_name)
{
	t_map	*map;
	int		fd;

	map = ft_calloc(1, sizeof(t_map));
	if (!map)
		print_error_and_exit("Map allocation failed", 0);
	map->is_color = 0;
	init_map_dimensions(file_name, map, &fd);
	count_map_height(map, fd);
	close(fd);
	return (map);
}

/**
 * @brief Handles reading first line and setting initial dimensions.
 *
 * @param file_name The map file path.
 * @param map The map structure to initialize.
 * @param fd File descriptor of the open map file.
 * @return 1 on success, 0 on failure.
 */
int	init_map_dimensions(char *file_name, t_map *map, int *fd)
{
	char	*line;

	*fd = open(file_name, O_RDONLY);
	if (*fd < 0)
	{
		free(map);
		print_error_and_exit("Cannot open file", 1);
	}
	line = get_next_line(*fd);
	if (!line)
	{
		close(*fd);
		free(map);
		print_error_and_exit("Empty file", 0);
	}
	map->map_width = nbr_in_a_line(map, line);
	map->map_height = 1;
	free(line);
	return (1);
}

/**
 * @brief Processes remaining lines to determine map height.
 *
 * @param map The map structure to update.
 * @param fd File descriptor of the open map file.
 */
void	count_map_height(t_map *map, int fd)
{
	char	*line;
	int		width;

	line = get_next_line(fd);
	while (line)
	{
		width = nbr_in_a_line(map, line);
		free(line);

		if (width != map->map_width)
		{
			close(fd);
			free(map);
			print_error_and_exit("Inconsistent width", 1);
		}
		map->map_height++;
		line = get_next_line(fd);
	}
}

/**
 * @brief Counts the number of values in a line.
 *
 * This function splits a line by spaces and counts the number of elements,
 * which determines the width of the map at that row.
 *
 * @param map The map structure for error handling.
 * @param line The line to process.
 * @return The number of elements in the line.
 */
int	nbr_in_a_line(t_map *map, char *line)
{
	char	**split;
	int		count;
	int		i;

	if (!line)
	{
		free_map_memory(map);
		print_error_and_exit("Empty line in map file", 1);
	}
	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	if (line[i] == '\n')
		line[i] = '\0';
	while (i > 0 && line[i - 1] == ' ')
		line[--i] = '\0';
	split = ft_split(line, ' ');
	if (!split)
	{
		free_map_memory(map);
		print_error_and_exit("Split failed", 1);
	}
	count = 0;
	while (split[count])
		count++;
	free_matrix(split);
	return (count);
}

/**
 * @brief Checks if a file is readable.
 *
 * This function attempts to open the specified file to verify it exists
 * and the program has permission to read it.
 *
 * @param filename Path to the file to check.
 */
void	check_file_readable(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		print_error_and_exit("File not readable", 1);
	close(fd);
}
