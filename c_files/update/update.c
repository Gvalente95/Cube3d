/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:45:36 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 15:42:04 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

double	update_time(t_md *md, t_timer *timer)
{
	timer->current_time = get_time_in_seconds();
	md->timer.fps++;
	if (timer->current_time - md->timer.elapsed_pause >= 1)
	{
		md->timer.elapsed_pause = timer->current_time;
		md->timer.prv_fps = md->timer.fps;
		md->timer.fps = 0;
	}
	md->update_frames = 0;
	if (md->timer.anim_timer < md->timer.current_time)
	{
		md->timer.anim_timer = md->timer.current_time + ANIM_REFRESH;
		md->update_frames = 1;
	}
	timer->delta_time = (timer->current_time - timer->prev_time);
	timer->prev_time = timer->current_time;
	md->timer.time++;
	return (timer->delta_time);
}

int	set_menu_mode(t_md *md, t_menu *menu, int mode)
{
	set_mouse_lock(md, !mode);
	if (mode)
		mlx_mouse_move(md->mlx, md->win, md->win_size.x / 2, md->win_size.y / 2);
	else
		mlx_mouse_move(md->mlx, md->win, md->mouse.prev.x, md->mouse.prev.y);
	menu->active = mode;
	if (menu->freeze_frame)
		free_image_data(md, menu->freeze_frame);
	menu->freeze_frame = NULL;
	menu->refresh = 1;
	return (mode);
}

static void	update_camera_offset(t_md *md)
{
	t_vec3f	dspl;
	t_vec2	win_size;
	t_vec2	plr_size;
	t_vec3f	plr_pos;

	win_size = md->win_size;
	plr_size = md->plr.size;
	plr_pos = md->plr.pos;
	dspl.x = plr_pos.x - plr_size.x / 2 - win_size.x / 2 + md->t_len / 2;
	dspl.y = plr_pos.y - plr_size.y / 2 - win_size.y / 2 + md->t_len / 2;
	dspl.z = plr_pos.z;
	md->cam_ofst = dspl;
	md->wrd_mv_offst.x += md->plr_wrd_mv.x * 20;
	md->wrd_mv_offst.y += md->plr_wrd_mv.y * 20;
}

int	update_and_render(t_md *md)
{
	if (md->menu.active)
		return (update_menu(md, &md->menu));
	update_input(md);
	update_mouse(md);
	update_player(md, &md->plr);
	update_camera_offset(md);
	if (md->prm.ent_mode)
		update_ents(md);
	render(md);
	reset_mlx_values(md);
	update_time(md, &md->timer);
	return (0);
}
