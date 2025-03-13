/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 23:46:39 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/13 20:09:19 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"


void	render_cursor(t_md *md, int has_hov)
{
	if (md->mouse_pressed && has_hov)
		mlx_put_image_to_window(md->mlx, md->win, \
md->curs_grb, md->mouse_pos.x, md->mouse_pos.y);
	else if (has_hov)
		mlx_put_image_to_window(md->mlx, md->win, \
md->curs_dtc, md->mouse_pos.x, md->mouse_pos.y);
	else
		mlx_put_image_to_window(md->mlx, md->win, \
md->cursor, md->mouse_pos.x, md->mouse_pos.y);
}

void	render_init_img(t_md *md, void *img, t_vec2 size, char *txt)
{
	md->mlx_put(md->mlx, md->win, md->screen->sky->img, 0, 0);
	md->mlx_put(md->mlx, md->win, img, \
		md->win_size.x / 2 - size.x / 2, md->win_size.y - size.y / 2);
	rnd_abs_txt(md, \
		get_v4(\
			md->win_size.x / 2 - size.x / 2, \
			md->win_size.y / 2 + size.y + 10, \
			-1, md->txt_scale), \
			txt);
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
	if (md->txtr_2d[e->type])
		md->mlx_put(md->mlx, md->win, md->txtr_2d[e->type], ent_p.x, ent_p.y);
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
	md->mlx_put(md->mlx, md->win, mp->bgrnd, centr.x, centr.y);
	plr_p.x = centr.x + (md->plr.pos.x / md->t_len) * mp->ic_scl;
	plr_p.y = centr.y + (md->plr.pos.y / md->t_len) * mp->ic_scl;
	md->mlx_put(md->mlx, md->win, mp->plr_icon, plr_p.x, plr_p.y);
	while (mp->mray_len > 0)
	{
		ray_p = mp->ray_pos[mp->mray_len - 1];
		mlx_pixel_put(md->mlx, md->win, md->win_size.x - mp->size.x + \
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


	offs_spd = get_v3f(3.0, 3.0, 0);
	if (!md->ray_mode)
	{
		draw_img(md->screen->floor, md->screen->buffer, 0, 0);
		return ;
	}
	draw_img(md->screen->floor, md->screen->sky, 0, 0);
	offs.x = fmod((((md->plr.rot.x + 180.0)) / 360.0) * md->win_size.x * offs_spd.x, md->win_size.x);
	offs.y = fmod((((md->plr.rot.y + 90.0)) / 180.0) * md->win_size.y * offs_spd.y, md->win_size.y * 2);\
	draw_img(md->screen->floor, md->screen->sky, -offs.x, -offs.y);
	draw_img(md->screen->floor, md->screen->sky, md->win_size.x - offs.x, -offs.y);
	draw_img(md->screen->floor, md->screen->sky, -offs.x, md->win_size.y - offs.y);
	draw_img(md->screen->floor, md->screen->sky, md->win_size.x - offs.x, md->win_size.y - offs.y);
	if (md->plr.rot.y + md->plr.pos.z <= -45)
		return ;
	offs_spd = get_v3f(5.0, 5.0, 0);
	pitch_offs = compute_perspective_change(md, NULL, 99999);
	offs.x = fmod((((md->plr.rot.x + 180.0)) / 360.0) * md->win_size.x * offs_spd.x, md->win_size.x);
	offs.y = fmod(pitch_offs - md->plr.pos.z, md->win_size.y);
	draw_img(md->screen->floor, md->screen->floor, -offs.x, offs.y + md->win_size.y);
	draw_img(md->screen->floor, md->screen->floor, md->win_size.x - offs.x, offs.y);
	draw_img(md->screen->floor, md->screen->floor, -offs.x, offs.y);
	draw_img(md->screen->floor, md->screen->floor, md->win_size.x - offs.x, offs.y + md->win_size.y);
}

void	render(t_md *md)
{
	flush_img(md->screen->buffer);
	render_background(md);
	render_rays(md, get_v3f(\
		md->plr.pos.x + md->plr.size.x / 2, \
		md->plr.pos.y + md->plr.size.y / 2, \
		md->plr.pos.z));
	if (md->mmap.active)
		render_minimap(md, &md->mmap);
	if (!md->ray_mode)
		render_entities(md);
	md->mlx_put(md->mlx, md->win, md->center, \
	md->win_size.x / 2 - CROSS_SCALE / 2, md->win_size.y / 2 - CROSS_SCALE / 2);
	render_cursor(md, 0);
	show_update_information(md);
}
