/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:59:55 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/17 12:12:01 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	show_revl_done(t_md *md, t_vec2 pos)
{
	float		revealed_perc_;
	int			color;
	t_txtd		data;

	revealed_perc_ = (100.0f / md->mmap.revealed_len) * md->mmap.revealed_cur;
	color = -1;
	data = (t_txtd){pos.x, pos.y, color, md->prm.txt_sc, NULL};
	rnd_fast_txt(md, data, "%.1f/", revealed_perc_);
}

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
	color = set_alpha(color, 1);
	render_mmap_ray(md, md->win_sz.x / 2, color);
}

void	show_minimap_entity(t_md *md, t_ent *e, t_image *screen, int no_redraw)
{
	int				draw_clr;
	t_vec2			pos;
	int				icsz;

	if (!e || (no_redraw && e->revealed))
		return ;
	draw_clr = md->rgb[e->type];
	icsz = md->mmap.ic_scl;
	pos = (t_vec2){1 + e->coord.x * icsz, 1 + e->coord.y * icsz};
	if (e->type == nt_plr)
	{
		pos.x = 1 + (e->pos.x / md->t_len) * icsz;
		pos.y = 1 + (e->pos.y / md->t_len) * icsz;
		icsz /= 2;
	}
	if (e->type == nt_empty)
	{
		icsz++;
		pos = sub_vec2(pos, v2(1));
	}
	draw_pixels(screen, sub_vec2(pos, v2(1)), v2(icsz + 1), _BLACK);
	draw_pixels(screen, pos, v2(icsz - 1), draw_clr);
	if (!e->revealed && e->type != nt_plr)
		md->mmap.revealed_cur++;
	e->revealed = 1;
}

void	render_minimap(t_md *md, t_mmap *mp)
{
	t_vec2			cntr;
	int				width;

	if (!mp->active)
	{
		rnd_fast_txt(md, (t_txtd){md->win_sz.x - 90, md->win_sz.y - 30, \
			_BLUE, md->prm.txt_sc, md->screen}, "[M]");
		return ;
	}
	if (mp->cmps)
	{
		width = mp->comps_scl * (8 + 6);
		cntr = get_v2(md->win_sz.x - width, md->win_sz.y - width);
		show_cmps_mmap(md, cntr, 8);
		return ;
	}
	flush_img(mp->img, _RED, 1, 0);
	remove_img_color(mp->img, _RED);
	draw_alpha_img(mp->bg, mp->img, v2(0), 1);
	render_minimap_ray(md);
	show_minimap_entity(md, &md->plr, mp->img, 0);
	cntr = get_v2(md->win_sz.x - mp->img->size.x, \
		0);
	draw_alpha_img(mp->img, md->screen, cntr, 0.5);
	show_revl_done(md, get_v2(cntr.x, mp->img->size.y - md->prm.txt_sc));
}
