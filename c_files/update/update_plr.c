/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_plr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:43:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/12 11:17:26 by giuliovalen      ###   ########.fr       */
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
	mv_for = ((md->key_prs[W_KEY] || md->key_prs[UP_KEY]) * spd);
	mv_back = ((md->key_prs[S_KEY] || md->key_prs[DOWN_KEY]) * spd);
	mv_rght = ((md->key_prs[D_KEY] || md->key_prs[RIGHT_KEY]) * spd);
	mv_lft = ((md->key_prs[A_KEY] || md->key_prs[LEFT_KEY]) * spd);
	mov.x = (mv_for - mv_back) * for_dir.x + (mv_rght - mv_lft) * -rgt_dir.x;
	mov.y = (mv_for - mv_back) * for_dir.y + (mv_rght - mv_lft) * -rgt_dir.y;
	if (md->key_clicked == SPACE_KEY && md->plr.pos.z == 0)
		mov.z -= PLR_JUMPPOW * spd;
	mov.z -= (md->key_prs[NUM_LFTCMD_KEY] * (spd / 3));
	md->input_mov = get_v3f(mv_for + mv_back, mv_lft + mv_rght, md->key_clicked == SPACE_KEY || md->key_prs[NUM_LFTCMD_KEY]);
	return (mov);
}

static t_vec3f	set_input_mov(t_md *md)
{
	float	spd;
	t_vec3f	for_dir;
	t_vec3f	rgt_dir;

	spd = ACCSPD;
	if (md->key_prs[SHIFT_KEY] == 1)
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
	else if (md->plr.pos.z >= -EPSILON && md->plr.mov.z > GRAVITY)
	{
		md->plr.pos.z = 0;
		md->plr.mov.z = 0;
	}
	new_mov = get_v3f(
		md->plr.mov.x + conv_mov.x,
		md->plr.mov.y + conv_mov.y,
		md->plr.mov.z + conv_mov.z
	);
	for_dir = get_v3f(cosf(md->plr.angle), sinf(md->plr.angle), 0);
	rgt_dir = get_v3f(cosf(md->plr.angle - M_PI_2), sinf(md->plr.angle - M_PI_2), 0);
	md->plr_wrd_mv = get_v3f(
		new_mov.x * rgt_dir.x + new_mov.y * rgt_dir.y,
		-(new_mov.x * for_dir.x + new_mov.y * for_dir.y),
		new_mov.z
	);
	md->plr.mov = new_mov;
	if (!conv_mov.x && !conv_mov.y)
		return (none);
	if (fabsf(conv_mov.x) > fabsf(conv_mov.y))
		return (right - (conv_mov.x > 0) * 2);
	return (down - (conv_mov.y > 0) * 2);
}

static void	update_player_rot(t_md *md)
{
	md->plr.rot.x = fmod(md->mouse_pos.x * SCROLL_SPD, 360.0f);
	if (md->plr.rot.x < -180.0f)
		md->plr.rot.x += 360.0f;
	else if (md->plr.rot.x > 180.0f)
		md->plr.rot.x -= 360.0f;
	if (md->ray_mode)
	{
		md->plr.rot.y = (md->mouse_pos.y - md->win_size.y / 2) * SCROLL_SPD;
		md->plr.rot.y = minmaxf(-80, 80, md->plr.rot.y);
	}
	md->plr.angle = md->plr.rot.x * (M_PI / 180.0f);
}

int	update_player(t_md *md)
{
	t_ent	*plr;

	plr = &md->plr;

	if (md->mouse_focus)
		update_player_rot(md);
	update_player_mov(md);
	set_collisions(md, plr);
	move_ent(plr);
	plr->coord_pos = get_v3(\
		plr->pos.x / md->t_len, \
		plr->pos.y / md->t_len, \
		plr->pos.z / md->t_len);
	return (1);
}
