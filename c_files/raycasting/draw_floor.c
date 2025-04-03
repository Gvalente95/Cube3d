/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_floor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 12:48:34 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/03 15:57:33 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	find_and_store_fe(t_md *md, t_floor_draw_d d, t_fe **prv_fe)
{
	t_vec2				local;
	t_vec2f				rel;
	t_fe				*fe;
	const t_env_manager	*env = &md->env;

	if ((int)d.flr.x >= md->map.size.x - 1 || (int)d.flr.x < 0 || \
	(int)d.flr.y >= md->map.size.y - 1 || (int)d.flr.y < 0)
		return (1);
	rel.x = d.flr.x * (md->t_len / md->prm.grass_w);
	rel.y = d.flr.y * (md->t_len / md->prm.grass_w);
	local.x = minmax(0, safe_mod((int)rel.x, md->t_len), md->t_len - 1);
	local.y = minmax(0, safe_mod((int)rel.y, md->t_len), md->t_len - 1);
	fe = &env->grass[(int)d.flr.y][(int)d.flr.x][local.y][local.x];
	if (!fe->active)
		return (1);
	update_fe(md, d.win, fe, d);
	render_fe(md, fe, 1);
	return (*prv_fe = fe, 1);
}

static int	draw_floor_px(t_md *md, t_floor_draw_d d, \
	t_fe **prv_fe, int has_flr)
{
	const t_vec2	fog_clr_d = (t_vec2){md->rgb[RGB_BLACK], md->hud.bgr_color};
	const t_vec2	clr_d = (t_vec2){d.clr, md->hud.bgr_color};

	d.fogalpha = -1;
	if (md->fx.fog > 0)
		d.fogalpha = minmaxf(0, 1, (d.rwd / 10.0f) * md->fx.fog);
	if (d.fogalpha >= 1)
		return (draw_pixel(md->screen, d.win, fog_clr_d.x, d.fogalpha), 1);
	if (has_flr)
		draw_pixel(md->screen, d.win, clr_d.x, 1);
	if (md->prm.use_grass)
		find_and_store_fe(md, d, prv_fe);
	if (d.fogalpha > 0)
		draw_pixel(md->screen, d.win, fog_clr_d.x, d.fogalpha);
	return (1);
}

int	set_floor_pxl(t_md *md, t_floor_draw_d *d, t_fe **prv_fe)
{
	const t_image	*img = md->hud.floor;
	const t_vec2	img_sz = md->hud.floor->size;

	if (d->win.x < 0 || d->win.x >= md->win_sz.x)
		return (0);
	d->flr.x = (d->plr.x / md->t_len) * 1 + d->rwd * d->dirl.x + d->stp.x * d->win.x;
	d->flr.y = (d->plr.y / md->t_len) * .75 + d->rwd * d->dirl.y + d->stp.y * d->win.x;
	if (d->win.y > md->win_sz.y && md->prm.use_grass)
		return (draw_floor_px(md, *d, prv_fe, 0), 1);
	d->txp.x = ((int)(d->flr.x * img_sz.x) % img_sz.x + img_sz.x) % img_sz.x;
	d->txp.y = ((int)(d->flr.y * img_sz.y) % img_sz.y + img_sz.y) % img_sz.y;
	d->clr = img->src[d->txp.y * (img->size_line / 4) + d->txp.x];
	return (draw_floor_px(md, *d, prv_fe, 1), 1);
}

void	init_floor_data(t_md *md, t_ray *ray, t_vec2f pn, t_floor_draw_d *d)
{
	*d = (t_floor_draw_d){ray, v2(0), md->plr.pos, \
		(t_vec2f){md->plr.dir.x - pn.x, md->plr.dir.y - pn.y}, \
		(t_vec2f){md->plr.dir.x + pn.x, md->plr.dir.y + pn.y}, \
		0, v2f(0), v2f(0), v2(0), -1, 0, 0
	};
	md->env.stored_blades = NULL;
}

void	draw_floor(t_md *md, t_ray *ray, int y_start, t_vec2f pn)
{
	t_floor_draw_d	d;
	const t_vec2	winsz = md->win_sz;
	t_fe			*prv_fe;

	prv_fe = NULL;
	init_floor_data(md, ray, pn, &d);
	d.win.x = ray->index;
	d.win.y = max(y_start, winsz.y / 2 - md->plr_rot.y * 8) - 1;
	while (++d.win.y < winsz.y + md->t_len)
	{
		d.p = d.win.y - (winsz.y / 2 - md->plr_rot.y * 8);
		if (d.p == 0)
			d.p = 1;
		d.rwd = ((0.5 - (md->cam_pos.z / md->t_len)) * winsz.y) / d.p;
		d.stp.x = d.rwd * (d.dirr.x - d.dirl.x) / winsz.x;
		d.stp.y = d.rwd * (d.dirr.y - d.dirl.y) / winsz.x;
		if (!set_floor_pxl(md, &d, &prv_fe))
			break ;
	}
}
