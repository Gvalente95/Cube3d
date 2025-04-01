/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:37:22 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/31 14:06:22 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	show_init_information(t_md *md)
{
	int	i;

	print_vec2(md->map.size, "map size");
	print_vec3f(md->plr.pos, "plr pos");
	print_vec3f(md->plr_rot, "plr rot");
	print_vec3(md->plr.coord, "plr coord");
	printf("map name: %s\nmap content: \n", md->map.name);
	i = -1;
	while (md->map.buffer[++i])
	{
		if (md->map.buffer[i] == '\n')
			printf("n\n");
		else
			printf("%c", md->map.buffer[i]);
	}
}

void	show_fps(t_md *md, t_vec2 pos)
{
	const int	colors[5] = {RGB_RED, RGB_ORANGE, RGB_YELLOW, \
		RGB_GREEN, RGB_BLUE};
	int			color_index;
	int			color;
	t_vec4		txt_data;

	color_index = minmaxf(0, 4, md->timer.prv_fps / 10);
	color = md->rgb[colors[color_index]];
	txt_data = get_v4(pos.x, pos.y, color, md->prm.txt_sc);
	rnd_abs_txt(md, txt_data, "fps:	%d", md->timer.prv_fps);
	if (!md->prm.debug_mode)
		return ;
	txt_data.b = -1;
	txt_data.g -= md->prm.txt_sc * 1.5;
	rnd_abs_txt(md, txt_data, "time: %.3f", md->timer.cur_tm);
	txt_data.g -= md->prm.txt_sc * 1.5;
	rnd_abs_txt(md, txt_data, "delta:	%.3f", md->timer.delta_time);
}

void	show_update_information(t_md *md)
{
	int	y;

	if (!md->prm.debug_mode)
		return ;
	y = 0;
	show_vec3f(md, "input", md->input_mov, get_v2(0, y++));
	show_vec2(md, "input offst", md->input_offst, get_v2(0, y++));
	show_vec3f(md, "mov", md->plr.mov, get_v2(0, y++));
	show_vec3f(md, "wrd_mv", md->plr_wrd_mv, get_v2(0, y++));
	show_vec3f(md, "pos", md->plr.pos, get_v2(0, y++));
	show_vec3f(md, "dir", md->plr.dir, get_v2(0, y++));
	show_vec3f(md, "rot", md->plr_rot, get_v2(0, y++));
	show_float(md, "angle: ", md->plr.angle, get_v2(0, y++));
	show_int(md, "FOV: ", (int)md->prm.fov, get_v2(0, y++));
	show_vec3(md, "crd", md->plr.coord, get_v2(0, y++));
	show_vec3f(md, "mouse pos", md->mouse.pos, get_v2(0, y++));
	show_vec2(md, "mouse delta", md->mouse.delta, get_v2(0, y++));
	show_vec2(md, "mouse wrd", md->mouse.world, get_v2(0, y++));
	show_vec2(md, "mouse grd", md->mouse.grid_pos, get_v2(0, y++));
	show_int(md, "t_len", md->t_len, get_v2(0, y++));
	show_float(md, "cam z", md->cam_pos.z, get_v2(0, y++));
}

void	print_color(int color, const char *label)
{
	if (label)
		printf("%s: ", label);
	printf("r%d g%d b%d a%d\n", \
		(color >> 16) & 0xFF, \
		(color >> 8) & 0xFF, \
		color & 0xFF, \
		(color >> 24) & 0xFF);
}
