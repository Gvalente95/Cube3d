/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:10:05 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/07 01:21:53 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

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

static t_vec2	get_map_size(char *map)
{
	int		cur_width;
	t_vec2	size;

	cur_width = 0;
	size = get_v2(0, 0);
	while (*map++)
	{
		if (*map == '\n')
		{
			size.y++;
			if (cur_width - 1 > size.x)
				size.x = cur_width;
			cur_width = 0;
		}
		else
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
	md->map.size = get_map_size(md->map.buffer);
	return (1);
}
