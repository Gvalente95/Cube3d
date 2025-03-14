/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 23:46:39 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/14 03:16:33 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"


void	render_cursor(t_md *md, int has_hov)
{
	t_vec2	pos;

	pos = get_v2(md->mouse_pos.x, md->mouse_pos.y);
	if (md->mouse_pressed && has_hov)
		draw_img(md->curs_grb, md->screen.buffer, pos, -1);
	else if (has_hov)
		draw_img(md->curs_dtc, md->screen.buffer, pos, -1);
	else
		draw_img(md->cursor, md->screen.buffer, pos, -1);
}

void	render_2d_ent(t_md *md, t_ent *e)
{
	t_vec2	ent_p;
	t_vec2	centr;

	centr = get_v2((md->win_size.x / 4 + md->size_2d / 2 - \
		(md->cam_ofst.x / md->t_len * md->size_2d)), \
		md->win_size.y / 4 + md->size_2d / 2 - \
		((md->cam_ofst.y / md->t_len) * md->size_2d));
	ent_p.x = centr.x + (e->pos.x / md->t_len) * md->size_2d;
	ent_p.y = centr.y + (e->pos.y / md->t_len) * md->size_2d;
	draw_img(md->txtr_2d[e->type], md->screen.buffer, ent_p, -1);
}

void	render_entities(t_md *md)
{
	t_dblst	*node;
	t_ent	*e;

	node = md->entities;
	while (node)
	{
		e = (t_ent *)node->content;
		if (e->is_active)
			render_2d_ent(md, e);
		node = node->next;
	}
	render_2d_ent(md, &md->plr);
}

void	render_minimap(t_md *md, t_mmap *mp)
{
	t_vec2	plr_p;
	t_vec3f	ray_p;
	t_vec2	centr;

	centr = get_v2(md->win_size.x - mp->size.x, 0);
	draw_img(mp->bgrnd, md->screen.buffer, centr, -1);
	plr_p.x = centr.x + (md->plr.pos.x / md->t_len) * mp->ic_scl;
	plr_p.y = centr.y + (md->plr.pos.y / md->t_len) * mp->ic_scl;
	draw_img(mp->plr_icon, md->screen.buffer, plr_p, -1);
	while (mp->mray_len > 0)
	{
		ray_p = mp->ray_pos[mp->mray_len - 1];
		draw_pixel(md->screen.buffer, md->win_size.x - mp->size.x + \
	((ray_p.x / md->t_len) * mp->ic_scl), \
	(ray_p.y / md->t_len) * mp->ic_scl, md->rgb[1]);
		ray_p = get_v3f(0, 0, 0);
		mp->mray_len--;
	}
}

void	render_background(t_md *md)
{
	int		pitch_offs;
	t_vec2	offs;
	t_vec3f	offs_spd;
	t_image	*sky;
	t_image	*floor;
	t_image	*buff;

	sky = md->screen.sky;
	floor = md->screen.floor;
	buff = md->screen.buffer;
	offs_spd = get_v3f(3.0, 3.0, 0);
	if (!md->ray_mode)
	{
		draw_img(floor, buff, get_v2(0, 0), 0);
		return ;
	}
	offs_spd = get_v3f(5.0, 5.0, 0);
	pitch_offs = compute_perspective_change(md, NULL, 99999);
	offs.x = fmod((((md->plr.rot.x + 180.0)) / 360.0) * md->win_size.x * offs_spd.x, md->win_size.x);
	offs.y = fmod(pitch_offs - md->plr.pos.z, md->win_size.y);
	draw_img(floor, buff, get_v2(-offs.x, md->floor_start), md->rgb[2]);
	draw_img(floor, buff, get_v2(md->win_size.x, md->floor_start), md->rgb[2]);
	draw_img(floor, buff, get_v2(-offs.x, md->floor_start), md->rgb[2]);
	draw_img(floor, buff, get_v2(md->win_size.x - offs.x, md->floor_start), md->rgb[2]);
	offs.x = fmod((((md->plr.rot.x + 180.0)) / 360.0) * md->win_size.x * offs_spd.x, md->win_size.x);
	offs.y = fmod((((md->plr.rot.y + 90.0)) / 180.0) * md->win_size.y * offs_spd.y, md->win_size.y * 2);\
	draw_img(sky, buff, get_v2(-offs.x, -offs.y), md->rgb[2]);
	draw_img(sky, buff, get_v2(md->win_size.x - offs.x, -offs.y), md->rgb[2]);
	draw_img(sky, buff, get_v2(-offs.x, md->win_size.y - offs.y), md->rgb[2]);
	draw_img(sky, buff, get_v2(md->win_size.x - offs.x, md->win_size.y - offs.y), md->rgb[2]);
}

void	render(t_md *md)
{
	flush_img(md->screen.buffer, md->rgb[2]);
	render_rays(md, get_v3f(\
		md->plr.pos.x + md->plr.size.x / 2, \
		md->plr.pos.y + md->plr.size.y / 2, \
		md->plr.pos.z));
	render_background(md);
	if (md->mmap.active)
		render_minimap(md, &md->mmap);
	if (!md->ray_mode)
		render_entities(md);
	draw_img(md->center, md->screen.buffer, \
	get_v2(md->win_size.x / 2 - CROSS_SCALE / 2, md->win_size.y / 2 - CROSS_SCALE / 2), -1);
	render_cursor(md, 0);
	show_update_information(md);
	mlx_put_image_to_window(md->mlx, md->win, md->screen.buffer->img, 0, 0);
}
