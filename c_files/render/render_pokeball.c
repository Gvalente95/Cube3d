/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pokeball.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 00:26:01 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/22 23:06:22 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static t_vec2	get_size(t_md *md, float elapsed, double throw_dur, int max)
{
	float			t;
	float			scale;
	const float		min = md->inv.pkbl_sz.x * .2f;
	const t_vec2	max_size = v2(max);

	if (!md->inv.throwing_pokebal)
		return (max_size);
	t = fminf(elapsed / throw_dur, 1.0f);
	scale = 1.0f - t * (1.0f - min / (float)max);
	return (scale_vec2(max_size, scale));
}

static t_vec2	get_pos(t_md *md, t_vec2 sz, double elapsed, double throw_dur)
{
	float	t;
	float	arc_height;
	float	offset_y;
	t_vec2	pos;

	if (!md->inv.pkbl_set)
	{
		pos.x = md->win_sz.x / 2 - sz.x / 2;
		pos.y = md->win_sz.y / 2 - sz.y / 2;
		md->inv.pkbl_set = 1;
		md->inv.pkbl_rot_z = 0;
		return (pos);
	}
	md->inv.pkbl_rot_z = minmaxf(-180, 180, \
		md->inv.pkbl_rot_z - md->mouse.delta.x * .05f);
	if (!md->inv.throwing_pokebal)
		return (add_vec2(md->inv.pkbl_p, md->mouse.delta));
	t = fminf(elapsed / throw_dur, 1.0f);
	arc_height = 120.0f;
	offset_y = -4.0f * arc_height * t * (1.0f - t);
	md->inv.arc_origin.x -= md->mouse.delta.x * 10;
	md->inv.arc_origin.y -= md->mouse.delta.y * 5;
	md->inv.arc_origin.x += md->cam.input_mov.x * 10;
	pos = (t_vec2){md->inv.arc_origin.x, md->inv.arc_origin.y + offset_y};
	return (pos);
}

static void	end_pokeball_anim(t_md *md, t_inventory *inv, t_ent *sel)
{
	t_image	*base_txtr;

	base_txtr = md->txd.pickup_txtr[Pokeball][0];
	inv->hold_pkbl = 0;
	inv->throwing_pokebal = 0;
	capture_pokemon(md, inv, sel);
	free_image_data(md, inv->pkball);
	inv->pkball = copy_image(md, base_txtr, inv->pkbl_sz, -1);
	md->txd.last_pointed = NULL;
	inv->pkbl_set = 0;
}

void	render_pokeball(t_md *md, t_inventory *inv, double throw_dur)
{
	t_vec2			sz;
	t_vec2			pos;
	const float		elapsed_time = md->timer.cur_tm - inv->pokeball_timer;
	t_ent			*sel;
	const int		throwing = inv->throwing_pokebal;

	sel = md->txd.last_pointed;
	sz = get_size(md, elapsed_time, throw_dur, md->win_sz.x * .2f);
	if (!cmp_vec2(sz, inv->pkball->size))
	{
		free_image_data(md, inv->pkball);
		inv->pkball = copy_image(md, md->txd.pickup_txtr[Pokeball][0], sz, -1);
	}
	pos = get_pos(md, sz, elapsed_time, throw_dur);
	inv->pkbl_p = pos;
	draw_rotated(md, inv->pkball, md->screen, \
		(t_vec3f){(float)pos.x, (float)pos.y, inv->pkbl_rot_z});
	if (sel && !sel->caught && throwing && elapsed_time > throw_dur)
		sel->caught = 1;
	if (throwing && elapsed_time >= throw_dur * 1.5)
		end_pokeball_anim(md, inv, sel);
}
