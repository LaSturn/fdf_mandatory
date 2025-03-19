#include "fdf.h"

t_map *parse_map_dimensions(char *file_name)
{
    t_map *map = ft_calloc(1, sizeof(t_map));
    int fd;
    char *line;
    int width;

    if (!map)
        print_error_and_exit("Map allocation failed", 0);
    fd = open(file_name, O_RDONLY);
    if (fd < 0)
    {
        free(map);
        print_error_and_exit("Cannot open file", 1);
    }
    line = get_next_line(fd);
    if (!line)
    {
        close(fd);
        free(map);
        print_error_and_exit("Empty file", 0);
    }
    map->map_width = nbr_in_a_line(map, line);
    map->map_height = 1;
    free(line);
    while ((line = get_next_line(fd)))
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
    }
    close(fd);
    return (map);
}
/*
// version soft
int nbr_in_a_line(t_map *map, char *line)
{
    char **split;
    int count = 0;

    if (!line)
    {
        free_map_memory(map);
        print_error_and_exit("Empty line in map file", 1);
    }
    split = ft_split(line, ' ');
    if (!split)
    {
        free_map_memory(map);
        print_error_and_exit("Split failed", 1);
    }
    while (split[count])
        count++;
    free_matrix(split);
    return (count);
}
    */
// version finale
int nbr_in_a_line(t_map *map, char *line)
{
	char **split;
	int count;
	int i;

	if (!line)
	{
		free_map_memory(map);
		print_error_and_exit("Empty line in the map file (in nbr_in_a_line)", 1);
	}

	// Remove trailing newline character if present
	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	if (line[i] == '\n')
		line[i] = '\0';

	// Remove trailing spaces
	while (i > 0 && line[i - 1] == ' ')
		line[--i] = '\0';

	split = ft_split(line, ' ');
	if (!split)
	{
		free_map_memory(map);
		print_error_and_exit("split is NULL (in nbr_in_a_line)", 1);
	}
	count = 0;
	while (split[count])
		count++;
	free_matrix(split);
	return (count);
}

void check_file_readable(const char *filename)
{
    int fd;
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        close(fd); // vérifier si besoin de close
        print_error_and_exit("File not readable", 1);
    }
    close(fd);
}