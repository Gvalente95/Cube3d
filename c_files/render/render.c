/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 23:46:39 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/07 19:43:36 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	render_cursor(t_md *md, t_image *screen, int has_hov)
{
	t_vec2	pos;
	t_image	*img;

	pos = get_v2(md->mouse.real.x, md->mouse.real.y);
	if (md->mouse.pressed == MOUSE_PRESS && has_hov)
		img = md->mouse.curs_grb;
	else if (has_hov)
		img = md->mouse.curs_dtc;
	else
		img = md->mouse.cursor;
	if (screen)
		draw_img(img, screen, pos, -1);
	else
		mlx_put_image_to_window(md->mlx, md->win, img->img, pos.x, pos.y);
}

void	render_2d_ent(t_md *md, t_ent *e, t_vec2 centr)
{
	t_vec2			ent_p;
	t_texture_data	*td;
	t_image			*img;

	td = &md->txd;
	ent_p.x = centr.x + (e->pos.x / md->t_len) * td->size_2d;
	ent_p.y = centr.y + (e->pos.y / md->t_len) * td->size_2d;
	img = td->wall_img2d[0];
	if (e->type == nt_mob)
		img = td->mobs_txtrs_mini[e->mob_type][e->action][e->frame_index];
	else if (e->type == nt_pickup)
		img = td->pickup_txtr_mini[e->pckp_type][0];
	else if (e->type == nt_door)
		img = td->door_txtr_mini;
	else if (e->type == nt_plr)
	{
		ent_p.x = ent_p.x - td->e_sizes2d[nt_mob].x / 2;
		ent_p.y = ent_p.y - td->e_sizes2d[nt_mob].y / 2;
		img = td->mobs_txtrs_mini[1][md->plr.action][md->plr.frame_index];
	}
	draw_img(img, md->screen, ent_p, -1);
}

void	render_2d_entities(t_md *md)
{
	t_dblst	*node;
	t_ent	*e;
	t_vec2	centr;

	centr = get_v2((md->win_sz.x * .5 - md->txd.size_2d * 2 - \
		(md->cam.ofst.x / md->t_len * md->txd.size_2d)), \
		md->win_sz.y * .5 - md->txd.size_2d * 2 - \
		((md->cam.ofst.y / md->t_len) * md->txd.size_2d));
	node = md->entities;
	while (node)
	{
		e = (t_ent *)node->content;
		if (e->is_active && e->type != nt_empty)
			render_2d_ent(md, e, centr);
		node = node->next;
	}
	render_2d_ent(md, &md->plr, centr);
}

void	apply_fx(t_md *md, t_image *screen, t_fx_data *fx)
{
	if (fx->anti_alias)
		apply_antialiasing(screen);
	if (fx->scanlines > 0)
		apply_scanlines(screen, fx->scanlines);
	if (fx->hue.r != 1 || fx->hue.g != 1 || fx->hue.b != 1)
		set_hue(screen, fx->hue);
	if (fx->dithering > 0)
		apply_dithering(screen, fx->dithering, \
			fx->palette, fx->palette_size);
	if (fx->bloom_threshold > 0)
		apply_bloom(screen, fx->bloom_threshold);
	if (fx->barrel_amount > 0)
		apply_barrel_distortion(screen, fx->barrel_amount);
	if (fx->color_band > 0)
		apply_color_banding(screen, fx->color_band);
	if (fx->noise > 0)
		apply_noise(md, md->screen, fx->noise, 1);
	return ;
}

void	render(t_md *md)
{
	render_background(md);
	md->cam.pointed = NULL;
	if (md->prm.use_thrd)
		cast_ray_threads_lp(md);
	else
		cast_rays(md);
	if (md->prm.view_2d)
		render_2d_entities(md);
	else
		render_hud_elements(md, &md->hud);
	if (md->mmap.active)
		render_minimap(md, &md->mmap);
	if (md->prm.debug_mode)
		show_update_information(md);
	if (md->prm.show_fps)
		show_fps(md, get_v2(0, md->win_sz.y - (md->prm.txt_sc * 1.5)));
	apply_fx(md, md->screen, &md->fx);
	mlx_put_image_to_window(md->mlx, md->win, md->screen->img, 0, 0);
}
