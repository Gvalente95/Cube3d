/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:37:22 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/21 15:15:22 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	show_init_information(t_md *md)
{
	t_vec2	pos;
	t_ent	*e;

	print_vec2(md->map.size, "map size");
	print_vec3f(md->plr.pos, "plr pos");
	print_vec3f(md->cam.rot, "plr rot");
	print_vec3(md->plr.coord, "plr coord");
	printf("map name: %s\nmap content: \n", md->map.name);
	return ;
	pos = v2(-1);
	while (++pos.y < md->map.size.y)
	{
		pos.x = -1;
		while (++pos.x < md->map.size.x)
		{
			e = get_mapped_at_cord(md, pos);
			if (e)
				printf("%c", e->character);
			else
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}

void	show_debug_time(t_md *md, t_txtd txt_data)
{
	txt_data.y -= md->prm.txt_sc * 1.5;
	rnd_fast_txt(md, txt_data, "g_time: %d", md->timer.time);
	txt_data.y -= md->prm.txt_sc * 1.5;
	rnd_fast_txt(md, txt_data, "delta:	%.3f", md->timer.delta_time);
}

void	show_fps(t_md *md, t_vec2 pos)
{
	const int	colors[5] = {RGB_RED, RGB_ORANGE, RGB_YELLOW, \
		RGB_GREEN, RGB_BLUE};
	const int	color_index = minmaxf(0, 4, md->timer.prv_fps / 10);
	const int	color = md->rgb[colors[color_index]];
	t_txtd		txt_data;
	float		fps_gain;

	txt_data = (t_txtd){pos.x, pos.y, color, md->prm.txt_sc, NULL};
	rnd_fast_txt(md, txt_data, "fps %d", md->timer.prv_fps);
	txt_data.y -= md->prm.txt_sc * 1.5;
	fps_gain = md->timer.avrg_fps_prev - md->timer.avrg_fps;
	if (isinf(fps_gain) || isnan(fps_gain))
		fps_gain = 0;
	txt_data.color = md->rgb[colors[(fps_gain < 0) * 3]];
	if (fps_gain > 0)
		rnd_fast_txt(md, txt_data, "+%.1f", fps_gain);
	else
		rnd_fast_txt(md, txt_data, "%.1f", fps_gain);
	txt_data.color = -1;
	txt_data.y -= md->prm.txt_sc * 1.5;
	if (md->prm.use_thrd)
		rnd_fast_txt(md, txt_data, "T-on");
	if (md->prm.debug_mode)
		show_debug_time(md, txt_data);
}

void	show_update_information(t_md *md)
{
	int	y;

	y = 0;
	show_vec3f(md, "input", md->cam.input_mov, get_v2(0, y++));
	show_vec2(md, "input offst", md->cam.input_offst, get_v2(0, y++));
	show_vec3f(md, "mov", md->plr.mov, get_v2(0, y++));
	show_vec3f(md, "wrd_mv", md->cam.plr_wrd_mv, get_v2(0, y++));
	show_vec3f(md, "pos", md->plr.pos, get_v2(0, y++));
	show_vec3f(md, "dir", md->plr.dir, get_v2(0, y++));
	show_vec3f(md, "rot", md->cam.rot, get_v2(0, y++));
	show_float(md, "angle: ", md->plr.angle, get_v2(0, y++));
	show_vec3(md, "crd", md->plr.coord, get_v2(0, y++));
	show_vec3f(md, "mouse pos", md->mouse.pos, get_v2(0, y++));
	show_vec2(md, "mouse delta", md->mouse.delta, get_v2(0, y++));
	show_float(md, "cam z", md->cam.pos.z, get_v2(0, y++));
	show_int(md, "key click", md->last_key, get_v2(0, y++));
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
