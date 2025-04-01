/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:55:29 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/01 01:53:50 by giuliovalen      ###   ########.fr       */
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

static void	update_key_input(t_md *md, unsigned int c)
{
	int	i;

	if (c == (unsigned int)-1)
		return ;
	i = -1;
	while (md->menu.buttons[++i].active)
	{
		if (c != md->menu.buttons[i].key_trigger)
			continue ;
		*md->menu.buttons[i].value = !(*md->menu.buttons[i].value);
		break ;
	}
	if (c == TAB_KEY)
		set_weapon_index(md);
	else if (c == ESC_KEY)
		set_menu_mode(md, &md->menu, 1);
	else if (c == NUM_Q_KEY)
		free_and_quit(md, NULL, NULL);
	else if (c == SHIFT_KEY)
		md->timer.tm_walk = md->timer.cur_tm - 1;
	else if (c == NUM_C_KEY)
		plr_shoot(md);
	else if (c == NUM_H_KEY)
		set_mouse_lock(md, !md->mouse.locked);
}

static void	update_mouse_input(t_md *md)
{
	if (md->plr.shot)
		md->plr.shot--;
	if (md->mouse.click != MOUSE_NOPRESS && !md->mouse.locked)
		set_mouse_lock(md, 1);
	if (md->mouse.click)
		play_sound(md, AU_GRAB);
	if (md->prm.ray_mode)
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
	update_key_input(md, md->key_clicked);
}

int	update_menu_input(t_md *md, t_menu *menu)
{
	if (md->key_clicked == ESC_KEY)
		return (set_menu_mode(md, menu, 0));
	if (md->key_clicked == NUM_Q_KEY)
		free_and_quit(md, NULL, NULL);
	return (1);
}
