/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:59:55 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/18 12:56:53 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	render_debug_ray(t_md *md, t_vec2 start_pos, t_vec3f dir, int color)
{
	t_vec2	pos;
	int		i;

	pos = start_pos;
	i = -1;
	while (++i < 50)
	{
		draw_pixel(md->mmap.img, pos, color, -1);
		pos.x += (int)dir.x;
		pos.y += (int)dir.y;
	}
}

void	render_minimap_ray(t_md *md, t_mmap *mp, t_vec2 center)
{
	t_vec3f	ray_p;
	t_vec2	draw_pos;

	while (mp->mray_len > 0)
	{
		ray_p = mp->ray_pos[mp->mray_len - 1];
		draw_pos = get_v2(center.x + \
			((ray_p.x / md->t_len) * mp->ic_scl), \
			(ray_p.y / md->t_len) * mp->ic_scl);
		draw_pixel(mp->img, draw_pos, md->rgb[1], -1);
		ray_p = get_v3f(0, 0, 0);
		mp->mray_len--;
	}
}

void	render_minimap_entities(t_md *md, t_mmap *mp, t_vec2 center)
{
	int		i;
	int		draw_color;
	t_vec3f	pos;
	t_ent	*e;

	i = -1;
	while (md->map.buffer[++i])
	{
		e = md->mapped_ents[i];
		if (!e || e->type == nt_wall)
			continue ;
		draw_color = md->rgb[e->type];
		pos.x = center.x + (e->pos.x / md->t_len) * mp->ic_scl;
		pos.y = center.y + (e->pos.y / md->t_len) * mp->ic_scl;
		draw_pixels(\
			mp->img, \
			get_v2(pos.x + 1, pos.y + 1), \
			get_v2(mp->ic_scl - 1, mp->ic_scl - 1), \
			draw_color);
		if (e->dir.x || e->dir.y)
			render_debug_ray(md, \
				get_v2(pos.x + mp->ic_scl / 2, pos.y + mp->ic_scl / 2), e->dir, \
				draw_color + 10000);
	}
}

void	render_minimap(t_md *md, t_mmap *mp)
{
	t_vec2	centr;

	mp->limits_x = get_v2(\
		minf(md->plr.pos.x, mp->limits_x.x), \
		maxf(md->plr.pos.x, mp->limits_x.y));
	mp->limits_y = get_v2(\
		minf(md->plr.pos.y, mp->limits_y.x), \
		maxf(md->plr.pos.y, mp->limits_y.y));
	centr = get_v2(0, 0);
	flush_img(mp->img, -1, -1, -1);
	draw_img(mp->bg, mp->img, centr, -1);
	render_minimap_entities(md, mp, centr);
	render_minimap_ray(md, mp, centr);
	centr = get_v2(md->win_size.x - mp->size.x, 0);
	apply_scanlines(mp->img, .5);
	draw_transp_img(mp->img, md->screen, centr, 0.1);
}
