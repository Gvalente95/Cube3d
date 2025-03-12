/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:55:04 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/12 11:36:07 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static char	*extract_line(const char *str)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	if (!str || !str[0])
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	j = -1;
	while (++j < i && str[j] != '\n')
		line[j] = str[j];
	line[i] = '\0';
	return (line);
}

static void	add_texture_img(t_md *md, char *line, t_wrd_dir dir)
{
	void	*txt_img;
	char	*path;
	int		fd;
	t_vec2	size;

	if (ft_strlen(line) <= 4)
		free_and_quit(md, "wrong value for txtr", line);
	path = line + 3;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		free_and_quit(md, "file at path not found for txtr", path);
	close(fd);
	txt_img = add_img(path, &size.x, &size.y, md);
	if (!txt_img)
		free_and_quit(md, "no txtr", path);
	txt_img = scale_abs_img(md, txt_img, &size, md->e_sizes[nt_wall]);
	md->wall_txtr[dir] = txt_img;
	txt_img = add_img(path, &size.x, &size.y, md);
	txt_img = scale_abs_img(md, txt_img, &size, md->e_sizes_2d[nt_wall]);
	md->wall_txtr_2d[dir] = txt_img;
	if (md->debug_mode)
		printf("%s texture[%d] correctly set\n", path, dir);
}

static int	parse_file_line(char *line, t_md *md)
{
	if (!ft_strncmp(line, "NO ", 3))
		add_texture_img(md, line, NORTH);
	else if (!ft_strncmp(line, "SO ", 3))
		add_texture_img(md, line, SOUTH);
	else if (!ft_strncmp(line, "EA ", 3))
		add_texture_img(md, line, EAST);
	else if (!ft_strncmp(line, "WE ", 3))
		add_texture_img(md, line, WEST);
	else if (!ft_strncmp(line, "F ", 2))
		md->floor_color = str_to_color(line + 2);
	else if (!ft_strncmp(line, "C ", 2))
		md->sky_color = str_to_color(line + 2);
	else if (char_in_str('1', line) || char_in_str('0', line))
		return (0);
	return (1);
}

static char	*parse_file_data(t_md *md)
{
	char	*file_content;
	char	*line;

	file_content = md->map.buffer;
	line = extract_line(file_content);
	while (line)
	{
		if (!parse_file_line(line, md))
			break ;
		file_content = ft_strchr(file_content, '\n');
		if (file_content)
			file_content++;
		else
			break ;
		setstr(&line, extract_line(file_content));
	}
	return (safe_free(line), ft_strdup(file_content));
}

void	init_map_data(t_md *md)
{
	md->wall_txtr = malloc(sizeof(void *) * 5);
	md->wall_txtr_2d = malloc(sizeof(void *) * 5);
	md->wall_txtr[4] = NULL;
	md->wall_txtr_2d[4] = NULL;
	setstr(&md->map.buffer, parse_file_data(md));
	if (!md->map.buffer)
		free_and_quit(md, "no map found", NULL);
}
