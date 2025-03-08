/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:45:36 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/07 16:10:12 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	update_cam_ofst(t_md *md)
{
	t_vec3f	dspl;

	dspl = get_v3f(\
		md->plr.pos.x - md->plr.size.x / 2 - md->win_size.x / 2 + md->t_len / 2, \
		md->plr.pos.y - md->plr.size.y / 2 - md->win_size.y / 2 + md->t_len / 2, \
		md->plr.pos.z);
	md->cam_ofst = dspl;
}

int	update(t_md *md)
{
	int	has_moved;

	has_moved = update_mouse(md);
	if (update_player(md))
		has_moved = 1;
	if (has_moved)
		update_cam_ofst(md);
	return (!md->time || has_moved);
}

void	update_keys(t_md *md)
{
	if (md->key_clicked == R_KEY)
		md->ray_mode = !md->ray_mode;
	if (md->key_clicked == M_KEY)
		md->mmap.active = !md->mmap.active;
	if (md->key_clicked == ESC_KEY)
		free_and_quit(md, NULL, NULL);
	if (md->key_clicked == TAB_KEY)
		md->debug_mode = !md->debug_mode;
	if (md->mouse_pressed == MOUSE_PRESS)
	{
		md->plr.pos.x -= (md->plr.pos.x - md->mouse_world_pos.x) * .06f;
		md->plr.pos.y -= (md->plr.pos.y - md->mouse_world_pos.y) * .06f;
	}
	if (md->mouse_clicked == MOUSE_DPRESS)
		md->plr.pos = get_v3f(md->mouse_world_pos.x, md->mouse_world_pos.y, md->plr.pos.z);
}

int	update_and_render(t_md *md)
{
	update_keys(md);
	if (update(md))
		render(md);
	md->time++;
	reset_mlx_values(md);
	return (0);
}
