/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:55:29 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 05:29:53 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	update_arrow_rotation(t_md *md)
{
	float	rot_speed;

	rot_speed = ARROW_ROT_SPEED;
	if (md->key_prs[SHIFT_KEY])
		rot_speed *= 2;
	if (md->key_prs[LEFT_KEY])
		md->plr.rot.x -= rot_speed;
	if (md->key_prs[RIGHT_KEY])
		md->plr.rot.x += rot_speed;
	if (md->key_prs[UP_KEY])
		md->plr.rot.y -= rot_speed;
	if (md->key_prs[DOWN_KEY])
		md->plr.rot.y += rot_speed;
}

static void	update_key_input(t_md *md)
{
	if (md->key_clicked == NUM_Y_KEY)
		md->lock_rotation.y = !md->lock_rotation.y;
	if (md->key_clicked == X_KEY)
		md->lock_rotation.x = !md->lock_rotation.x;
	if (md->key_clicked == NUM_1_KEY)
		md->real_mode = !md->real_mode;
	if (md->key_clicked == TAB_KEY)
		md->hud.wpn_index = (md->hud.wpn_index + 1) % (WEAPON_TYPE_LEN - 1);
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
		md->prev_mouse = md->mouse_real;
		md->menu.active = 1;
	}
	if (md->key_clicked == B_KEY)
		md->hud.active_background = !md->hud.active_background;
	if (md->key_clicked == Q_KEY)
		free_and_quit(md, NULL, NULL);
}

static void	update_mouse_input(t_md *md)
{
	md->plr.shot = 0;
	if (md->plr.can_shoot)
		if ((md->mouse_click == MOUSE_PRESS || md->key_clicked == C_KEY))
			plr_shoot(md);
	if (md->mouse_click == MOUSE_DPRESS)
		md->lock_mouse = !md->lock_mouse;
	if (md->real_mode)
		return ;
	if (md->mouse_pressed == MOUSE_PRESS)
	{
		md->plr.pos.x -= (md->plr.pos.x - md->mouse_world_pos.x) * .06f;
		md->plr.pos.y -= (md->plr.pos.y - md->mouse_world_pos.y) * .06f;
	}
	if (md->mouse_click == MOUSE_DPRESS)
		md->plr.pos = get_v3f(md->mouse_world_pos.x, \
				md->mouse_world_pos.y, md->plr.pos.z);
}

void	update_input(t_md *md)
{
	update_arrow_rotation(md);
	update_mouse_input(md);
	update_key_input(md);
}

int	update_menu_input(t_md *md, t_menu *menu)
{
	if (md->key_clicked == ESC_KEY)
	{
		mlx_mouse_move(md->win, md->prev_mouse.x, md->prev_mouse.y);
		menu->active = 0;
		return (0);
	}
	if (md->key_clicked == Q_KEY)
		free_and_quit(md, NULL, NULL);
	return (1);
}
