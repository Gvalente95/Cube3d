/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:45:36 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/19 22:47:18 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

double	update_time(t_md *md, t_timer *tm)
{
	if (md->prm.cap_fps < 100)
		cap_fps(&md->timer, 1.0f / md->prm.cap_fps);
	tm->cur_tm = get_time_in_seconds();
	md->timer.fps++;
	if (md->timer.fps > 50)
		reset_fps_timer(tm);
	tm->avrg_fps = (float)(++tm->frm_cnt) / (tm->cur_tm - tm->fps_tm);
	if (tm->cur_tm - tm->elapsed_pause >= 1)
	{
		tm->elapsed_pause = tm->cur_tm;
		tm->prv_fps = tm->fps;
		tm->fps = 0;
	}
	upd_timer(&tm->tm_fe, tm->cur_tm, .005 / md->prm.fe_speed, &tm->trig_fe);
	upd_timer(&tm->tm_anim, tm->cur_tm, ANIM_REFRESH, &tm->trig_anim);
	upd_timer(&tm->tm_walk, tm->cur_tm, \
		WALK_REFRESH / (1 + (md->key_prs[SHIFT_KEY] == 1)), &tm->trig_walk);
	tm->delta_time = (tm->cur_tm - tm->prev_time);
	tm->prev_time = tm->cur_tm;
	tm->time++;
	md->timer.fe_time += md->prm.fe_speed;
	return (tm->delta_time);
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
	play_loop(md, &au->mus_pid, AU_MUS, 1);
	play_loop(md, &au->wind_pid, AU_WIND, !md->menu.active && md->prm.fly_cam);
	if (!md->prm.au_on)
		return ;
	if (md->cam.pos.z + md->prm.height < -.5)
		return ;
	if (md->timer.trig_walk && !md->prm.fly_cam && md->plr.grounded && \
		md->cam.is_moving && !cmp_vec3f(md->plr.mov, v3f(0), .01))
		md->au.walk_index = \
			play_rand_sound(md, AU_WALK_GRASS, 8, md->au.walk_index);
}

static void	update_portals(t_md *md, t_ent *e, t_vec2 out_pos)
{
	t_portal	*p;
	int			index;

	p = &md->portal;
	if (!md->portal.found)
		return ;
	if (!p->ends[0].e)
		p->ends[0].e = e;
	else if (!p->ends[1].e)
		p->ends[1].e = e;
	else
	{
		index = p->last_shot_index;
		p = &md->portal;
		free_image_data(md, p->ends[index].e->overlay);
		p->ends[index].e->overlay = NULL;
		p->ends[index].e = e;
		p->ends[index].dir = e->overlay_dir;
		p->ends[index].out = out_pos;
		p->last_shot_index = !p->last_shot_index;
	}
	p->found = NULL;
}

int	update_and_render(t_md *md)
{
	if (md->autocam.active)
		return (update_autocam(md, &md->autocam));
	update_portals(md, md->portal.found, md->portal.out_pos);
	update_audio(md, &md->au);
	if (md->menu.active)
		return (update_menu(md, &md->menu));
	update_time(md, &md->timer);
	update_input(md);
	update_mouse(md);
	if (md->timer.time > 5)
		update_player(md, &md->plr);
	update_ents(md);
	render(md);
	reset_mlx_values(md);
	return (0);
}
