/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_floor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 12:48:34 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/06 12:28:48 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	draw_flr_px(t_md *md, t_floor_draw_d *d, t_vec2 t_crd, int has_flr)
{
	const int		fog_clr = md->hud.fog_color;
	const t_vec2	clr_d = (t_vec2){d->clr, md->hud.bgr_color};

	d->fogalpha = -1;
	if (md->fx.fog > 0)
		d->fogalpha = minmaxf(0, 1, (d->rwd / 10.0f) * md->fx.fog);
	if (d->fogalpha >= 1)
		return (draw_pixel(md->screen, d->win, fog_clr, d->fogalpha), 1);
	if (has_flr)
		draw_pixel(md->screen, d->win, clr_d.x, 1);
	if (md->prm.use_grass && d->has_grass)
		render_world_map(md, *d, 15);
	if (md->prm.use_grass)
		update_and_render_fe(md, d, t_crd);
	if (d->fogalpha > 0)
		draw_pixel(md->screen, d->win, fog_clr, d->fogalpha);
	return (1);
}

int	set_floor_pxl(t_md *md, t_floor_draw_d *d)
{
	const t_image	*img = md->hud.floor;
	const t_vec2	sz = img->size;
	t_ent			*map_e;
	t_vec2f			plr_crd;
	t_vec2			t_crd;

	t_crd = _v2(-1);
	if (md->prm.use_grass)
	{
		plr_crd = div_v2f((t_vec2f){md->plr.pos.x, md->plr.pos.y}, md->t_len);
		t_crd.x = (int)(plr_crd.x + d->rwd * d->dirl.x + d->stp.x * d->win.x);
		t_crd.y = (int)(plr_crd.y + d->rwd * d->dirl.y + d->stp.y * d->win.x);
		map_e = get_mapped_at_cord(md, t_crd);
		d->has_grass = (map_e && map_e->type == nt_grass);
	}
	if (d->win.y > md->win_sz.y && md->prm.use_grass && d->has_grass)
		return (draw_flr_px(md, d, t_crd, 0), 1);
	d->flr_t.x = d->flr.x + d->rwd * d->dirl.x + d->stp.x * d->win.x;
	d->flr_t.y = d->flr.y + d->rwd * d->dirl.y + d->stp.y * d->win.x;
	d->txp.x = ((int)(d->flr_t.x * sz.x) % sz.x + sz.x) % sz.x;
	d->txp.y = ((int)(d->flr_t.y * sz.y) % sz.y + sz.y) % sz.y;
	d->clr = img->src[d->txp.y * (img->size_line / 4) + d->txp.x];
	return (draw_flr_px(md, d, t_crd, 1), 1);
}

void	draw_floor(t_md *md, t_floor_draw_d d)
{
	const t_vec2	winsz = md->win_sz;
	float			pitch_scale;
	int				y_end;

	d.win.y = d.ray->wall_strip_pos.y + md->prm.height - 1;
	pitch_scale = 8.0f / fmaxf(1.0f, -md->cam.pos.z / md->t_len);
	d.win.y--;
	y_end = winsz.y;
	if (md->prm.use_grass)
		y_end += md->prm.grass_sz.y * 2;
	while (d.win.y++ < y_end)
	{
		d.p = d.win.y - (winsz.y / 2 - (md->cam.rot.y * pitch_scale));
		if (d.p <= 0)
			d.p = 1;
		d.rwd = ((0.5 - (md->cam.pos.z / md->t_len)) * winsz.y) / d.p;
		d.stp.x = d.rwd * (d.dirr.x - d.dirl.x) / winsz.x;
		d.stp.y = d.rwd * (d.dirr.y - d.dirl.y) / winsz.x;
		if (!set_floor_pxl(md, &d))
			break ;
	}
}

void	init_floor_data(t_md *md, t_ray *ray, t_floor_draw_d *d)
{
	t_vec2f			pn;
	const float		cam_scale = md->win_sz.x / (float)md->win_sz.y;

	pn.x = -(md->plr.dir.y * .8) * cam_scale * md->prm.floor_fov;
	pn.y = md->plr.dir.x * cam_scale * md->prm.floor_fov;
	d->ray = ray;
	d->plr = md->plr.pos;
	d->dirl = (t_vec2f){md->plr.dir.x - pn.x, md->plr.dir.y - pn.y};
	d->dirr = (t_vec2f){md->plr.dir.x + pn.x, md->plr.dir.y + pn.y};
	d->rwd = 0;
	d->stp = v2f(0);
	d->flr.x = (md->plr.pos.x / md->t_len);
	d->flr.y = (md->plr.pos.y / md->t_len);
	d->txp = _v2(0);
	d->clr = -1;
	d->has_grass = 0;
	d->win.x = ray->index;
	d->p = 0;
	d->fogalpha = 0;
}

void	draw_raycast_background(t_md *md, t_ray *ray)
{
	t_floor_draw_d	d;
	t_vec4			bgr_v4;

	if (!md->prm.show_walls)
	{
		bgr_v4 = color_to_v4(_WHITE);
		bgr_v4.r = minmax(0, 255, bgr_v4.r - ray->distance * .2);
		bgr_v4.g = minmax(0, 255, bgr_v4.g - ray->distance * .2);
		bgr_v4.b = minmax(0, 255, bgr_v4.b - ray->distance * .2);
		draw_pixels(md->screen, v2(ray->index, 0), v2(1, md->win_sz.y), \
			v4_to_color(bgr_v4.r, bgr_v4.g, bgr_v4.b, 255));
	}
	if (md->prm.use_ceiling || md->prm.use_floor || md->prm.show_sky)
		init_floor_data(md, ray, &d);
	if (md->prm.use_ceiling || md->prm.show_sky)
		draw_ceiling(md, d);
	else if (!md->prm.show_sky)
		draw_pixels(md->screen, v2(ray->index, 0), \
			v2(1, ray->wall_strip_pos.x), md->hud.sky_color);
	if (md->prm.use_floor && ray->wall_strip_pos.y < md->win_sz.y)
		draw_floor(md, d);
	else
		draw_pixels(md->screen, v2(ray->index, ray->wall_strip_pos.y), \
			v2(1, md->win_sz.y), md->hud.floor_color);
}
