/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:37:22 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/07 14:03:11 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	show_init_information(t_md *md)
{
	print_vec2(md->map.size, "map size");
	print_vec3f(md->plr.pos, "plr pos");
	print_vec3f(md->plr.rot, "plr rot");
	print_vec3(md->plr.coord_pos, "plr coord");
	printf("map name: %s\nmap content: \n%s\n", md->map.name, md->map.buffer);
}

void	show_vec3(t_md *md, char *label, t_vec3 vec, t_vec2 pos)
{
	rnd_abs_txt(md, get_v4(pos.x, pos.y * (DEBUG_SCALE * 1.5), COLOR_WHITE, DEBUG_SCALE), \
"%s x%d y%d z%d", label, vec.x, vec.y, vec.z);
}

void	show_vec3f(t_md *md, char *label, t_vec3f vec, t_vec2 pos)
{
	rnd_abs_txt(md, get_v4(pos.x, pos.y * (DEBUG_SCALE * 1.5), COLOR_WHITE, DEBUG_SCALE), \
"%s x%.2f y%.2f z%.2f", label, vec.x, vec.y, vec.z);
}

void	show_update_information(t_md *md)
{
	int	y;

	y = 0;
	show_vec3f(md, "pos", md->plr.pos, get_v2(0, y++));
	show_vec3f(md, "mov", md->plr.mov, get_v2(0, y++));
	show_vec3f(md, "rot", md->plr.rot, get_v2(0, y++));
	show_vec3(md, "crd", md->plr.coord_pos, get_v2(0, y++));
	show_vec3f(md, "mouse pos", md->mouse_pos, get_v2(0, y++));
	show_vec3f(md, "mouse wrd", md->mouse_world_pos, get_v2(0, y++));
	show_vec3(md, "mouse grd", v2_to_v3(md->mouse_grid_pos), get_v2(0, y++));
}
