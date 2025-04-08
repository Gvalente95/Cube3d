/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:56:16 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/07 22:59:07 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	plr_shoot(t_md *md)
{
	if (!md->plr.can_shoot)
		return ;
	if (md->hud.ammo < md->hud.wpn_index)
		return ;
	md->hud.ammo = minmax(0, MAX_AMMO, md->hud.ammo - md->hud.wpn_index);
	md->plr.shot_timer = \
		get_time_in_seconds() + (double)(SHOOT_REFRESH * md->hud.wpn_index);
	md->plr.shot = md->hud.wpn_index;
	md->plr.can_shoot = 0;
	md->hud.weapon_frame = 1;
	if (md->hud.wpn_index == Knife)
		play_sound(md, AU_SLICE);
}
