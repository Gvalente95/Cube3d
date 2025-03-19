/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 23:46:39 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 05:30:35 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	render_cursor(t_md *md, t_image *screen, int has_hov)
{
	t_vec2	pos;
	t_image	*img;

	pos = get_v2(md->mouse_real.x, md->mouse_real.y);
	if (md->mouse_pressed == MOUSE_PRESS && has_hov)
		img = md->curs_grb;
	else if (has_hov)
		img = md->curs_dtc;
	else
		img = md->cursor;
	if (screen)
		draw_img(img, screen, pos, -1);
	else
		mlx_put_image_to_window(md->mlx, md->win, img->img, pos.x, pos.y);
}

void	render_2d_ent(t_md *md, t_ent *e, t_vec2 centr)
{
	t_vec2	ent_p;

	ent_p.x = centr.x + (e->pos.x / md->t_len) * md->size_2d;
	ent_p.y = centr.y + (e->pos.y / md->t_len) * md->size_2d;
	if (e->type == nt_mob)
		draw_img(md->mobs_txtrs_mini[e->mob_type][e->action][e->frame_index], \
			md->screen, ent_p, -1);
	else if (e->type == nt_pickup)
		draw_img(e->frame, md->screen, ent_p, -1);
	else if (e->type == nt_wall)
		draw_img(md->wall_img2d[0], md->screen, ent_p, -1);
	else if (e->type == nt_plr)
		draw_img(md->mobs_txtrs_mini[0][0][0], md->screen, ent_p, -1);
}

void	render_entities(t_md *md)
{
	t_dblst	*node;
	t_ent	*e;
	t_vec2	centr;

	centr = get_v2((md->win_size.x / 4 + md->size_2d / 2 - \
		(md->cam_ofst.x / md->t_len * md->size_2d)), \
		md->win_size.y / 4 + md->size_2d / 2 - \
		((md->cam_ofst.y / md->t_len) * md->size_2d));
	node = md->entities;
	while (node)
	{
		e = (t_ent *)node->content;
		if (e->is_active)
			render_2d_ent(md, e, centr);
		node = node->next;
	}
	render_2d_ent(md, &md->plr, centr);
}

void	render_hud_elements(t_md *md, t_hud *hud)
{
	t_vec2	center_gun;
	t_vec2	cross_pos;
	t_image	*gun_image;
	t_vec2	lock_pos;

	cross_pos = (t_vec2){md->win_size.x / 2, md->win_size.y / 2};
	draw_img(md->center, md->screen, cross_pos, -1);
	if (!md->wpn_txtr[hud->wpn_index][hud->weapon_frame])
		hud->weapon_frame = 0;
	gun_image = md->wpn_txtr[hud->wpn_index][hud->weapon_frame];
	center_gun = get_v2(md->win_size.x / 2 - gun_image->size.x / 2 + 30, \
		md->win_size.y - gun_image->size.y);
	draw_img(gun_image, md->screen, center_gun, -1);
	if (!md->lock_rotation.x && !md->lock_rotation.y)
		return ;
	lock_pos = get_v2(md->win_size.x - hud->lock_x_icon->size.x, \
		md->win_size.y - hud->lock_y_icon->size.y);
	if (md->lock_rotation.x)
		draw_img(hud->lock_x_icon, md->screen, lock_pos, -1);
	lock_pos.y -= hud->lock_x_icon->size.y;
	if (md->lock_rotation.y)
		draw_img(hud->lock_y_icon, md->screen, lock_pos, -1);
}

void	apply_fx(t_md *md, t_image *screen)
{
	if (md->scanlines != 1)
		apply_scanlines(screen, md->scanlines);
	if (md->menu.hue.r != 1 || md->menu.hue.g != 1 || md->menu.hue.b != 1)
		set_hue(screen, md->menu.hue);
	if (md->dithering)
		apply_dithering(screen, md->dithering);
	if (md->rgb_distortion)
		apply_rgb_glitch(screen, md->rgb_distortion);
	if (md->anti_aliasing)
		apply_antialiasing(screen);
}

void	render(t_md *md)
{
	flush_img(md->screen, md->hud.bgr_color, -1, 0);
	cast_rays(md, get_v3f(\
		md->plr.pos.x + md->plr.size.x / 2, \
		md->plr.pos.y + md->plr.size.y / 2, \
		md->plr.pos.z));
	render_background(md);
	if (!md->real_mode)
		render_entities(md);
	else
		render_hud_elements(md, &md->hud);
	if (md->mmap.active)
		render_minimap(md, &md->mmap);
	show_update_information(md);
	show_fps(md, get_v2(0, md->win_size.y - (md->txt_scale * 1.5)));
	apply_fx(md, md->screen);
	mlx_put_image_to_window(md->mlx, md->win, md->screen->img, 0, 0);
}
