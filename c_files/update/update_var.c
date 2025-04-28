/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:56:16 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/28 16:04:25 by giuliovalen      ###   ########.fr       */
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

void	add_alert(t_md *md, double duration, int *out_cond, const char *alert)
{
	t_log	*ld;

	if (md->timer.cur_tm > md->alert.duration)
		play_sound(md, AU_LOW_CLICK);
	ld = &md->alert;
	ft_strlcpy(ld->buffer, alert, 254);
	ld->txt_d.x = md->win_sz.x / 2 - (ft_strlen(alert) * ld->txt_d.scale) / 2;
	ld->duration = md->timer.cur_tm + duration;
	ld->out_cond = out_cond;
}
