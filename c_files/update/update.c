/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:45:36 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 01:48:12 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	update_cam_ofst(t_md *md)
{
	t_vec3f	dspl;
	float	wrd_spd;

	dspl = get_v3f(\
		md->plr.pos.x - md->plr.size.x / 2 - md->win_size.x / 2 + md->t_len / 2, \
		md->plr.pos.y - md->plr.size.y / 2 - md->win_size.y / 2 + md->t_len / 2, \
		md->plr.pos.z);
	md->cam_ofst = dspl;
	wrd_spd = 20;
	md->wrd_mv_offst.x += md->plr_wrd_mv.x * wrd_spd;
	md->wrd_mv_offst.y += md->plr_wrd_mv.y * wrd_spd;
}

static void	update_arrow_rotation(t_md *md)
{
	float	rot_speed;

	rot_speed = ARROW_ROTATION_SPD + (md->key_prs[SHIFT_KEY] * ARROW_ROTATION_SPD);
	if (md->key_prs[LEFT_KEY])
		md->arrow_rot.x += rot_speed;
	if (md->key_prs[RIGHT_KEY])
		md->arrow_rot.x -= rot_speed;
	if (md->key_prs[UP_KEY])
		md->arrow_rot.y += rot_speed;
	if (md->key_prs[DOWN_KEY])
		md->arrow_rot.y -= rot_speed;
}

static void	update_key_params(t_md *md)
{
	if (md->key_clicked == NUM_Y_KEY)
		md->lock_y = !md->lock_y;
	if (md->mouse_clicked == MOUSE_PRESS && md->plr.can_shoot)
		plr_shoot(md);
	if (md->mouse_clicked == MOUSE_DPRESS)
		md->lock_mouse = !md->lock_mouse;
	if (md->key_clicked == NUM_1_KEY)
		md->ray_mode = !md->ray_mode;
	if (md->key_clicked == NUM_2_KEY)
		md->debug_mode = !md->debug_mode;
	if (md->key_clicked == NUM_3_KEY)
		md->show_rays = !md->show_rays;
	if (md->key_clicked == NUM_4_KEY)
		md->mouse_hide = !md->mouse_hide;
	if (md->key_clicked == M_KEY)
		md->mmap.active = !md->mmap.active;
	if (md->key_clicked == ESC_KEY)
	{
		md->prev_mouse = md->mouse_world_pos;
		mlx_mouse_move(md->win, md->win_size.x / 2, md->win_size.y / 2);
		md->menu.active = 1;
	}
	if (md->key_clicked == B_KEY)
		md->hud.active_background = !md->hud.active_background;
	if (md->key_clicked == Q_KEY)
		free_and_quit(md, NULL, NULL);
}

static void	update_keys(t_md *md)
{
	update_arrow_rotation(md);
	update_key_params(md);
	if (md->ray_mode)
		return ;
	if (md->mouse_pressed == MOUSE_PRESS)
	{
		md->plr.pos.x -= (md->plr.pos.x - md->mouse_world_pos.x) * .06f;
		md->plr.pos.y -= (md->plr.pos.y - md->mouse_world_pos.y) * .06f;
	}
	if (md->mouse_clicked == MOUSE_DPRESS)
		md->plr.pos = get_v3f(md->mouse_world_pos.x, \
				md->mouse_world_pos.y, md->plr.pos.z);
}

int	update_and_render(t_md *md)
{
	if (md->menu.active)
		return (update_menu(md, &md->menu));
	md->plr.shot = 0;
	update_keys(md);
	update_mouse(md);
	update_particles(md);
	update_player(md, &md->plr);
	update_cam_ofst(md);
	update_ents(md);
	render(md);
	reset_mlx_values(md);
	update_time(md, &md->timer);
	return (0);
}
