/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 23:46:39 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 14:27:34 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	convert_line_to_color(const char *line)
{
	t_vec4		rgba;
	const char	*values;
	char		**splits;

	if (!line || ft_strlen(line) <= 3)
		return (convert_line_to_color("255,255,255"));
	values = line;
	while (values && !ft_isdigit(*values))
		values++;
	if (!*values)
		return (-1);
	splits = ft_split(values, ',');
	if (!splits)
		return (-1);
	rgba.r = ft_atoi(splits[0]);
	if (splits[1])
		rgba.g = ft_atoi(splits[1]);
	if (splits[1] && splits[2])
		rgba.b = ft_atoi(splits[2]);
	free_void_array((void **)splits, -1);
	return ((rgba.r << 16) | (rgba.g << 8) | rgba.b);
}

void	render_entities(t_md *md)
{
	t_vec2	pos;
	t_vec2	rnd_pos;
	int		i;

	pos = get_v2(0, 0);
	i = -1;
	while (md->map.buffer[++i])
	{
		if (md->map.buffer[i] == '1')
		{
			rnd_pos = get_v2((pos.x * 250) - md->cam_offset.x, \
			(pos.y * 250) - md->cam_offset.y);
			if (rnd_pos.x > -250 && rnd_pos.x < md->win_size.x + 250 && rnd_pos.y > -250 && rnd_pos.y < md->win_size.y + 250)
			{
				mlx_put_image_to_window(md->mlx, md->win, md->wall_txtr[0], rnd_pos.x, rnd_pos.y);
			}
		}
		else if (md->map.buffer[i] == '\n')
		{
			pos.x = 0;
			pos.y++;
		}
		else
			pos.x++;
	}
}

void	render_minimap(t_md *md)
{
	t_vec2	plr_p;
	t_vec2	centr;
	void	*plr;

	centr = get_v2(md->win_size.x - md->mmap_size.x, 0);
	mlx_put_image_to_window(md->mlx, md->win, md->minimap, centr.x, centr.y);
	plr_p = get_v2(centr.x, centr.y);
	plr_p.x += md->plr.pos.x / 10;
	plr_p.y += md->plr.pos.y / 10;
	plr = mlx_new_image(md->mlx, md->mmap_ic_size, md->mmap_ic_size);
	set_img_color(plr, get_v2(md->mmap_ic_size, md->mmap_ic_size), convert_line_to_color("0,255,0"), 1);
	mlx_put_image_to_window(md->mlx, md->win, plr, plr_p.x, plr_p.y);
	mlx_destroy_image(md->mlx, plr);
}

void	render(t_md *md)
{
	mlx_put_image_to_window(md->mlx, md->win, md->sky, 0, -md->win_size.y / 2);
	mlx_put_image_to_window(md->mlx, md->win, md->floor, 0, md->win_size.y / 2);
	render_entities(md);
	if (md->show_minimap)
		render_minimap(md);
	render_cursor(md, 0);
	if (md->debug_mode)
		show_update_information(md);
}
