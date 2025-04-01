/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_plr_movement.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 11:45:19 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/01 11:18:52 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	update_cam(t_md *md, t_ent *plr)
{
	float	bob_speed;
	float	bob_amount;
	float	vrt_offset;
	int		is_running;
	t_vec3f	pos;

	pos = plr->pos;
	is_running = md->key_prs[SHIFT_KEY];
	md->cam_pos.x = pos.x + plr->size.x / 2 - (plr->dir.x * md->prm.zoom);
	md->cam_pos.y = pos.y + plr->size.y / 2 - (plr->dir.y * md->prm.zoom);
	md->cam_pos.z = pos.z - md->prm.height;
	if (md->plr.grounded && !cmp_vec3f \
		(get_v3f(plr->mov.x, plr->mov.y, 0), v3f(0), 0.01f))
		md->bob_time += md->timer.delta_time;
	bob_speed = BOB_SPD * (1 + is_running);
	bob_amount = BOB_AMOUNT + (is_running * .4f);
	vrt_offset = sinf(md->bob_time * bob_speed) * bob_amount;
	md->cam_pos.z += vrt_offset;
}

int	move_player(t_md *md, t_ent *e)
{
	t_vec3f	sign_mv;
	t_vec3f	decc;
	t_vec3f	cur;
	float	speed;

	speed = md->prm.plr_speed / RESOLUTION;
	sign_mv = get_v3f(e->mov.x * speed, e->mov.y * speed, e->mov.z * speed);
	decc = get_v3f(e->mov.x * ACCSPD, e->mov.y * ACCSPD, e->mov.z * ACCSPD);
	if (!md->prm.free_cam && e->pos.z + sign_mv.z > 0)
	{
		e->pos.z = 0;
		decc.z = 0;
		sign_mv.z = 0;
		e->mov.z = 0;
	}
	cur = e->pos;
	e->pos = get_v3f(cur.x + sign_mv.x, cur.y + sign_mv.y, cur.z + sign_mv.z);
	e->mov = get_v3f(e->mov.x - decc.x, e->mov.y - decc.y, e->mov.z - decc.z);
	e->coord = get_v3(e->pos.x / md->t_len, e->pos.y / md->t_len, 1);
	return (1);
}

t_vec3f	set_free_cam_mov(t_md *md, float spd)
{
	t_vec3f	mov;
	t_vec3f	cam_forward;
	t_vec3f	cam_right;
	t_vec3f	cam_up;

	mov = get_v3f(0, 0, 0);
	cam_forward = normalize_vec3f(md->plr.dir);
	cam_right = get_v3f(-cam_forward.y, cam_forward.x, 0);
	cam_up = get_v3f(0, 0, -1);
	if (md->key_prs[NUM_W_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_forward, spd));
	if (md->key_prs[NUM_S_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_forward, -spd));
	if (md->key_prs[NUM_D_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_right, spd));
	if (md->key_prs[NUM_A_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_right, -spd));
	if (md->key_prs[NUM_E_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_up, -spd));
	if (md->key_prs[NUM_Q_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_up, spd));
	md->input_mov = get_v3f(
		md->key_prs[NUM_A_KEY] - md->key_prs[NUM_D_KEY],
		md->key_prs[NUM_W_KEY] - md->key_prs[NUM_S_KEY],
		md->key_prs[NUM_E_KEY] - md->key_prs[NUM_Q_KEY]
	);
	return (mov);
}

static t_vec3f	set_input_mov_2(t_md *md, float spd, \
		t_vec3f for_dir, t_vec3f rgt_dir)
{
	t_vec3f	mov;
	float	mv_for;
	float	mv_back;
	float	mv_rght;
	float	mv_lft;

	mov = get_v3f(0, 0, 0);
	mv_for = ((md->key_prs[NUM_W_KEY]) * spd);
	mv_back = ((md->key_prs[NUM_S_KEY]) * spd);
	mv_rght = ((md->key_prs[NUM_D_KEY]) * spd);
	mv_lft = ((md->key_prs[NUM_A_KEY]) * spd);
	mov.x = (mv_for - mv_back) * for_dir.x + (mv_rght - mv_lft) * -rgt_dir.x;
	mov.y = (mv_for - mv_back) * for_dir.y + (mv_rght - mv_lft) * -rgt_dir.y;
	if (md->key_clicked == SPACE_KEY && \
		md->plr.pos.z + md->prm.height >= -EPSILON)
		mov.z -= (-PLR_JUMPPOW);
	if (md->key_prs[NUM_R_KEY] == 1)
		mov.z = -(PLR_JUMPPOW * .2);
	md->input_mov = get_v3f(\
		mv_lft - mv_rght, mv_for - mv_back, \
		md->key_clicked == SPACE_KEY || md->key_prs[NUM_LFTCMD_KEY] == 1);
	if (md->key_clicked == SPACE_KEY)
		play_random_sound(md, AU_WALK_METAL, 6);
	return (mov);
}

static t_vec3f	set_input_mov(t_md *md)
{
	float	spd;
	t_vec3f	for_dir;
	t_vec3f	rgt_dir;

	spd = 1;
	if (md->key_prs[SHIFT_KEY])
		spd *= 2;
	if (md->prm.free_cam)
		return (set_free_cam_mov(md, spd));
	for_dir.x = cosf(md->plr.angle);
	for_dir.y = sinf(md->plr.angle);
	rgt_dir.x = cosf(md->plr.angle - M_PI_2);
	rgt_dir.y = sinf(md->plr.angle - M_PI_2);
	return (set_input_mov_2(md, spd, for_dir, rgt_dir));
}

int	update_player_mov(t_md *md)
{
	t_vec3f	conv_mov;
	t_vec3f	new_mov;
	t_vec3f	for_dir;
	t_vec3f	rgt_dir;

	conv_mov = set_input_mov(md);
	if (!md->prm.free_cam && md->plr.pos.z + md->prm.height < 0)
		md->plr.mov.z += GRAVITY;
	else if (!md->prm.free_cam && md->plr.pos.z + md->prm.height > 1)
	{
		md->plr.pos.z = -md->prm.height;
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
	md->plr.mov = get_v3f(md->plr.mov.x + new_mov.x * md->timer.delta_time, \
		md->plr.mov.y + new_mov.y * md->timer.delta_time, new_mov.z);
	md->plr.grounded = md->plr.pos.z - md->prm.height <= .1;
	return (conv_mov.x || conv_mov.y);
}
