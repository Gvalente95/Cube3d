/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:45:36 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/01 16:52:47 by giuliovalen      ###   ########.fr       */
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
	upd_timer(&tm->tm_fe, tm->cur_tm, .01 / md->prm.fe_speed, &tm->trig_fe);
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

static void	update_camera_offset(t_md *md)
{
	t_vec3f	dspl;
	t_vec2	win_sz;
	t_vec2	plr_size;
	t_vec3f	plr_pos;

	win_sz = md->win_sz;
	plr_size = md->plr.size;
	plr_pos = md->plr.pos;
	dspl.x = plr_pos.x - plr_size.x / 2 - win_sz.x / 2 + md->t_len / 2;
	dspl.y = plr_pos.y - plr_size.y / 2 - win_sz.y / 2 + md->t_len / 2;
	dspl.z = plr_pos.z;
	md->cam_ofst = dspl;
	md->wrd_mv_offst.x += md->plr_wrd_mv.x * 20;
	md->wrd_mv_offst.y += md->plr_wrd_mv.y * 20;
}

int	update_and_render(t_md *md)
{
	play_loop(md, &md->au.mus_pid, AU_MUS, !md->menu.active);
	if (md->menu.active)
		return (update_menu(md, &md->menu));
	update_time(md, &md->timer);
	update_input(md);
	update_mouse(md);
	update_player(md, &md->plr);
	update_camera_offset(md);
	update_ents(md);
	render(md);
	reset_mlx_values(md);
	return (0);
}
