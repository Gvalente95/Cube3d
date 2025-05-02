/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_inv_team.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:28:17 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 13:19:11 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	draw_team_slot(t_md *md, t_txtd td, t_vec2 spacing, t_vec2 base)
{
	const t_inventory	*inv = &md->inv;
	const t_vec2		slot_size = _v2(inv->sz.x / 4);
	t_ent				*pk;
	t_vec2				pos;
	t_vec2				sc;
	t_vec2				img_pos;

	sc = slot_size;
	if (md->var == inv->hov_indexes[1])
		sc = _v2(slot_size.x * 1.4);
	pos.x = base.x + (md->var % 2) * (slot_size.x + spacing.x);
	pos.y = base.y + (md->var / 2) * spacing.y;
	pk = inv->pokemon_team[md->var];
	draw_sphere(inv->img, pos, sc, \
		v3(md->rgb[RGB_WHITE - (md->var == inv->hov_indexes[1])], 2, 1));
	if (!pk)
		return ;
	img_pos = add_vec2(_v2(slot_size.x / 2), sub_vec2(pos, _v2(sc.x / 2)));
	draw_img(copy_image(md, pk->frame, _v2(sc.x), -1), inv->img, img_pos, -1);
	draw_hp_bar(md, pk, (t_vec2){pos.x, pos.y + sc.y}, (t_vec2){100, 20});
	if (inv->hov_indexes[1] != md->var)
		return ;
	td.x = pos.x + slot_size.x / 2 - ft_strlen(pk->label) * td.scale / 2;
	td.y = pos.y + sc.y - md->prm.txt_sc * 1.3;
	rnd_fast_txt(md, td, pk->label);
}

void	draw_pokemon_team(t_md *md, t_inventory *inv, t_txtd td, int brdsz)
{
	const t_vec2	slot_size = _v2(inv->sz.x / 4);
	const t_vec2	spacing = (t_vec2){slot_size.x - 10, slot_size.y + 10};
	const t_vec2	layout = \
		(t_vec2){2 * slot_size.x + spacing.x, 3 * slot_size.y + 2 * 10};
	const t_vec2	base = (t_vec2){((inv->sz.x - layout.x) / 2 + brdsz / 2), \
		(inv->sz.y - layout.y) / 2};

	md->var = -1;
	while (++md->var < 6)
		draw_team_slot(md, td, spacing, base);
	md->var = 0;
}

void	render_used_shadow(t_md *md, t_vec2 usd_p, t_vec2 u_sz, double elapsed, double dur)
{
	t_vec2	p;
	t_vec2	sz;

	sz = v2(u_sz.x, u_sz.y / 2);
	p.x = usd_p.x;
	p.y = usd_p.y + u_sz.y * .8f;
	p.y = maxf(usd_p.y + u_sz.y * .8f, md->win_sz.y * .9f - md->cam.rot.y * 4);
	p.y -= minf(elapsed, dur) * 400;
	draw_sphere(md->screen, p, sz, v3(_BLACK, 1, 1));
}
