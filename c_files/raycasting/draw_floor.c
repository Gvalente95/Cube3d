/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_floor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 12:48:34 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/19 11:46:50 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	draw_floor_px(t_md *md, t_floor_draw_d d, \
	t_fe **prv_fe, int has_flr)
{
	const int		fog_clr = md->hud.fog_color;
	const t_vec2	clr_d = (t_vec2){d.clr, md->hud.bgr_color};

	d.fogalpha = -1;
	if (md->fx.fog > 0)
		d.fogalpha = minmaxf(0, 1, (d.rwd / 10.0f) * md->fx.fog);
	if (d.fogalpha >= 1)
		return (draw_pixel(md->screen, d.win, fog_clr, d.fogalpha), 1);
	if (has_flr)
		draw_pixel(md->screen, d.win, clr_d.x, 1);
	if (md->prm.use_grass)
		update_and_render_fe(md, d, prv_fe);
	if (d.fogalpha > 0)
		draw_pixel(md->screen, d.win, fog_clr, d.fogalpha);
	return (1);
}

int	set_floor_pxl(t_md *md, t_floor_draw_d *d, t_fe **prv_fe)
{
	const t_image	*img = md->hud.floor;
	const t_vec2	img_sz = md->hud.floor->size;
	t_vec2f			flr_tile;

	if (d->win.x < 0 || d->win.x >= md->win_sz.x)
		return (0);
	flr_tile.x = d->flr.x + d->rwd * d->dirl.x + d->stp.x * d->win.x;
	flr_tile.y = d->flr.y + d->rwd * d->dirl.y + d->stp.y * d->win.x;
	if (d->win.y > md->win_sz.y && md->prm.use_grass)
		return (draw_floor_px(md, *d, prv_fe, 0), 1);
	d->txp.x = ((int)(flr_tile.x * img_sz.x) % img_sz.x + img_sz.x) % img_sz.x;
	d->txp.y = ((int)(flr_tile.y * img_sz.y) % img_sz.y + img_sz.y) % img_sz.y;
	d->clr = img->src[d->txp.y * (img->size_line / 4) + d->txp.x];
	return (draw_floor_px(md, *d, prv_fe, 1), 1);
}

void	init_floor_data(t_md *md, t_ray *ray, t_floor_draw_d *d)
{
	t_vec2f			pn;

	pn.x = -md->plr.dir.y * (.66 + md->prm.floor_fov);
	pn.y = md->plr.dir.x * (.66 + md->prm.floor_fov);
	d->ray = ray;
	d->win = (t_vec2){ray->index, md->hud.floor_start};
	d->plr = md->plr.pos;
	d->dirl = (t_vec2f){md->plr.dir.x - pn.x, md->plr.dir.y - pn.y};
	d->dirr = (t_vec2f){md->plr.dir.x + pn.x, md->plr.dir.y + pn.y};
	d->rwd = 0;
	d->stp = v2f(0);
	d->flr.x = (md->plr.pos.x / md->t_len);
	d->flr.y = (md->plr.pos.y / md->t_len);
	d->txp = v2(0);
	d->clr = -1;
	d->p = 0;
	d->fogalpha = 0;
}

void	draw_floor(t_md *md, t_floor_draw_d d)
{
	const t_vec2	winsz = md->win_sz;
	t_fe			*prv_fe;

	prv_fe = NULL;
	while (d.win.y++ < winsz.y + md->t_len)
	{
		d.p = d.win.y - (winsz.y / 2 - md->cam.rot.y * 8);
		if (d.p == 0)
			d.p = 1;
		d.rwd = ((0.5 - (md->cam.pos.z / md->t_len)) * winsz.y) / d.p;
		d.stp.x = d.rwd * (d.dirr.x - d.dirl.x) / winsz.x;
		d.stp.y = d.rwd * (d.dirr.y - d.dirl.y) / winsz.x;
		if (!set_floor_pxl(md, &d, &prv_fe))
			break ;
	}
}

void	draw_raycast_background(t_md *md, t_ray *ray)
{
	t_floor_draw_d	d;

	if (md->prm.use_ceiling || md->prm.use_floor)
	{
		init_floor_data(md, ray, &d);
		md->env.stored_blades = NULL;
	}
	if (md->prm.use_ceiling)
		draw_ceiling(md, d);
	else
		draw_pixels(md->screen, get_v2(ray->index, 0), \
			get_v2(1, md->hud.floor_start + 1), md->hud.sky_color);
	if (md->prm.use_floor)
		draw_floor(md, d);
	else
		draw_pixels(md->screen, get_v2(ray->index, md->hud.floor_start - 1), \
			get_v2(1, md->win_sz.y), md->hud.floor_color);
}
