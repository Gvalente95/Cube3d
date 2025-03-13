/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ents_data.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:57:39 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/13 01:21:50 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	init_e_sizes(t_md *md, t_vec2 base)
{
	int		i;

	md->e_sizes[nt_plr] = get_v2(base.x * .8, base.y * .8);
	md->e_sizes[nt_mob] = get_v2(base.x / 2, base.y / 2);
	md->e_sizes[nt_coin] = get_v2(base.x / 3, base.y / 3);
	md->e_sizes[nt_key] = base;
	md->e_sizes[nt_door] = base;
	md->e_sizes[nt_portal] = base;
	md->e_sizes[nt_wall] = base;
	md->e_sizes[nt_empty] = base;
	md->e_sizes[nt_axe] = base;
	md->e_sizes[nt_ground] = base;
	md->e_sizes[nt_tile] = base;
	md->e_sizes[nt_mover] = base;
	md->e_sizes[nt_bush] = base;
	md->e_sizes[nt_spike] = base;
	md->e_sizes[nt_thrower] = base;
	md->e_sizes[nt_trees] = base;
	md->e_sizes[nt_star] = base;
	md->e_sizes[nt_road] = base;
	md->e_sizes[nt_env] = base;
	md->e_sizes[nt_prt] = base;
	i = -1;
	while (++i < ENT_TYPE_LEN)
		md->e_sizes_2d[i] = get_v2(md->size_2d, md->size_2d);
}

static void	init_2d_txtrs(t_md *md)
{
	t_vec2	*sz;

	sz = md->e_sizes_2d;
	md->txtr_2d = malloc(sizeof(t_ent *) * (ENT_TYPE_LEN + 1));
	md->txtr_2d[nt_plr] = ld_txtr(md, sz[nt_plr], "entities/plr/0");
	md->txtr_2d[nt_mob] = ld_txtr(md, sz[nt_mob], "entities/mobs/0");
	md->txtr_2d[nt_coin] = ld_txtr(md, sz[nt_coin], "env/grab/coin/0");
	md->txtr_2d[nt_key] = ld_txtr(md, sz[nt_key], "env/grab/key/0");
	md->txtr_2d[nt_door] = ld_txtr(md, sz[nt_door], "env/door/0");
	md->txtr_2d[nt_portal] = ld_txtr(md, sz[nt_portal], "env/portal/0");
	md->txtr_2d[nt_wall] = ld_txtr(md, sz[nt_wall], "env/wall/0");
	md->txtr_2d[nt_empty] = ld_txtr(md, sz[nt_empty], "0");
	md->txtr_2d[nt_axe] = ld_txtr(md, sz[nt_axe], "env/grab/axe/0");
	md->txtr_2d[nt_ground] = ld_txtr(md, sz[nt_ground], "env/road/0");
	md->txtr_2d[nt_tile] = ld_txtr(md, sz[nt_tile], "env/bush/0");
	md->txtr_2d[nt_mover] = ld_txtr(md, sz[nt_mover], "entities/mvr/0");
	md->txtr_2d[nt_bush] = ld_txtr(md, sz[nt_bush], "env/bush/0");
	md->txtr_2d[nt_spike] = ld_txtr(md, sz[nt_spike], "env/road/0");
	md->txtr_2d[nt_thrower] = ld_txtr(md, sz[nt_thrower], "env/road/0");
	md->txtr_2d[nt_trees] = ld_txtr(md, sz[nt_trees], "env/trees/0");
	md->txtr_2d[nt_star] = ld_txtr(md, sz[nt_star], "env/road/0");
	md->txtr_2d[nt_road] = ld_txtr(md, sz[nt_road], "env/road/0");
	md->txtr_2d[nt_env] = ld_txtr(md, sz[nt_env], "env/road/0");
	md->txtr_2d[nt_prt] = ld_txtr(md, sz[nt_prt], "env/particles/0");
	md->txtr_2d[ENT_TYPE_LEN] = NULL;
}

static void	init_e_txtrs(t_md *md)
{
	t_vec2	*siz;

	siz = md->e_sizes;
	md->e_frms[nt_plr][0] = ld_txtrs(md, siz[nt_plr], "entities/plr/");
	md->e_frms[nt_mob][0] = ld_txtrs(md, siz[nt_mob], "entities/mobs/");
	md->e_frms[nt_coin][0] = ld_txtrs(md, siz[nt_coin], "env/grab/coin/");
	md->e_frms[nt_key][0] = ld_txtrs(md, siz[nt_key], "env/grab/key/");
	md->e_frms[nt_door][0] = ld_txtrs(md, siz[nt_door], "env/door/");
	md->e_frms[nt_portal][0] = ld_txtrs(md, siz[nt_portal], "env/portal/");
	md->e_frms[nt_wall][0] = ld_txtrs(md, siz[nt_wall], "env/wall/");
	md->e_frms[nt_empty][0] = ld_txtrs(md, siz[nt_empty], "");
	md->e_frms[nt_axe][0] = ld_txtrs(md, siz[nt_axe], "");
	md->e_frms[nt_ground][0] = ld_txtrs(md, siz[nt_ground], "env/road/");
	md->e_frms[nt_tile][0] = ld_txtrs(md, siz[nt_tile], "env/bush/");
	md->e_frms[nt_mover][0] = ld_txtrs(md, siz[nt_mover], "entities/mvr/");
	md->e_frms[nt_bush][0] = ld_txtrs(md, siz[nt_bush], "env/bush/");
	md->e_frms[nt_spike][0] = ld_txtrs(md, siz[nt_spike], "env/road/");
	md->e_frms[nt_thrower][0] = ld_txtrs(md, siz[nt_thrower], "env/road/");
	md->e_frms[nt_trees][0] = ld_txtrs(md, siz[nt_trees], "env/trees/");
	md->e_frms[nt_star][0] = ld_txtrs(md, siz[nt_star], "env/road/");
	md->e_frms[nt_road][0] = ld_txtrs(md, siz[nt_road], "env/road/");
	md->e_frms[nt_env][0] = ld_txtrs(md, siz[nt_env], "env/road/");
	md->e_frms[nt_prt][0] = ld_txtrs(md, siz[nt_prt], "env/particles/");
}

int	init_txtrs(t_md *md)
{
	int		i;

	md->e_frms = malloc(sizeof(void ***) * (ENT_TYPE_LEN + 1));
	if (!md->e_frms)
		return (printf("alloc failed for frames"), 0);
	i = -1;
	while (++i < ENT_TYPE_LEN)
		md->e_frms[i] = md_malloc(md, sizeof(void **) * ENT_ACTION_LEN);
	md->e_frms[ENT_TYPE_LEN] = NULL;
	return (1);
}

void	init_ents_data(t_md *md)
{
	init_e_sizes(md, get_v2(md->t_len, md->t_len));
	init_txtrs(md);
	init_e_txtrs(md);
	init_2d_txtrs(md);
}
