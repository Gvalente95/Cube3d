/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 23:46:39 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 13:53:18 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	render_cursor(t_md *md, t_image *screen, int has_hov)
{
	t_vec2	pos;

	pos = get_v2(md->mouse_real.x, md->mouse_real.y);
	if (md->mouse_pressed == MOUSE_PRESS && has_hov)
		draw_img(md->curs_grb, screen, pos, -1);
	else if (has_hov)
		draw_img(md->curs_dtc, screen, pos, -1);
	else
		draw_img(md->cursor, screen, pos, -1);
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
	draw_img(md->txtr_2d[e->type], md->screen, ent_p, -1);
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

void	render_hud_elements(t_md *md)
{
	t_vec2	center_cross;
	t_vec2	center_gun;
	t_image	*gun_image;

	center_cross = v2_center(md->win_size, md->center->size);
	draw_img(md->center, md->screen, center_cross, -1);
	gun_image = md->hud.gun[md->plr.can_shoot];
	center_gun = center_pos(\
		get_v2(md->win_size.x / 2, \
		(md->win_size.y / 2) * 2 + gun_image->size.y / 2), \
		gun_image->size, get_v2(0, 0), \
		md->win_size);
	draw_img(gun_image, md->screen, center_gun, -1);
}

void	render(t_md *md)
{
	flush_img(md->screen, md->hud.bgr_color, -1, 0);
	cast_rays(md, get_v3f(\
		md->plr.pos.x + md->plr.size.x / 2, \
		md->plr.pos.y + md->plr.size.y / 2, \
		md->plr.pos.z));
	render_background(md);
	if (md->mmap.active)
		render_minimap(md, &md->mmap);
	if (!md->ray_mode)
		render_entities(md);
	render_hud_elements(md);
	show_update_information(md);
	show_fps(md, get_v2(0, md->win_size.y - (md->txt_scale * 1.5) * 3));
	if (md->menu.hue.r != 1 || md->menu.hue.g != 1 || md->menu.hue.b != 1)
		set_hue(md->screen, md->menu.hue);
	mlx_put_image_to_window(md->mlx, md->win, md->screen->img, 0, 0);
}
