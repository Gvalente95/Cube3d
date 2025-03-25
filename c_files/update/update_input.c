/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:55:29 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 16:00:14 by gvalente         ###   ########.fr       */
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
		md->plr_rot.x -= rot_speed;
	if (md->key_prs[RIGHT_KEY])
		md->plr_rot.x += rot_speed;
	if (md->key_prs[UP_KEY])
		md->plr_rot.y -= rot_speed;
	if (md->key_prs[DOWN_KEY])
		md->plr_rot.y += rot_speed;
}

static void	update_key_input(t_md *md)
{
	unsigned int	c;

	c = md->key_clicked;
	if (c == NUM_Y_KEY)
		md->mouse.lock_rotation.y = !md->mouse.lock_rotation.y;
	else if (c == NUM_X_KEY)
		md->mouse.lock_rotation.x = !md->mouse.lock_rotation.x;
	else if (c == NUM_1_KEY)
		md->prm.real_mode = !md->prm.real_mode;
	else if (c == TAB_KEY)
		set_weapon_index(md);
	else if (c == NUM_2_KEY)
		md->prm.debug_mode = !md->prm.debug_mode;
	else if (c == NUM_3_KEY)
		md->prm.show_rays = !md->prm.show_rays;
	else if (c == NUM_M_KEY)
		md->mmap.active = !md->mmap.active;
	else if (c == NUM_N_KEY)
		md->mmap.collapsed = !md->mmap.collapsed;
	else if (c == ESC_KEY)
		set_menu_mode(md, &md->menu, 1);
	else if (c == NUM_B_KEY)
		md->hud.active_background = !md->hud.active_background;
	else if (c == NUM_Q_KEY)
		free_and_quit(md, NULL, NULL);
}

static void	update_mouse_input(t_md *md)
{
	if (md->plr.shot)
		md->plr.shot--;
	if (md->mouse.click != MOUSE_NOPRESS && !md->mouse.locked)
		set_mouse_lock(md, 1);
	if (md->plr.can_shoot)
		if ((md->mouse.click == MOUSE_PRESS || md->key_clicked == NUM_C_KEY))
			plr_shoot(md);
	if (md->key_clicked == NUM_H_KEY)
		set_mouse_lock(md, !md->mouse.locked);
	if (md->prm.real_mode)
		return ;
	if (md->mouse.pressed == MOUSE_PRESS)
	{
		md->plr.pos.x -= (md->plr.pos.x - md->mouse.world.x) * .06f;
		md->plr.pos.y -= (md->plr.pos.y - md->mouse.world.y) * .06f;
	}
	if (md->mouse.click == MOUSE_DPRESS)
		md->plr.pos = get_v3f(md->mouse.world.x, \
				md->mouse.world.y, md->plr.pos.z);
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
		return (set_menu_mode(md, menu, 0));
	if (md->key_clicked == NUM_Q_KEY)
		free_and_quit(md, NULL, NULL);
	return (1);
}
