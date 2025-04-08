/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:59:55 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/08 02:41:41 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	render_mmap_ray(t_md *md, int ray_index, int color)
{
	t_vec2			ray_start;
	t_vec2			ray_end;
	t_ray			*ray;
	const t_mmap	*mp = &md->mmap;

	ray = &md->rays[ray_index];
	ray_start.x = (ray->start.x / md->t_len) * mp->ic_scl;
	ray_start.y = (ray->start.y / md->t_len) * mp->ic_scl;
	ray_end.x = (ray->pos.x / md->t_len) * mp->ic_scl;
	ray_end.y = (ray->pos.y / md->t_len) * mp->ic_scl;
	draw_line(mp->img, ray_start, ray_end, get_v2(color, 1));
}

void	render_minimap_ray(t_md *md)
{
	int	i;
	int	color;

	color = _GREEN;
	color = set_alpha(color, 0.975);
	i = -1;
	while (++i < md->win_sz.x)
		render_mmap_ray(md, i, color);
	color = _BLUE;
	color = set_alpha(color, 0.65);
	render_mmap_ray(md, md->win_sz.x / 2, color);
}

void	show_minimap_entity(t_md *md, t_ent *e, t_image *screen, int no_redraw)
{
	int				draw_clr;
	t_vec2			pos;
	int				psz;
	int				offset;

	if (!e || (no_redraw && e->revealed))
		return ;
	draw_clr = md->rgb[e->type];
	if (e->type == nt_plr)
		draw_clr = md->rgb[RGB_GOLD];
	psz = max(1, md->mmap.ic_scl);
	offset = (md->mmap.ic_scl - psz) / 2;
	pos.x = 1 + (e->pos.x / (md->t_len)) * psz - offset;
	pos.y = 1 + (e->pos.y / (md->t_len)) * psz - offset;
	if (e->type == nt_empty)
	{
		psz++;
		pos = sub_vec2(pos, v2(1));
	}
	set_alpha(draw_clr, .5);
	draw_safe_pxls(screen, sub_vec2(pos, v2(1)), v2(psz + 1), md->rgb[16]);
	draw_pixels(screen, pos, v2(psz - 1), draw_clr);
	if (!e->revealed && e->type != nt_plr)
		md->mmap.revealed_cur++;
	e->revealed = 1;
}

void	render_minimap(t_md *md, t_mmap *mp)
{
	t_vec2			center;
	int				view_len;
	int				width;
	t_vec2			txp;
	const int		txsc = md->prm.txt_sc * .5;

	if (mp->cmps)
	{
		view_len = 10;
		width = mp->comps_scl * view_len;
		center = get_v2(md->win_sz.x - width * 2, md->win_sz.y - width * 2);
		show_cmps_mmap(md, center, view_len);
		return ;
	}
	flush_img(mp->img, _RED, 1, 0);
	remove_img_color(mp->img, _RED);
	draw_alpha_img(mp->bg, mp->img, v2(0), 1);
	render_minimap_ray(md);
	show_minimap_entity(md, &md->plr, mp->img, 0);
	center = get_v2(md->win_sz.x - mp->img->size.x, \
		md->win_sz.y - mp->img->size.y);
	txp = get_v2(center.x + mp->img->size.x / 2, mp->img->size.y);
	show_revealed_perc(md, txsc, txp);
	draw_alpha_img(mp->img, md->screen, center, 0.5);
}
