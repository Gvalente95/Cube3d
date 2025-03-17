/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:37:22 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/15 17:05:35 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	show_init_information(t_md *md)
{
	print_vec2(md->map.size, "map size");
	print_vec3f(md->plr.pos, "plr pos");
	print_vec3(md->plr.rot, "plr rot");
	print_vec3(md->plr.coord_pos, "plr coord");
	printf("map name: %s\nmap content: \n%s\n", md->map.name, md->map.buffer);
}

void	show_vec2(t_md *md, char *label, t_vec2 vec, t_vec2 pos)
{
	rnd_abs_txt(md, \
		get_v4(pos.x * (md->txt_scale * 1.5), pos.y * (md->txt_scale * 1.5), \
		COLOR_WHITE, md->txt_scale), \
		"%s x%d y%d", \
		label, vec.x, vec.y);
}


void	show_vec3(t_md *md, char *label, t_vec3 vec, t_vec2 pos)
{
	rnd_abs_txt(md, \
		get_v4(pos.x * (md->txt_scale * 1.5), pos.y * (md->txt_scale * 1.5), \
		COLOR_WHITE, md->txt_scale), \
		"%s x%d y%d z%d", \
		label, vec.x, vec.y, vec.z);
}

void	show_int(t_md *md, char *label, int value, t_vec2 pos)
{
	rnd_abs_txt(md, \
		get_v4(pos.x * (md->txt_scale * 1.5), pos.y * (md->txt_scale * 1.5), \
		COLOR_WHITE, md->txt_scale), "%s %d", label, value);
}

void	show_float(t_md *md, char *label, float value, t_vec2 pos)
{
	rnd_abs_txt(md, \
		get_v4(pos.x * (md->txt_scale * 1.5), pos.y * (md->txt_scale * 1.5), \
		COLOR_WHITE, md->txt_scale), "%s %-3f", label, value);
}

void	show_vec3f(t_md *md, char *label, t_vec3f vec, t_vec2 pos)
{
	rnd_abs_txt(md, \
		get_v4(pos.x * (md->txt_scale * 1.5), pos.y * (md->txt_scale * 1.5), \
		COLOR_WHITE, md->txt_scale), \
		"%s x%.2f y%.2f z%.2f", \
		label, vec.x, vec.y, vec.z);
}

void	show_fps(t_md *md, t_vec2 pos)
{
	const int	fps_color[5] = { RGB_RED, RGB_ORANGE, RGB_YELLOW, RGB_GREEN, RGB_BLUE};
	int			color_index;
	int			color;

	color_index = minmaxf(0, 4, md->timer.prv_fps / 10);
	color = md->rgb[fps_color[color_index]];
	rnd_abs_txt(md, get_v4(pos.x, pos.y, color, md->txt_scale), "fps:	%d", md->timer.prv_fps);
	if (!md->debug_mode)
		return ;
	rnd_abs_txt(md, get_v4(pos.x, pos.y + md->txt_scale * 1.5, -1, md->txt_scale), \
	"time:	%.3f", md->timer.current_time);
	rnd_abs_txt(md, get_v4(pos.x, pos.y + md->txt_scale * 3, -1, md->txt_scale), \
	"delta:	%.3f", md->timer.delta_time);
}

void	show_update_information(t_md *md)
{
	int	y;

	if (!md->debug_mode)
		return ;
	y = 0;
	show_vec3f(md, "input", md->input_mov, get_v2(0, y++));
	show_vec2(md, "input offst", md->input_offst, get_v2(0, y++));
	show_vec3f(md, "mov", md->plr.mov, get_v2(0, y++));
	show_vec3f(md, "wrd_mv", md->plr_wrd_mv, get_v2(0, y++));
	show_vec3f(md, "wrd_mv offset", md->wrd_mv_offst, get_v2(0, y++));
	show_vec3f(md, "pos", md->plr.pos, get_v2(0, y++));
	show_vec3(md, "rot", md->plr.rot, get_v2(0, y++));
	show_float(md, "angle: ", md->plr.angle, get_v2(0, y++));
	show_int(md, "particles: ", md->particles_alive, get_v2(0, y++));
	show_vec3(md, "crd", md->plr.coord_pos, get_v2(0, y++));
	show_vec3f(md, "mouse pos", md->mouse_pos, get_v2(0, y++));
	show_vec2(md, "mouse wrd", md->mouse_world_pos, get_v2(0, y++));
	show_vec2(md, "mouse grd", md->mouse_grid_pos, get_v2(0, y++));
}
