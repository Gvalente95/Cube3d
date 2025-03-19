/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hud.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:44:55 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 05:29:07 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	init_sky(t_md *md, t_hud *hud)
{
	int		i;
	int		pow;
	t_vec2	r_pos;
	int		size;

	hud->base_sky = init_img(md, md->win_size, NULL, hud->sky_color);
	hud->sky = init_img(md, md->win_size, NULL, hud->sky_color);
	i = -1;
	while (++i < STARS_AMOUNT)
	{
		size = r_range(1, 3);
		pow = r_range(50, 255);
		r_pos = get_v2(r_range(0, md->win_size.x), r_range(0, md->win_size.y));
		draw_pixels(md->hud.sky, \
			get_v2(r_pos.x, r_pos.y), \
			get_v2(size, size), \
			vec4_to_color(pow, pow, pow, pow));
	}
}

static void	init_floor(t_md *md, t_hud *hud)
{
	hud->base_floor = init_img(md, md->win_size, NULL, hud->floor_color);
	hud->floor = init_img(md, md->win_size, "ground.xpm", -1);
	flush_img(hud->floor, hud->floor_color, 0.8, 0);
}

void	init_hud(t_md *md)
{
	md->hud.active_background = 1;
	md->hud.bgr_color = md->rgb[2];
	md->screen = init_img(md, md->win_size, NULL, md->hud.bgr_color);
	md->hud.overlay = init_img(md, md->win_size, NULL, -1);
	init_floor(md, &md->hud);
	init_sky(md, &md->hud);
	md->hud.lock_x_icon = init_img(md, get_v2(20, 20), "utils/lock_x.xpm", -1);
	md->hud.lock_y_icon = init_img(md, get_v2(20, 20), "utils/lock_y.xpm", -1);
	md->hud.wpn_index = 1;
	md->hud.weapon_frame = 0;
}
