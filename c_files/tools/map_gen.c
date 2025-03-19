/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_gen.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 23:10:51 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 05:20:35 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	is_border(t_vec2 size, t_vec3 pos)
{
	int	is_it_border;

	is_it_border = 0;
	if (pos.x <= 0)
		is_it_border = 1;
	if (pos.x >= size.x - 1)
		is_it_border = 1;
	if (pos.y <= 0)
		is_it_border = 1;
	if (pos.y >= size.y - 1)
		is_it_border = 1;
	return (is_it_border);
}

int	get_char_amount(char *buffer, char c)
{
	int	i;
	int	amount;

	amount = 0;
	i = -1;
	while (buffer[++i])
		if (buffer[i] == c)
			amount++;
	return (amount);
}

static char	*generate_map(t_vec2 size)
{
	char	*buffer;
	t_vec3	pos;
	int		current_width;

	buffer = malloc((size.x + 1) * size.y + 1);
	pos = get_v3(-1, -1, 0);
	current_width = size.x - r_range(0, size.x * .2);
	while (++pos.y < size.y)
	{
		if (r_range(0, 5) == 0)
			current_width = size.x - r_range(0, size.x * .2);
		pos.x = -1;
		while (++pos.x < current_width)
		{
			buffer[pos.z] = '0';
			if (is_border(get_v2(current_width, size.y), pos))
				buffer[pos.z] = '1';
			pos.z++;
		}
		pos.x--;
		while (++pos.x < size.x)
			buffer[pos.z++] = ' ';
		buffer[pos.z++] = '\n';
	}
	buffer[pos.z] = '\0';
	return (buffer);
}

static char	*set_map_with_base(char *map)
{
	const char	data_info[6][50] = {
		"NO ent/wall/NORTH.xpm\n", "SO ent/wall/SOUTH.xpm\n", \
		"WE ent/wall/WEST.xpm\n", "EA ent/wall/EAST.xpm\n", \
		"F 96,64,32\n", "C 32,64,128\n"
	};
	int			i;
	char		*full_data;
	char		*tmp;

	full_data = ft_strdup("");
	if (!full_data)
		return (NULL);
	i = -1;
	while (++i < 6)
	{
		tmp = full_data;
		full_data = ft_strjoin(tmp, data_info[i]);
		free(tmp);
		if (!full_data)
			return (NULL);
	}
	tmp = full_data;
	full_data = ft_strjoin(tmp, map);
	free(tmp);
	return (full_data);
}

char	*get_new_map(int difficulty, t_vec2 *size, char *data_info)
{
	const char		map_name[] = "Randomap.cub";
	char			*map;
	char			*map_data;
	int				fd;
	unsigned int	seed;

	seed = get_random_seed();
	size->x = r_range_seed(&seed, size->x * .8, size->x * 1.2);
	size->y = r_range_seed(&seed, size->y * .8, size->y * 1.2);
	map = generate_map(*size);
	generate_maze(map, *size);
	set_characters(map, difficulty);
	if (!data_info)
		map_data = set_map_with_base(map);
	else
		map_data = ft_megajoin(map, "\n", data_info, NULL);
	free(map);
	fd = open(map_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (printf("unable to create map"), free(map_data), NULL);
	write(fd, map_data, ft_strlen(map_data));
	printf("map generated: \n%s\n", map_data);
	close(fd);
	return (map_data);
}
