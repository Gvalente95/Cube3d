/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pokeball.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:05:46 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/22 23:07:18 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	throw_pokeball(t_md *md, t_inventory *inv, t_ent *pointed)
{
	if (pointed && pointed->type == nt_pokemon)
	{
		inv->throwing_pokebal = 1;
		inv->pokeball_timer = md->timer.cur_tm;
		inv->arc_origin = inv->pkbl_p;
		return (1);
	}
	else
	{
		inv->items[Pokeball]++;
		inv->hold_pkbl = 0;
		return (0);
	}
}

void	capture_pokemon(t_md *md, t_inventory *inv, t_ent *e)
{
	(void)inv;
	remove_ent_at_cord(md, get_v2(e->coord.x, e->coord.y));
	play_sound(md, md->au.pokemon_cries[e->mob_type]);
	e->is_active = 0;
}
