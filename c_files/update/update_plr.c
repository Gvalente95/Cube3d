/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_plr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:43:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 01:52:23 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static t_vec3f	set_input_mov_2(t_md *md, float spd, \
		t_vec3f for_dir, t_vec3f rgt_dir)
{
	t_vec3f	mov;
	float	mv_for;
	float	mv_back;
	float	mv_rght;
	float	mv_lft;

	mov = get_v3f(0, 0, 0);
	mv_for = ((md->key_prs[W_KEY]) * spd);
	mv_back = ((md->key_prs[S_KEY]) * spd);
	mv_rght = ((md->key_prs[D_KEY]) * spd);
	mv_lft = ((md->key_prs[A_KEY]) * spd);
	mov.x = (mv_for - mv_back) * for_dir.x + (mv_rght - mv_lft) * -rgt_dir.x;
	mov.y = (mv_for - mv_back) * for_dir.y + (mv_rght - mv_lft) * -rgt_dir.y;
	if (md->key_clicked == SPACE_KEY && md->plr.pos.z >= -EPSILON)
		mov.z -= (PLR_JUMPPOW);
	if (md->key_prs[R_KEY] == 1)
		mov.z = -(PLR_JUMPPOW * spd);
	md->input_mov = get_v3f(mv_lft - mv_rght, mv_for - mv_back, md->key_clicked == SPACE_KEY || md->key_prs[NUM_LFTCMD_KEY]);
	return (mov);
}

static t_vec3f	set_input_mov(t_md *md)
{
	float	spd;
	t_vec3f	for_dir;
	t_vec3f	rgt_dir;

	spd = ACCSPD;
	if (md->key_prs[SHIFT_KEY])
		spd *= 2;
	for_dir.x = cosf(md->plr.angle);
	for_dir.y = sinf(md->plr.angle);
	rgt_dir.x = cosf(md->plr.angle - M_PI_2);
	rgt_dir.y = sinf(md->plr.angle - M_PI_2);
	return (set_input_mov_2(md, spd, for_dir, rgt_dir));
}

static int	update_player_mov(t_md *md)
{
	t_vec3f	conv_mov;
	t_vec3f	new_mov;
	t_vec3f	for_dir;
	t_vec3f	rgt_dir;

	conv_mov = set_input_mov(md);
	if (md->plr.pos.z < 0)
		md->plr.mov.z += GRAVITY;
	else if (md->plr.pos.z > 1)
	{
		md->plr.pos.z = 0;
		md->plr.mov.z = 0;
	}
	new_mov = get_v3f(md->plr.mov.x + conv_mov.x, md->plr.mov.y + conv_mov.y, \
		md->plr.mov.z + conv_mov.z);
	for_dir = get_v3f(cosf(md->plr.angle), sinf(md->plr.angle), 0);
	rgt_dir = get_v3f(cosf(md->plr.angle - M_PI_2), \
		sinf(md->plr.angle - M_PI_2), 0);
	md->plr_wrd_mv = get_v3f(\
		new_mov.x * rgt_dir.x + new_mov.y * rgt_dir.y, \
		-(new_mov.x * for_dir.x + new_mov.y * for_dir.y), new_mov.z);
	md->plr.mov = get_v3f(new_mov.x * md->timer.delta_time, new_mov.y * md->timer.delta_time, new_mov.z);
	return (conv_mov.x || conv_mov.y);
}

static void	update_player_rot(t_md *md)
{
	if (!md->mouse_focus)
		return ;
	md->plr.rot.x = fmod((md->mouse_pos.x - md->arrow_rot.x), 360.0f);
	if (md->plr.rot.x < -180.0f)
		md->plr.rot.x += 360.0f;
	else if (md->plr.rot.x > 180.0f)
		md->plr.rot.x -= 360.0f;
	if (md->lock_y)
		md->plr.rot.y = 0;
	else
		md->plr.rot.y = (md->mouse_pos.y - md->arrow_rot.y);
	if (md->ray_mode)
	{
		md->plr.rot.y = minmaxf(-40, 40, md->plr.rot.y);
		md->plr.angle = md->plr.rot.x * (M_PI / 180.0f);
	}
	md->plr.dir.x = cosf(md->plr.angle);
	md->plr.dir.y = sinf(md->plr.angle);
	md->plr.dir.z = 0;
}

int	update_player(t_md *md, t_ent *plr)
{
	if (!md->plr.can_shoot && \
		md->timer.current_time - SHOOT_REFRESH > md->plr.shot_timer)
		md->plr.can_shoot = 1;
	update_player_rot(md);
	update_player_mov(md);
	set_collisions(md, plr);
	if (!plr->mov.x)
		md->plr_wrd_mv.x = 0;
	if (!plr->mov.y)
		md->plr_wrd_mv.y = 0;
	move_player(md, plr);
	md->input_offst = get_v2(md->input_offst.x + (int)(md->plr_wrd_mv.x), \
		md->input_offst.y - (int)(md->plr_wrd_mv.y));
	return (1);
}
