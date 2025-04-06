/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap_cmp.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 09:54:23 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/04 12:00:42 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	show_revealed_perc(t_md *md, int scale, t_vec2 pos)
{
	float	revealed_perc_;
	int		color;
	t_vec4	data;

	revealed_perc_ = (100.0f / md->mmap.revealed_len) * md->mmap.revealed_cur;
	color = -1;
	data = get_v4(pos.x, pos.y, color, scale);
	rnd_abs_txt(md, data, "%.1f/100", revealed_perc_);
}

static void	show_mmap_ent(t_md *md, t_ent *e, t_vec3f cosin, t_vec2 center)
{
	int		draw_color;
	t_vec3f	relp;
	t_vec3f	rotp;
	t_vec3f	pos;
	int		dist;

	if (e->type == nt_empty)
		return ;
	dist = hypotf(md->plr.pos.x - e->pos.x, md->plr.pos.y - e->pos.y);
	if (dist > cosin.z * md->t_len)
		return ;
	draw_color = (md->rgb[e->type] & 0x00FFFFFF) | \
	((int)((minmaxf(0.0f, 1.0f, dist / (cosin.z * md->t_len)) * 255)) << 24);
	if (e == md->portal.ends[0].e)
		draw_color = _VIOLET;
	else if (e == md->portal.ends[1].e)
		draw_color = _ORANGE;
	relp = sub_vec3f(e->pos, md->plr.pos);
	rotp.x = (relp.x * cosin.x - relp.y * cosin.y - md->cam.ofst.x / md->t_len);
	rotp.y = (relp.x * cosin.y + relp.y * cosin.x - md->cam.ofst.y / md->t_len);
	pos.x = center.x + ((rotp.x / md->t_len) * md->mmap.comps_scl);
	pos.y = center.y + ((rotp.y / md->t_len) * md->mmap.comps_scl);
	draw_sphere(md->screen,
		get_v2(pos.x + 1 + cosin.z, pos.y + 1 + cosin.z), \
		v2(md->mmap.comps_scl - 1), get_v3(draw_color, 10, 1));
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
		rnd_fast_txt(md, get_v4(rot.x, rot.y, -1, md->prm.txt_sc), str[i]);
	}
}

void	show_cmps_mmap(t_md *md, t_vec2 center, int view_dist)
{
	int		i;
	t_vec3f	pdir;
	t_vec2	p;
	t_vec2	psz;

	p = get_v2(center.x - (view_dist - 1) * md->mmap.comps_scl, \
		center.y - (view_dist - 1) * md->mmap.comps_scl);
	psz = v2((view_dist * 2) * md->mmap.comps_scl);
	draw_sphere(md->screen, p, psz, get_v3(md->rgb[RGB_GRAY], 6, 1));
	pdir.z = md->plr.angle + M_PI_2;
	pdir.x = cosf(pdir.z);
	pdir.y = sinf(pdir.z);
	show_mmap_dir(md, pdir, psz, p);
	pdir.z = view_dist;
	i = -1;
	while (md->map.buffer[++i])
		if (md->mapped_ents[i] && md->mapped_ents[i]->type != nt_plr)
			show_mmap_ent(md, md->mapped_ents[i], pdir, center);
	show_mmap_ent(md, md->mapped_ents[md->plr.map_index], pdir, center);
}
