/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_plr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:43:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/07 19:52:08 by giuliovalen      ###   ########.fr       */
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
	float		speed;
	const float	pitch = md->cam.rot.y * (M_PI / 180.0f);

	speed = md->prm.rot_speed;
	if (!md->mouse.lock_rot.x && md->mouse.delta.x && md->mouse.focus)
		md->cam.rot.x += (md->mouse.delta.x * speed);
	if (md->cam.rot.x < -180.0f)
		md->cam.rot.x += 360.0f;
	else if (md->cam.rot.x > 180.0f)
		md->cam.rot.x -= 360.0f;
	if (!md->mouse.lock_rot.y && md->mouse.delta.y && md->mouse.focus)
		md->cam.rot.y += (md->mouse.delta.y * speed);
	if (md->prm.fly_cam)
		md->cam.rot.y = minmaxf(-140, 140, md->cam.rot.y);
	else
		md->cam.rot.y = minmaxf(-80, 80, md->cam.rot.y);
	md->cam.rot.z = 0;
	md->plr.angle = (md->cam.rot.x) * (M_PI / 180.0f);
	md->plr.dir.x = cosf(md->plr.angle);
	md->plr.dir.y = sinf(md->plr.angle);
	md->plr.dir.z = sinf(pitch) * .05;
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
			md->cam.plr_wrd_mv.x = 0;
		if (!plr->mov.y)
			md->cam.plr_wrd_mv.y = 0;
		move_player(md, plr);
	}
	else if (md->plr.shot)
		md->plr.action = m_atk;
	else
		md->plr.action = m_idle;
	if (md->plr.action != prv_action)
		md->plr.frame_index = 0;
}

static void	update_player_weapon(t_md *md, t_ent *plr)
{
	if (md->prm.ent_mode && md->timer.trig_anim && md->hud.weapon_frame >= 1)
		md->hud.weapon_frame++;
	if (!plr->can_shoot && \
		md->timer.cur_tm > plr->shot_timer && !plr->shot)
		plr->can_shoot = 1;
}

int	update_player(t_md *md, t_ent *plr)
{
	update_player_weapon(md, plr);
	update_player_rot(md);
	update_player_mov(md, plr);
	update_player_action(md, plr);
	update_cam(md, &md->cam);
	md->plr.pos.z = minmaxf(-md->t_len * 10, 0, md->plr.pos.z);
	md->hud.floor_start = md->win_sz.y / 2 - md->cam.rot.y * 8 + 1;
	if (md->hud.floor_start < 0)
		md->hud.floor_start = 0;
	return (1);
}
