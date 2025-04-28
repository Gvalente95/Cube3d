/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pokeball.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:05:46 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/28 15:34:55 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	use_held_item(t_md *md, t_inventory *inv, t_ent *pointed, int index)
{
	if (index == Keys && md->cam.pointed_door)
	{
		inv->held_used = 1;
		return (1);
	}
	else if (pointed)
	{
		inv->held_used = 1;
		inv->pokeball_timer = md->timer.cur_tm;
		inv->arc_origin = inv->pkbl_p;
		return (1);
	}
	else
	{
		inv->items[index]++;
		inv->held_index = -1;
		add_alert(md, .5f, NULL, "Can't use that");
		return (0);
	}
}

void	capture_pokemon(t_md *md, t_inventory *inv, t_ent *e)
{
	if (!e)
		return ;
	if (inv->team_size > 5)
		add_alert(md, 2, NULL, "Team already full, placing in computer");
	inv->pokemon_team[inv->team_size++] = e;
	play_sound(md, md->au.pokemon_cries[e->mob_type]);
	add_log_to_queue(md, _BLUE, "%s caught!", e->label);
	remove_ent(md, e);
	inv->update_img = 1;
	e->is_active = 0;
}
