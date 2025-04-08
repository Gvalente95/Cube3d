/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_autocam.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 22:58:02 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/07 20:59:53 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	init_autocam(t_md *md, t_autocam *autocam)
{
	const t_vec2	center = {
		md->map.size.x * md->t_len * 0.5f,
		md->map.size.y * md->t_len * 0.5f
	};
	const float		map_w = minf(40, md->map.size.x) * md->t_len;
	const float		map_h = minf(40, md->map.size.y) * md->t_len;
	const float		map_diag = sqrtf(map_w * map_w + map_h * map_h);

	autocam->fade = 1;
	autocam->center = center;
	autocam->map_w = map_w;
	autocam->map_h = map_h;
	autocam->map_diag = map_diag;
	autocam->quitting = 0;
	md->plr.angle = M_PI_2;
	md->plr.pos.z = -md->t_len * minf(10, (md->map.size.y * .6));
	md->prm.fly_cam = 1;
	md->prm.use_ceiling = 0;
	md->prm.ray_depth = md->t_len * md->map.size.y * 2;
	md->cam.rot.y = 55;
	md->fx.fog = maxf(.3, 10 / md->map.size.y);
	md->prm.super_view = 1;
	md->hud.floor_start = md->win_sz.y * .1;
	md->timer.time = 10;
}

static int	exit_autocam(t_md *md, t_autocam *autocam)
{
	md->plr.pos.x = md->plr.start_pos.x;
	md->plr.pos.y = md->plr.start_pos.y;
	md->plr.pos.z = md->plr.start_pos.z;
	md->plr.coord.x = md->plr.pos.x / md->t_len;
	md->plr.coord.y = md->plr.pos.y / md->t_len;
	md->plr.coord.z = md->plr.pos.z / md->t_len;
	md->prm.fly_cam = 0;
	md->prm.ray_depth = md->t_len * RAY_DEPTH;
	md->prm.super_view = 0;
	md->cam.rot.y = 0;
	md->cam.rot.x = md->cam.x_dir_start;
	md->plr.dir.x = 0;
	md->plr.dir.y = 0;
	md->timer.time = 1;
	md->key_clicked = -1;
	autocam->active = 0;
	autocam->quitting = 0;
	return (1);
}

void	update_player_orbit(t_md *md, t_autocam *aut)
{
	t_vec2f			delta;

	md->plr.angle += 0.005f;
	if (md->plr.angle > M_PI * 2)
		md->plr.angle -= M_PI * 2;
	md->plr.pos.x = aut->center.x + (aut->map_diag * 1) * cosf(md->plr.angle);
	md->plr.pos.y = aut->center.y + (aut->map_diag * 1) * sinf(md->plr.angle);
	delta.x = aut->center.x - md->plr.pos.x;
	delta.y = aut->center.y - md->plr.pos.y;
	md->cam.rot.x = atan2f(delta.y, delta.x) * (180.0f / M_PI);
	md->cam.rot.z = 0;
	md->plr.dir.x = cosf(md->plr.angle);
	md->plr.dir.y = sinf(md->plr.angle);
	update_cam(md, &md->cam);
}

int	move_cam_to_start(t_md *md)
{
	if (fabsf(md->plr.pos.x - md->plr.start_pos.x) > EPSILON)
		md->plr.pos.x += (md->plr.start_pos.x - md->plr.pos.x) * .1;
	if (fabsf(md->plr.pos.y - md->plr.start_pos.y) > EPSILON)
		md->plr.pos.y += (md->plr.start_pos.y - md->plr.pos.y) * .1;
	if (fabsf(md->plr.pos.z - md->plr.start_pos.z) > EPSILON)
		md->plr.pos.z += (md->plr.start_pos.z - md->plr.pos.z) * .1;
	if (fabsf(md->cam.rot.x - md->cam.x_dir_start) > EPSILON)
		md->cam.rot.x += (md->cam.x_dir_start - md->cam.rot.x) * 0.1f;
	else
		md->cam.rot.x = md->cam.x_dir_start;
	if (md->cam.rot.y > 0)
		md->cam.rot.y -= md->cam.rot.y * 0.07f;
	else
		md->cam.rot.y = 0;
	md->plr.angle = md->cam.rot.x * (M_PI / 180.0f);
	md->plr.dir.x = cosf(md->plr.angle);
	md->plr.dir.y = sinf(md->plr.angle);
	md->hud.floor_start = md->win_sz.y / 2 - (md->cam.rot.y * 8) + 1;
	render_background(md);
	update_cam(md, &md->cam);
	if (fabsf(md->plr.pos.x - md->plr.start_pos.x) < 1 && \
		fabsf(md->plr.pos.y - md->plr.start_pos.y) < 1 && \
		fabsf(md->plr.pos.z - md->plr.start_pos.z) < 1)
		return (1);
	return (0);
}

int	update_autocam(t_md *md, t_autocam *autocam)
{
	if (md->timer.time <= 1)
		init_autocam(md, autocam);
	if (md->key_clicked == ESC_KEY)
		return (exit_autocam(md, autocam));
	if (md->key_clicked != -1)
		autocam->quitting = 1;
	else if (autocam->quitting && move_cam_to_start(md))
		return (exit_autocam(md, autocam));
	else if (!autocam->quitting)
		update_player_orbit(md, autocam);
	play_loop(md, &md->au.mus_pid, AU_MUS, !autocam->quitting);
	play_loop(md, &md->au.wind_pid, AU_WIND, autocam->quitting);
	cast_ray_threads_lp(md);
	if (!autocam->quitting)
		render_autocam_text(md, 0);
	if (autocam->fade)
		init_fade_intro(md, 2.0f);
	mlx_put_image_to_window(md->mlx, md->win, md->screen->img, 0, 0);
	reset_mlx_values(md);
	return (1);
}
