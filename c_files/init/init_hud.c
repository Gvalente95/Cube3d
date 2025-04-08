/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hud.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:44:55 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/08 01:37:58 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	init_fx(t_md *md, t_fx_data *fx)
{
	fx->vignette = init_img(md, md->win_sz, NULL, -1);
	flush_img(fx->vignette, 0xFFFFFFFF, .2, 0);
	apply_vignette(fx->vignette, 1, _WHITE);
	fx->anti_alias = 0;
	fx->palette[0] = 0x000000;
	fx->palette[1] = 0x555555;
	fx->palette[2] = 0xAAAAAA;
	fx->palette[3] = 0xFFFFFF;
	fx->palette[4] = 0xFF0000;
	fx->palette[6] = 0x00FF00;
	fx->palette[7] = 0x0000FF;
	fx->palette[8] = 0xFFFF00;
	fx->palette[9] = 0xFF00FF;
	fx->palette[10] = 0x00FFFF;
	fx->fog = .5;
	fx->noise = 0.5;
	fx->palette_size = sizeof(fx->palette) / sizeof(fx->palette[0]);
}

void	init_hud(t_md *md, t_hud *hud)
{
	hud->unlocked_weapons[Knife] = 1;
	hud->unlocked_weapons[Pistol] = 1;
	hud->unlocked_weapons[Machine_Gun] = 1;
	hud->unlocked_weapons[Flame_Thrower] = 1;
	hud->unlocked_weapons[Rocket] = 1;
	md->hud.ammo = 100;
	hud->weapon_frame = 0;
	hud->wpn_index = 1;
	md->hud.hp = 100;
	md->hud.keys = 1;
	hud->bgr_color = md->hud.sky_color;
	md->screen = init_img(md, md->win_sz, NULL, hud->bgr_color);
	hud->overlay = init_img(md, md->win_sz, NULL, -1);
	hud->lock_x_icon = init_img(md, v2(20), "hud/icons/lock_x.xpm", -1);
	hud->lock_y_icon = init_img(md, v2(20), "hud/icons/lock_y.xpm", -1);
	hud->amm_icon = init_img(md, v2(20), "hud/icons/amm.xpm", -1);
	hud->amm2_icon = init_img(md, v2(20), "hud/icons/amm2.xpm", -1);
	hud->key_icon = init_img(md, v2(20), "hud/icons/key.xpm", -1);
	hud->key2_icon = init_img(md, v2(20), "hud/icons/key2.xpm", -1);
	md->hud.center = \
		init_img(md, v2(10), "hud/center.xpm", v4_to_color(150, 100, 150, 255));
	hud->rgun = init_img(md, get_v2(400, 400), "hud/rgun.xpm", -1);
	init_background(md, hud, md->win_sz);
	init_fx(md, &md->fx);
}
