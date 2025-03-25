/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_plr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:43:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 13:56:43 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	set_weapon_index(t_md *md)
{
	md->hud.wpn_index = (md->hud.wpn_index + 1) % (WEAPON_TYPE_LEN - 1);
	while (!md->hud.unlocked_weapons[md->hud.wpn_index])
		md->hud.wpn_index = (md->hud.wpn_index + 1) % (WEAPON_TYPE_LEN - 1);
}

static void	update_player_rot(t_md *md)
{
	float	speed;

	speed = md->prm.rot_speed;
	if (!md->mouse.focus)
		return ;
	if (!md->mouse.lock_rotation.x && md->mouse.delta.x)
		md->plr_rot.x += (md->mouse.delta.x * speed);
	if (md->plr_rot.x < -180.0f)
		md->plr_rot.x += 360.0f;
	else if (md->plr_rot.x > 180.0f)
		md->plr_rot.x -= 360.0f;
	if (!md->mouse.lock_rotation.y)
		md->plr_rot.y += (md->mouse.delta.y * speed);
	md->plr_rot.y = minmaxf(-80, 80, md->plr_rot.y);
	md->plr_rot.z = 0;
	md->plr.angle = (md->plr_rot.x) * (M_PI / 180.0f);
	md->plr.dir.x = cosf(md->plr.angle);
	md->plr.dir.y = sinf(md->plr.angle);
	md->plr.dir.z = 0;
}

static void	update_player_action(t_md *md, t_ent *plr)
{
	int				is_moving;
	t_ent_action	prv_action;

	prv_action = plr->action;
	is_moving = !cmp_vec3f(md->plr.mov, v3f(0), EPSILON);
	if (is_moving)
	{
		set_collisions(md, plr, get_v2(plr->size.x, plr->size.y));
		md->plr.action = m_walk;
		if (!plr->mov.x)
			md->plr_wrd_mv.x = 0;
		if (!plr->mov.y)
			md->plr_wrd_mv.y = 0;
		move_player(md, plr);
	}
	else if (md->plr.shot)
		md->plr.action = m_atk;
	else
		md->plr.action = m_idle;
	if (md->plr.action != prv_action)
		md->plr.frame_index = 0;
	update_cam(md, plr);
}

static void	update_player_weapon(t_md *md, t_ent *plr)
{
	if (md->prm.ent_mode && md->update_frames && md->hud.weapon_frame >= 1)
		md->hud.weapon_frame++;
	if (!plr->can_shoot && \
		md->timer.current_time > plr->shot_timer && !plr->shot)
		plr->can_shoot = 1;
}

int	update_player(t_md *md, t_ent *plr)
{
	update_player_weapon(md, plr);
	update_player_rot(md);
	update_player_mov(md);
	update_player_action(md, plr);
	md->input_offst = get_v2(md->input_offst.x + (int)(md->plr_wrd_mv.x), \
		md->input_offst.y - (int)(md->plr_wrd_mv.y));
	return (1);
}
