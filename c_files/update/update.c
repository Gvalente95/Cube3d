/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:45:36 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 14:18:47 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	update_player_mov(t_md *md)
{
	t_vec3f	start_mov;
	float	spd;

	spd = ACCSPD * (1 + md->key_prs[SHIFT_KEY]);
	start_mov = get_v3f(md->plr.mov.x, md->plr.mov.y, md->plr.mov.z);
	md->plr.mov.y += (md->key_prs[W_KEY] || md->key_prs[UP_KEY]) * spd;
	md->plr.mov.y -= (md->key_prs[S_KEY] || md->key_prs[DOWN_KEY]) * spd;
	md->plr.mov.x += (md->key_prs[D_KEY] || md->key_prs[RIGHT_KEY]) * spd;
	md->plr.mov.x -= (md->key_prs[A_KEY] || md->key_prs[LEFT_KEY]) * spd;
	return (!cmp_vec3f(start_mov, md->plr.mov));
}

int	check_collision(t_md *md, t_ent *e)
{
	(void)md;
	(void)e;
	return (1);
}

int	move_ent(t_ent *e)
{
	t_vec3f	sign_mov;
	t_vec3f	decc_mov;

	sign_mov = get_v3f(e->mov.x * PLRSPD, e->mov.y * PLRSPD, e->mov.z * PLRSPD);
	decc_mov = get_v3f(e->mov.x * ACCSPD, e->mov.y * ACCSPD, e->mov.z * ACCSPD);
	e->pos = get_v3f(\
		e->pos.x + sign_mov.x, \
		e->pos.y + sign_mov.y, \
		e->pos.z + sign_mov.z);
	e->mov = get_v3f(\
		e->mov.x - decc_mov.x, \
		e->mov.y - decc_mov.y, \
		e->mov.z - decc_mov.z);
	return (1);
}

int	update_player(t_md *md)
{
	int	has_moved;

	has_moved = update_player_mov(md);
	has_moved += check_collision(md, &md->plr);
	if (has_moved)
		return (move_ent(&md->plr));
	return (0);
}

void	update_cam_offset(t_md *md)
{
	t_vec3f	dspl;

	dspl = get_v3f(\
		md->plr.pos.x - md->plr.start_pos.x, \
		md->plr.pos.y - md->plr.start_pos.y, \
		md->plr.pos.z - md->plr.start_pos.z);
	md->cam_offset = dspl;
}

int	update(t_md *md)
{
	int	has_moved;

	has_moved = update_mouse(md);
	if (update_player(md))
		has_moved = 1;
	if (has_moved)
		update_cam_offset(md);
	return (!md->time || has_moved);
}

int	update_and_render(t_md *md)
{
	if (md->key_clicked == M_KEY)
		md->show_minimap = !md->show_minimap;
	if (md->key_clicked == ESC_KEY)
		free_and_quit(md, NULL, NULL);
	if (update(md))
		render(md);
	md->time++;
	reset_mlx_values(md);
	return (0);
}
