/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_inv_team.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:28:17 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/28 16:05:47 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	draw_team(t_md *md, t_txtd td, t_vec2 spacing, t_vec2 base)
{
	const t_vec2		slot_size = v2(md->inv.sz.x / 4);
	const t_inventory	*inv = &md->inv;
	t_vec2				pos;
	t_vec2				sc;
	t_ent				*pk;

	while (++md->var < 6)
	{
		sc = slot_size;
		if (md->var == inv->hov_indexes[1])
			sc = v2(slot_size.x * 1.4);
		pos.x = base.x + (md->var % 2) * (slot_size.x + spacing.x);
		pos.y = base.y + (md->var / 2) * spacing.y;
		pk = inv->pokemon_team[md->var];
		draw_sphere(inv->img, pos, sc, \
	get_v3(md->rgb[RGB_WHITE - (md->var == inv->hov_indexes[1])], 2, 1));
		if (!pk)
			continue ;
		draw_img(copy_image(md, pk->frame, v2(sc.x), -1), inv->img, \
	add_vec2(v2(slot_size.x / 2), sub_vec2(pos, v2(sc.x / 2))), -1);
		td.x = pos.x + slot_size.x / 2 - ft_strlen(pk->label) * td.scale / 2;
		td.y = pos.y + slot_size.y * 1.3f;
		rnd_fast_txt(md, td, pk->label);
	}
}

void	draw_pokemon_team(t_md *md, t_inventory *inv, t_txtd td, int brdsz)
{
	const t_vec2	slot_size = v2(inv->sz.x / 4);
	const t_vec2	spacing = (t_vec2){slot_size.x - 10, slot_size.y + 10};
	const t_vec2	layout = \
		(t_vec2){2 * slot_size.x + spacing.x, 3 * slot_size.y + 2 * 10};
	const t_vec2	base = (t_vec2){((inv->sz.x - layout.x) / 2 + brdsz / 2), \
		(inv->sz.y - layout.y) / 2};

	md->var = -1;
	draw_team(md, td, spacing, base);
	md->var = 0;
}

void	render_used_shadow(t_md *md, t_vec2 usd_p, t_vec2 u_sz)
{
	t_vec2	p;
	t_vec2	sz;

	sz = get_v2(u_sz.x, u_sz.y / 2);
	p.x = usd_p.x;
	p.y = maxf(usd_p.y + u_sz.y * .8f, md->win_sz.y * .9f - md->cam.rot.y * 4);
	draw_sphere(md->screen, p, sz, get_v3(_BLACK, 3, 1));
}
