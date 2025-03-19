/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:10:05 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 05:38:20 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	trim_excess_spaces(char **line)
{
	int		len;
	int		new_len;
	char	*trimmed_line;

	if (!*line)
		return ;
	len = ft_strlen(*line);
	if (len <= 0)
		return ;
	new_len = len - 1;
	while (new_len > 0 && (*line)[new_len - 1] == ' ')
		new_len--;
	if (new_len == len)
		return ;
	trimmed_line = ft_strndup(*line, new_len);
	free(*line);
	*line = ft_strjoin(trimmed_line, "\n");
	free(trimmed_line);
}

static char	*get_map(char *file_name)
{
	int		fd;
	char	*line;
	char	*content;
	char	*buffer;

	fd = open(file_name, O_RDONLY, 0644);
	if (fd == -1)
		return (NULL);
	content = ft_strdup("");
	if (!content)
		return (close(fd), NULL);
	line = get_next_line(fd);
	while (line)
	{
		trim_excess_spaces(&line);
		buffer = ft_strjoin(content, line);
		free(content);
		free(line);
		if (!buffer)
			return (close(fd), NULL);
		content = buffer;
		line = get_next_line(fd);
	}
	close(fd);
	return (content);
}

static t_vec2	get_map_size(t_md *md, char *map)
{
	int		cur_width;
	t_vec2	size;
	int		i;

	cur_width = 0;
	size = get_v2(0, 0);
	i = -1;
	while (map[++i])
	{
		if (map[i] == '\n')
		{
			if (map[i - 1] != '1' || (map[i + 1] != '1' && map[i + 1]))
				free_and_quit(md, "Error\nOpen Right Wall\n", map + i);
			size.y++;
			if (cur_width - 1 > size.x)
				size.x = cur_width;
			cur_width = 0;
		}
		else if (map[i] != ' ')
			cur_width++;
	}
	if (cur_width > size.x)
		size.x = cur_width;
	size.y++;
	return (size);
}

int	init_map(t_md *md, char *file_name)
{
	char	*extension_name;

	extension_name = ft_strchr(file_name, '.');
	if (!extension_name || ft_strncmp(extension_name, ".cub", 4))
		free_and_quit(md, "Wrong file extension", extension_name);
	md->map.name = file_name;
	md->map.buffer = get_map(file_name);
	if (!md->map.buffer)
		free_and_quit(md, "map data not found", md->map.buffer);
	md->map.len = ft_strlen(md->map.buffer);
	init_map_data(md);
	md->map.size = get_map_size(md, md->map.buffer);
	md->map.len = ft_strlen(md->map.buffer);
	if (!validate_map(md, md->map.buffer, md->map.len))
		free_and_quit(md, NULL, NULL);
	return (1);
}
