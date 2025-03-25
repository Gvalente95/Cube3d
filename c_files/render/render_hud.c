/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_hud.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:50:32 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/24 13:47:01 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	draw_locks_icons(t_md *md, t_hud *hud, t_vec2 winsz)
{
	t_vec2	lock_pos;

	lock_pos = get_v2(winsz.x - hud->lock_x_icon->size.x, \
		winsz.y - hud->lock_y_icon->size.y);
	if (md->mouse.lock_rotation.x)
		draw_img(hud->lock_x_icon, md->screen, lock_pos, -1);
	lock_pos.y -= hud->lock_x_icon->size.y;
	if (md->mouse.lock_rotation.y)
		draw_img(hud->lock_y_icon, md->screen, lock_pos, -1);
}

void	draw_info(t_md *md, t_vec2 *screen_pos, t_image *icon, int value)
{
	t_vec4	txt_data;

	txt_data = get_v4(\
		screen_pos->x + icon->size.x * 2, \
		screen_pos->y, \
		-1, \
		md->prm.txt_scale);
	draw_img(icon, md->screen, *screen_pos, -1);
	rnd_abs_txt(md, txt_data, "%d", value);
	screen_pos->x -= icon->size.x * 3;
}

void	draw_game_info(t_md *md, t_hud *hud, t_vec2 winsz)
{
	t_vec2	start_pos;
	int		txt_space;
	int		i;

	if (md->mouse.lock_rotation.x || md->mouse.lock_rotation.y)
		draw_locks_icons(md, hud, winsz);
	txt_space = md->prm.txt_scale * 1.5;
	start_pos = get_v2(winsz.x - txt_space * 5, winsz.y - txt_space);
	i = 0;
	while (i < hud->ammo && md->hud.ammo > md->hud.wpn_index)
	{
		draw_img(md->hud.amm2_icon, md->screen, \
			get_v2(start_pos.x, start_pos.y - i * 2), -1);
		i += 5;
	}
	start_pos = get_v2(winsz.x - txt_space * 10, winsz.y - txt_space);
	draw_info(md, &start_pos, md->hud.key_icon, md->hud.keys);
	draw_info(md, &start_pos, md->hud.key_icon, md->hud.hp);
	if (md->hud.ammo < md->hud.wpn_index)
		rnd_abs_txt(md, get_v4(md->win_size.x / 2 - \
			md->prm.txt_scale * 10, md->win_size.y * .4, \
			md->rgb[RGB_RED], md->prm.txt_scale * 2), "Not enough ammo!");
}

void	draw_hud_weapon(t_md *md, t_hud *hud, t_vec2 winsz)
{
	t_vec2			center_gun;
	t_image			*gun_image;
	t_texture_data	*td;
	t_vec2			gun_sz;
	t_vec2			pos;

	gun_image = md->hud.rgun;
	gun_sz = gun_image->size;
	pos.x = winsz.x - gun_sz.x * .8;
	pos.y = winsz.y - gun_sz.y / 2 - md->cam_pos.z * 3 + md->plr.pos.z * 2.8;
	draw_img(gun_image, md->screen, pos, -1);
	return ;
	td = &md->txd;
	if (!td->wpn_txtr[hud->wpn_index][hud->weapon_frame])
		hud->weapon_frame = 0;
	gun_image = td->wpn_txtr[hud->wpn_index][hud->weapon_frame];
	center_gun = get_v2(winsz.x / 2 - gun_image->size.x / 2 + 30, \
		winsz.y - gun_image->size.y);
	draw_img(gun_image, md->screen, center_gun, -1);
}

void	render_hud_elements(t_md *md, t_hud *hud)
{
	t_vec2	cross_pos;
	t_vec2	winsz;

	winsz = md->win_size;
	cross_pos = (t_vec2){winsz.x / 2 - 5, winsz.y / 2 - 5};
	draw_img(md->center, md->screen, cross_pos, -1);
	draw_hud_weapon(md, hud, winsz);
	if (!md->prm.ent_mode)
		return ;
	if (md->plr.was_hit)
		draw_sphere(md->screen, \
			get_v2(0, 0), \
			get_v2(md->win_size.x, md->win_size.y * 2), \
			get_v3(md->rgb[RGB_RED], md->plr.was_hit-- / 2, 1));
	draw_game_info(md, hud, winsz);
	if (md->timer.time % 50 == 0)
		md->hud.hp--;
}
