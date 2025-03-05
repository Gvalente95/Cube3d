/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:37:22 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 12:30:01 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	show_init_information(t_md *md)
{
	print_vec2(md->map.size, "map size");
	print_vec3f(md->plr.pos, "plr pos");
	print_vec3f(md->plr.rot, "plr rot");
	printf("map name: %s\nmap content: \n%s\n", md->map.name, md->map.buffer);
}

void	show_update_information(t_md *md)
{
	rnd_abs_txt(md, get_v4(0, 0, COLOR_WHITE, DEBUG_SCALE), \
"pos x%.2f y%.2f z%.2f", md->plr.pos.x, md->plr.pos.y, md->plr.pos.z);
	rnd_abs_txt(md, get_v4(0, DEBUG_SCALE * 1.5, COLOR_WHITE, DEBUG_SCALE), \
"mov x%.2f y%.2f z%.2f", md->plr.mov.x, md->plr.mov.y, md->plr.mov.z);
	rnd_abs_txt(md, get_v4(0, DEBUG_SCALE * 3, COLOR_WHITE, DEBUG_SCALE), \
"rot x%.2f y%.2f z%.2f", md->plr.rot.x, md->plr.rot.y, md->plr.rot.z);
}
