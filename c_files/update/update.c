/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:45:36 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/03 21:08:47 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

double	update_time(t_md *md, t_timer *tm)
{
	double	walk_increment;

	tm->cur_tm = get_time_in_seconds();
	md->timer.fps++;
	if (tm->cur_tm - tm->elapsed_pause >= 1)
	{
		tm->elapsed_pause = tm->cur_tm;
		tm->prv_fps = tm->fps;
		tm->fps = 0;
	}
	upd_timer(&tm->tm_fe, tm->cur_tm, .005 / md->prm.fe_speed, &tm->trig_fe);
	upd_timer(&tm->tm_anim, tm->cur_tm, ANIM_REFRESH, &tm->trig_anim);
	walk_increment = WALK_REFRESH;
	if (md->key_prs[SHIFT_KEY])
		walk_increment /= 2;
	upd_timer(&tm->tm_walk, tm->cur_tm, walk_increment, &tm->trig_walk);
	tm->delta_time = (tm->cur_tm - tm->prev_time);
	tm->prev_time = tm->cur_tm;
	tm->time++;
	md->timer.fe_time += md->prm.fe_speed;
	return (tm->delta_time);
}

void	reset_mapped_end(t_md *md, t_ent *e)
{
	if (e->map_index > 0 && e->map_index <= md->map.len - 1 && \
		md->mapped_ents[e->map_index] == e)
		md->mapped_ents[e->map_index] = NULL;
}

int	set_menu_mode(t_md *md, t_menu *menu, int mode)
{
	if (mode)
		play_sound(md, AU_MENU_IN);
	set_mouse_lock(md, !mode);
	if (md->is_linux)
	{
		if (mode)
			mlx_mouse_move(md->mlx, md->win, \
				md->win_sz.x / 2, md->win_sz.y / 2);
		else
			mlx_mouse_move(md->mlx, md->win, \
				md->mouse.prev.x, md->mouse.prev.y);
	}
	menu->active = mode;
	menu->slider_hov = -1;
	menu->button_hov = -1;
	menu->refresh_bg = mode;
	menu->refresh_ui = mode;
	return (mode);
}

void	update_audio(t_md *md, t_au_manager *au)
{
	play_loop(md, &au->mus_pid, AU_MUS, md->prm.au_on);
	play_loop(md, &au->wind_pid, AU_WIND, md->prm.fly_cam);
	if (!md->prm.au_on)
		return ;
	if (md->timer.trig_walk && !md->prm.fly_cam && md->plr.grounded && \
		!cmp_vec3f(md->input_mov, v3f(0), .01))
		play_random_sound(md, AU_WALK_GRASS, 8);
}

int	update_and_render(t_md *md)
{
	update_audio(md, &md->au);
	if (md->menu.active)
		return (update_menu(md, &md->menu));
	update_time(md, &md->timer);
	update_input(md);
	update_mouse(md);
	update_player(md, &md->plr);
	update_ents(md);
	render(md);
	reset_mlx_values(md);
	return (0);
}
