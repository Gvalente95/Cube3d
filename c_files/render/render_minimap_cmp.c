/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap_cmp.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 09:54:23 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/02 14:23:33 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	show_mmap_ent(t_md *md, t_ent *e, t_vec3f cosin, t_vec2 center)
{
	int		draw_color;
	t_vec3f	relp;
	t_vec3f	rotp;
	t_vec3f	pos;
	int		dist;

	dist = hypotf(md->plr.pos.x - e->pos.x, md->plr.pos.y - e->pos.y);
	if (dist > cosin.z * md->t_len)
		return ;
	draw_color = (md->rgb[e->type] & 0x00FFFFFF) | \
	((int)((minmaxf(0.0f, 1.0f, dist / (cosin.z * md->t_len)) * 255)) << 24);
	if (e == md->portal.ends[0].e)
		draw_color = md->rgb[RGB_VIOLET];
	else if (e == md->portal.ends[1].e)
		draw_color = md->rgb[RGB_ORANGE];
	relp.x = e->pos.x - md->plr.pos.x;
	relp.y = e->pos.y - md->plr.pos.y;
	rotp.x = (relp.x * cosin.x - relp.y * cosin.y - md->cam_ofst.x / md->t_len);
	rotp.y = (relp.x * cosin.y + relp.y * cosin.x - md->cam_ofst.y / md->t_len);
	pos.x = center.x + ((rotp.x / md->t_len) * md->mmap.collaps_scl);
	pos.y = center.y + ((rotp.y / md->t_len) * md->mmap.collaps_scl);
	draw_pixels(md->screen,
		get_v2(pos.x + 1 + cosin.z, pos.y + 1 + cosin.z), \
		get_v2(md->mmap.collaps_scl - 1, md->mmap.collaps_scl - 1), \
		draw_color);
}

static void	show_mmap_dir(t_md *md, t_vec3f cosin, t_vec2 psz, t_vec2 map_p)
{
	const t_vec2		relp[4] = {\
		(t_vec2){map_p.x + psz.x / 2, map_p.y}, \
		(t_vec2){map_p.x + psz.x / 2, map_p.y + psz.y}, \
		(t_vec2){map_p.x, map_p.y + psz.y / 2}, \
		(t_vec2){map_p.x + psz.x, map_p.y + psz.y / 2}};
	const char			*str[4] = {"N", "S", "W", "E"};
	t_vec3f				rot;
	const t_vec2		cnt = get_v2(map_p.x + psz.x / 2, map_p.y + psz.y / 2);
	int					i;

	i = -1;
	while (++i < 4)
	{
		rot.x = ((relp[i].x - cnt.x) * cosin.x - (relp[i].y - cnt.y) * cosin.y);
		rot.y = ((relp[i].x - cnt.x) * cosin.y + (relp[i].y - cnt.y) * cosin.x);
		rot.x += cnt.x;
		rot.y += cnt.y;
		rnd_abs_txt(md, get_v4(rot.x, rot.y, -1, md->prm.txt_sc), str[i]);
	}
}

void	show_cmps_mmap(t_md *md, t_vec2 center, int view_dist)
{
	int		i;
	t_vec3f	cosin;
	t_vec2	p;
	t_vec2	psz;

	p = get_v2(center.x - (view_dist - 1) * md->mmap.collaps_scl, \
		center.y - (view_dist - 1) * md->mmap.collaps_scl);
	psz = v2((view_dist * 2) * md->mmap.collaps_scl);
	draw_sphere(md->screen, p, psz, get_v3(md->rgb[RGB_INDIGO], 9, 1));
	cosin.z = md->plr.angle;
	cosin.x = md->plr.dir.x;
	cosin.y = md->plr.dir.y;
	show_mmap_dir(md, cosin, psz, p);
	cosin.z = view_dist;
	i = -1;
	while (md->map.buffer[++i])
		if (md->mapped_ents[i] && md->mapped_ents[i]->type != nt_plr)
			show_mmap_ent(md, md->mapped_ents[i], cosin, center);
	show_mmap_ent(md, md->mapped_ents[md->plr.map_index], cosin, center);
}
