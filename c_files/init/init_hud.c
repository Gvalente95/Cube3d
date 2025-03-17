/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hud.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:44:55 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/16 23:47:40 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"


static void	init_gun(t_md *md, t_hud *hud)
{
	t_vec2	gun_size;

	gun_size = get_v2(md->win_size.x * .75, md->win_size.y * .75);
	hud->gun[0] = init_img_data(md, gun_size, "png/gun/0.png", -1);
	hud->gun[1] = init_img_data(md, gun_size, "png/gun/1.png", -1);
}

static void	init_sky(t_md *md, t_hud *hud)
{
	int		i;
	int		pow;
	t_vec2	r_pos;

	hud->base_sky = init_img_data(md, md->win_size, NULL, hud->sky_color);
	hud->sky = init_img_data(md, md->win_size, NULL, hud->sky_color);
	i = -1;
	while (++i < STARS_AMOUNT)
	{
		pow = r_range(50, 255);
		r_pos = get_v2(r_range(0, md->win_size.x), r_range(0, md->win_size.y));
		draw_pixels(md->hud.sky, \
			get_v2(r_pos.x, r_pos.y), \
			get_v2(2, 2), \
			vec4_to_color(pow, pow, pow, pow));
	}
}

static void	init_floor(t_md *md, t_hud *hud)
{
	char	*ground_path;

	hud->base_floor = init_img_data(md, md->win_size, NULL, hud->floor_color);
	ground_path = ft_megajoin(md->image_dir, "/ground", md->img_format, NULL);
	hud->floor = init_img_data(md, md->win_size, ground_path, -1);
	free(ground_path);
	flush_img(hud->floor, hud->floor_color, 0.8, 0);
}

void	init_hud(t_md *md)
{
	md->hud.active_background = 0;
	md->hud.bgr_color = md->rgb[2];
	md->screen = init_img_data(md, md->win_size, NULL, md->hud.bgr_color);
	md->hud.background = init_img_data(md, md->win_size, NULL, -1);
	init_floor(md, &md->hud);
	init_sky(md, &md->hud);
	init_gun(md, &md->hud);
}
