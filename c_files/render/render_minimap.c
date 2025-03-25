/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:59:55 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 13:39:36 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

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
		draw_pixel(mp->img, draw_pos, md->rgb[RGB_RED], -1);
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
		if (!e || (e->type == nt_wall && !e->overlay))
			continue ;
		draw_color = md->rgb[e->type];
		if (e == md->portal_gun.entrance)
			draw_color = md->rgb[RGB_ORANGE];
		else if (e == md->portal_gun.exit)
			draw_color = md->rgb[RGB_INDIGO];
		pos.x = center.x + (e->pos.x / md->t_len) * mp->ic_scl;
		pos.y = center.y + (e->pos.y / md->t_len) * mp->ic_scl;
		draw_pixels(\
			mp->img, \
			get_v2(pos.x + 1, pos.y + 1), \
			get_v2(mp->ic_scl - 1, mp->ic_scl - 1), \
			draw_color);
	}
	draw_contour(mp->img, md->rgb[RGB_BLACK], 1, .5);
}

void	show_comps_ent(t_md *md, t_ent *e, t_vec3f view, t_vec2 center)
{
	int		draw_color;
	t_vec3f	relp;
	t_vec3f	rot_p;
	t_vec3f	pos;
	int		dist;

	dist = hypotf(md->plr.pos.x - e->pos.x, md->plr.pos.y - e->pos.y);
	if (dist > view.z * md->t_len)
		return ;
	draw_color = (md->rgb[e->type] & 0x00FFFFFF) | \
	((int)((minmaxf(0.0f, 1.0f, dist / (view.z * md->t_len)) * 255)) << 24);
	if (e == md->portal_gun.entrance)
		draw_color = md->rgb[RGB_ORANGE];
	else if (e == md->portal_gun.exit)
		draw_color = md->rgb[RGB_MAGENT];
	relp.x = e->pos.x - md->plr.pos.x;
	relp.y = e->pos.y - md->plr.pos.y;
	rot_p.x = (relp.x * view.x - relp.y * view.y - md->cam_ofst.x / md->t_len);
	rot_p.y = (relp.x * view.y + relp.y * view.x - md->cam_ofst.y / md->t_len);
	pos.x = center.x + ((rot_p.x / md->t_len) * md->mmap.collaps_scl);
	pos.y = center.y + ((rot_p.y / md->t_len) * md->mmap.collaps_scl);
	draw_pixels(md->screen,
		get_v2(pos.x + 1 + view.z, pos.y + 1 + view.z), \
		get_v2(md->mmap.collaps_scl - 1, md->mmap.collaps_scl - 1), \
		draw_color);
}

void	show_collapsed_mmap(t_md *md, t_vec2 center, int view_dist)
{
	int		i;
	t_vec3f	cosin;
	t_vec2	p;
	t_vec2	psz;
	int		txtscl;

	txtscl = md->prm.txt_scale;
	p = get_v2(center.x - (view_dist - 1) * md->mmap.collaps_scl, \
		center.y - (view_dist - 1) * md->mmap.collaps_scl);
	psz = v2((view_dist * 2) * md->mmap.collaps_scl);
	draw_sphere(md->screen, p, psz, get_v3(md->rgb[RGB_INDIGO], 9, 1));
	cosin.z = (md->plr_rot.x + 90) * (M_PI / 180.0f);
	cosin.x = cosf(cosin.z);
	cosin.y = sinf(cosin.z);
	rnd_abs_txt(md, get_v4(p.x + psz.x / 2, p.y, -1, txtscl), "N");
	rnd_abs_txt(md, get_v4(p.x + psz.x / 2, p.y + psz.y, -1, txtscl), "S");
	rnd_abs_txt(md, get_v4(p.x, p.y + psz.y / 2, -1, txtscl), "E");
	rnd_abs_txt(md, get_v4(p.x + psz.x, p.y + psz.y / 2, -1, txtscl), "W");
	cosin.z = view_dist;
	i = -1;
	while (md->map.buffer[++i])
		if (md->mapped_ents[i])
			show_comps_ent(md, md->mapped_ents[i], cosin, center);
}

void	render_minimap(t_md *md, t_mmap *mp)
{
	t_vec2	center;
	int		view_len;
	int		collapsed_width;

	view_len = 10;
	collapsed_width = mp->ic_scl * view_len;
	center = get_v2(md->win_size.x - collapsed_width * 1.5, collapsed_width);
	show_collapsed_mmap(md, center, view_len);
	if (mp->collapsed)
		return ;
	flush_img(mp->img, 0x00FFFFFF, 1, 0);
	draw_transp_img(mp->bg, mp->img, v2(0), .75f);
	render_minimap_entities(md, mp, v2(0));
	render_minimap_ray(md, mp, v2(0));
	center.x = md->win_size.x / 2 - mp->size.x / 2;
	center.y = md->win_size.y / 2 - mp->size.y / 2;
	apply_noise(md, mp->img, .3, 1);
	apply_scanlines(mp->img, .1);
	draw_transp_img(mp->img, md->screen, center, 0.75);
}
