/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ceiling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:56:10 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/19 23:37:53 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	draw_ceiling_pxl(t_md *md, t_floor_draw_d d)
{
	const int	fog_clr = md->hud.fog_color;

	d.fogalpha = -1;
	if (md->fx.fog > 0)
		d.fogalpha = minmaxf(0, 1, (d.rwd / 10.0f) * md->fx.fog);
	if (d.fogalpha >= 1)
		return (draw_pixel(md->screen, d.win, fog_clr, d.fogalpha), 1);
	draw_pixel(md->screen, d.win, d.clr, 1);
	if (d.fogalpha > 0)
		draw_pixel(md->screen, d.win, fog_clr, d.fogalpha);
	return (1);
}

static int	set_ceiling_pxl(t_md *md, t_floor_draw_d *d)
{
	const t_image	*img = md->hud.ceiling;
	const t_vec2	img_sz = img->size;
	t_vec2f			flr_t;

	if (d->win.x < 0 || d->win.x >= md->win_sz.x)
		return (0);
	flr_t.x = d->flr.x + d->rwd * d->dirl.x + d->stp.x * d->win.x;
	flr_t.y = d->flr.y + d->rwd * d->dirl.y + d->stp.y * d->win.x;
	d->txp.x = ((int)(flr_t.x * img_sz.x) % img_sz.x + img_sz.x) % img_sz.x;
	d->txp.y = ((int)(flr_t.y * img_sz.y) % img_sz.y + img_sz.y) % img_sz.y;
	d->clr = img->src[d->txp.y * (img->size_line / 4) + d->txp.x];
	return (draw_ceiling_pxl(md, *d));
}

void	draw_ceiling(t_md *md, t_floor_draw_d d)
{
	const t_vec2	winsz = md->win_sz;

	d.win = (t_vec2){d.ray->index, d.ray->wall_strip_pos.x};
	while (d.win.y-- > 0)
	{
		d.p = (winsz.y / 2 - md->cam.rot.y * 8) - d.win.y;
		if (d.p == 0)
			d.p = 1;
		d.rwd = ((0.5 + (md->cam.pos.z / md->t_len)) * winsz.y) / d.p;
		d.stp.x = d.rwd * (d.dirr.x - d.dirl.x) / winsz.x;
		d.stp.y = d.rwd * (d.dirr.y - d.dirl.y) / winsz.x;
		if (!set_ceiling_pxl(md, &d))
			break ;
	}
}
