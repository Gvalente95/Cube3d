/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:55:04 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 14:26:51 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	init_background(t_md *md)
{
	md->floor = mlx_new_image(md->mlx, md->win_size.x, md->win_size.y);
	md->sky = mlx_new_image(md->mlx, md->win_size.x, md->win_size.y);
	if (!md->floor || !md->sky)
		free_and_quit(md, "alloc of sky/floor", NULL);
	md->floor = set_img_color(md->floor, md->win_size, md->floor_color, 1);
	md->sky = set_img_color(md->sky, md->win_size, md->sky_color, 1);
	if (!md->floor || !md->sky)
		free_and_quit(md, "alloc of sky/floor", NULL);
}

static void	add_texture_img(t_md *md, char *line, int index)
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
	txt_img = scale_abs_img(md, txt_img, &size, get_v2(250, 250));
	md->wall_txtr[index] = txt_img;
	if (md->debug_mode)
		printf("%s texture[%d] correctly set\n", path, index);
}

static int	parse_file_line(char *line, t_md *md)
{
	if (!ft_strncmp(line, "NO ", 3))
		add_texture_img(md, line, 0);
	else if (!ft_strncmp(line, "SO ", 3))
		add_texture_img(md, line, 1);
	else if (!ft_strncmp(line, "WE ", 3))
		add_texture_img(md, line, 2);
	else if (!ft_strncmp(line, "EA ", 3))
		add_texture_img(md, line, 3);
	else if (!ft_strncmp(line, "F ", 2))
		md->floor_color = convert_line_to_color(line + 2);
	else if (!ft_strncmp(line, "C ", 2))
		md->sky_color = convert_line_to_color(line + 2);
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
	md->wall_txtr[4] = NULL;
	setstr(&md->map.buffer, parse_file_data(md));
	if (!md->map.buffer)
		free_and_quit(md, "no map found", NULL);
	init_background(md);
}
