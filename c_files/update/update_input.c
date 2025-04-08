/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:55:29 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/08 02:40:53 by giuliovalen      ###   ########.fr       */
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
		md->cam.rot.x -= rot_speed;
	if (md->key_prs[RIGHT_KEY])
		md->cam.rot.x += rot_speed;
	if (md->key_prs[UP_KEY])
		md->cam.rot.y -= rot_speed;
	if (md->key_prs[DOWN_KEY])
		md->cam.rot.y += rot_speed;
}

int	update_buttons_triggers(t_md *md, t_menu *menu, unsigned int c)
{
	int	i;

	i = -1;
	while (menu->buttons[++i].active)
	{
		if (c != menu->buttons[i].key_trigger)
			continue ;
		*menu->buttons[i].value = !(*menu->buttons[i].value);
		menu->refresh_ui = 1;
		menu->refresh_bg = 1;
		reset_fps_timer(&md->timer);
		render(md);
		play_sound(md, AU_MOUSE_CLICK);
		return (1);
	}
	return (0);
}

int	update_key_input(t_md *md, t_menu *menu, unsigned int c)
{
	if (update_buttons_triggers(md, menu, c))
		return (1);
	if (c == TAB_KEY)
		set_weapon_index(md);
	else if (c == ESC_KEY)
		set_menu_mode(md, &md->menu, !menu->active);
	else if (c == NUM_Q_KEY)
		free_and_quit(md, NULL, NULL);
	else if (c == SHIFT_KEY)
		md->timer.tm_walk = md->timer.cur_tm - 1;
	else if (c == NUM_C_KEY)
		plr_shoot(md);
	return (0);
}

static int	update_mouse_input(t_md *md)
{
	if (md->mouse.click != MOUSE_PRESS || !md->cam.pointed)
		return (0);
	if (md->cam.pointed->type == nt_door)
	{
		if (!md->hud.keys && md->cam.pointed->hp)
			return (play_sound(md, AU_MENU_OFF), 0);
		md->cam.pointed->hp = !md->cam.pointed->hp;
		if (!md->cam.pointed->hp)
			return (play_sound(md, AU_OPEN), md->hud.keys--, 1);
		return (play_sound(md, AU_CLOSE), md->hud.keys++, 1);
	}
	if (md->cam.pointed->type != nt_mob)
		return (0);
	paint_ent(md, md->cam.pointed, v2(0));
	play_sound(md, AU_PORTAL_SHOOT);
	md->cam.pointed->hp--;
	return (1);
}

void	update_input(t_md *md)
{
	if (md->key_clicked != -1)
		update_key_input(md, &md->menu, (unsigned int)md->key_clicked);
	if (md->menu.active)
		return ;
	update_arrow_rotation(md);
	update_mouse_input(md);
}
