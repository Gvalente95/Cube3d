/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:59:55 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/02 14:19:30 by giuliovalen      ###   ########.fr       */
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

	color = md->rgb[RGB_GREEN];
	color = set_alpha(color, 0.975);
	i = -1;
	while (++i < md->win_sz.x)
		render_mmap_ray(md, i, color);
	color = md->rgb[RGB_BLUE];
	color = set_alpha(color, 0.65);
	render_mmap_ray(md, md->win_sz.x / 2, color);
}

void	show_minimap_entity(t_md *md, int scl, t_ent *e, t_vec2 cntr)
{
	const int		portal[2] = {md->rgb[RGB_INDIGO], md->rgb[RGB_ORANGE]};
	int				draw_clr;
	t_vec2			pos;
	int				psz;
	int				offset;

	if (!e || (e->type == nt_wall && !e->overlay))
		return ;
	draw_clr = md->rgb[e->type];
	if (e == md->portal.ends[0].e)
		draw_clr = portal[0];
	else if (e == md->portal.ends[1].e)
		draw_clr = portal[1];
	psz = max(1, md->mmap.ic_scl / scl);
	offset = (md->mmap.ic_scl - psz) / 2;
	pos.x = cntr.x + 1 + (e->pos.x / (md->t_len / scl)) * psz - offset;
	pos.y = cntr.y + 1 + (e->pos.y / (md->t_len / scl)) * psz - offset;
	draw_pixels(md->mmap.img, pos, v2((psz) - 1), draw_clr);
}

void	render_minimap_entities(t_md *md, t_mmap *mp, t_vec2 center)
{
	const int		portal[2] = {md->rgb[RGB_INDIGO], md->rgb[RGB_ORANGE]};
	int				i;
	t_vec2			pos;
	t_vec2			out;

	i = -1;
	while (md->map.buffer[++i])
		if (md->mapped_ents[i] && md->mapped_ents[i]->type != nt_plr)
			show_minimap_entity(md, 1, md->mapped_ents[i], center);
	render_minimap_ray(md);
	show_minimap_entity(md, 10, md->mapped_ents[md->plr.map_index], center);
	i = -1;
	while (++i < 2)
	{
		if (md->portal.ends[i].e == NULL)
			continue ;
		out = md->portal.ends[i].out;
		pos.x = center.x + 1 + (out.x / (md->t_len / 4)) * mp->ic_scl / 4;
		pos.y = center.y + 1 + (out.y / (md->t_len / 4)) * mp->ic_scl / 4;
		draw_pixels(mp->img, pos, v2(mp->ic_scl / 4), portal[i] + 1000);
	}
}

void	render_minimap(t_md *md, t_mmap *mp)
{
	t_vec2	center;
	int		view_len;
	int		cmps_width;

	view_len = 10;
	cmps_width = mp->collaps_scl * view_len;
	center = get_v2(md->win_sz.x - cmps_width * 1.5, cmps_width);
	show_cmps_mmap(md, center, view_len);
	if (mp->cmps)
		return ;
	flush_img(mp->img, md->rgb[RGB_RED], 1, 0);
	remove_img_color(mp->img, md->rgb[RGB_RED]);
	draw_alpha_img(mp->bg, mp->img, v2(0), .75f);
	render_minimap_entities(md, mp, v2(0));
	center.x = 0;
	center.y = 0;
	apply_scanlines(mp->img, .1);
	draw_alpha_img(mp->img, md->screen, center, 0.75);
}
