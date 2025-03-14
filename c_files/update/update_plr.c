/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_plr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:43:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/14 07:07:25 by gvalente         ###   ########.fr       */
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
		mov.z -= (int)(PLR_JUMPPOW * spd);
	md->input_mov = get_v3f(mv_lft + mv_rght, mv_for + mv_back, md->key_clicked == SPACE_KEY || md->key_prs[NUM_LFTCMD_KEY]);
	return (mov);
}

static t_vec3f	set_input_mov(t_md *md)
{
	float	spd;
	t_vec3f	for_dir;
	t_vec3f	rgt_dir;

	spd = ACCSPD;
	if (md->key_prs[SHIFT_KEY])
	{
		spd *= 2;
	}
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
	if (fabsf(md->plr.pos.z) >= md->win_size.y)
	{
		md->plr.mov.z = 0;
		md->plr.pos.z = 0;
	}
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

int	update_map_index(t_md *md, t_ent *e)
{
	int		new_map_index;
	char	cur_map_char;

	new_map_index = e->coord_pos.x + ((md->map.size.x + 1) * e->coord_pos.y);
	if (new_map_index == e->map_index || new_map_index < 0 || new_map_index > md->map.len)
		return (0);
	cur_map_char = md->map.buffer[e->map_index];
	if (cur_map_char == '0' || cur_map_char == e->character)
		md->map.buffer[e->map_index] = '0';
	cur_map_char = md->map.buffer[new_map_index];
	if (char_in_str(cur_map_char, "\n1"))
		return (0);
	e->map_index = new_map_index;
	md->map.buffer[new_map_index] = e->character;
	md->mapped_ents[e->map_index] = e;
	return (1);
}

int	update_player(t_md *md)
{
	t_ent	*plr;

	plr = &md->plr;

	if (md->mouse_focus)
		update_player_rot(md);
	update_player_mov(md);
	set_collisions(md, plr);
	if (!plr->mov.x)
		md->plr_wrd_mv.x = 0;
	if (!plr->mov.y)
		md->plr_wrd_mv.y = 0;
	move_ent(plr);
	plr->coord_pos = get_v3(\
		plr->pos.x / md->t_len, \
		plr->pos.y / md->t_len, \
		plr->pos.z / md->t_len);
	update_map_index(md, plr);
	return (1);
}
