/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_autocam.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 22:58:02 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/25 17:51:56 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	init_autocam(t_md *md, t_autocam *autocam)
{
	const t_vec2	center = div_v2(md->win_sz, 2);
	const float		map_w = minf(40, md->map.size.x) * md->t_len;
	const float		map_h = minf(40, md->map.size.y) * md->t_len;
	const float		map_diag = sqrtf(map_w * map_w + map_h * map_h);

	autocam->fade = 1;
	autocam->center = center;
	autocam->map_w = map_w;
	autocam->map_h = map_h;
	autocam->map_diag = map_diag;
	autocam->quitting = 0;
	autocam->acc_spd = 0.0f;
	autocam->base_y = 80 - md->map.size.y * .5f;
	md->plr.angle = M_PI_2;
	md->plr.pos.z = -md->t_len * minf(10, (md->map.size.y * .5f));
	md->prm.fly_cam = 1;
	md->prm.use_ceiling = 0;
	md->prm.ray_depth = md->t_len * md->map.size.y * 2;
	md->fx.fog = .01f;
	md->prm.super_view = 1;
	md->prm.alternate_draw = 1;
	md->prm.ray_mod = 2.5f;
	md->hud.floor_start = md->win_sz.y * .25;
	md->timer.time = 10;
	mlx_mouse_show(md->mlx, md->win);
}

static int	exit_autocam(t_md *md, t_autocam *autocam)
{
	md->plr.pos.x = md->plr.start_pos.x;
	md->plr.pos.y = md->plr.start_pos.y;
	md->plr.pos.z = md->plr.start_pos.z;
	md->plr.coord.x = md->plr.pos.x / md->t_len;
	md->plr.coord.y = md->plr.pos.y / md->t_len;
	md->plr.coord.z = md->plr.pos.z / md->t_len;
	md->cam.rot.y = 0;
	md->cam.rot.x = md->cam.x_dir_start;
	md->plr.dir.x = 0;
	md->plr.dir.y = 0;
	md->timer.time = 1;
	md->key_click = -1;
	md->hud.fog_color = _BLACK;
	md->prm.fe_speed = 1;
	md->prm.ent_mode = 1;
	md->prm.alternate_draw = 1;
	md->prm.fly_cam = 0;
	md->prm.ray_depth = md->t_len * RAY_DEPTH;
	md->prm.super_view = 0;
	md->prm.use_ceiling = 1;
	md->hud.fog_color = v4_to_color(10, 10, 10, 255);
	md->fx.fog = .3f;
	autocam->active = 0;
	autocam->quitting = 0;
	return (1);
}

void	update_player_orbit(t_md *md, t_autocam *aut)
{
	t_vec2f			delta;
	const float		amplitude = .3f;
	const float		spd = 5.0f;

	md->plr.angle += 0.005f;
	if (md->plr.angle > M_PI * 2)
		md->plr.angle -= M_PI * 2;
	md->plr.pos.x = aut->center.x + (aut->map_diag * 1) * cosf(md->plr.angle);
	md->plr.pos.y = aut->center.y + (aut->map_diag * 1) * sinf(md->plr.angle);
	delta.x = aut->center.x - md->plr.pos.x;
	delta.y = aut->center.y - md->plr.pos.y;
	md->cam.rot.z = 0;
	md->cam.rot.x = atan2f(delta.y, delta.x) * (180.0f / M_PI);
	md->cam.rot.y = aut->base_y + sinf(md->plr.angle * spd) * amplitude;
	md->plr.dir.x = cosf(md->plr.angle);
	md->plr.dir.y = sinf(md->plr.angle);
	update_cam(md, &md->cam);
	if (!md->mouse.delta.y)
		return ;
	aut->base_y = minmaxf(50, 80, aut->base_y + md->mouse.delta.y * .001f);
	md->fx.fog = minmaxf(0.001f, 1, md->fx.fog - md->mouse.delta.y * .00005f);
}

int	move_cam_to_start(t_md *md)
{
	const float		acspd = md->autocam.acc_spd;
	const float		mv_spd = .05f + acspd;
	const t_vec2f	rot_spd = (t_vec2f){0.05f + acspd, 0.01f + acspd * .5f};
	const t_vec3f	diff = sub_vec3f(md->plr.pos, md->plr.start_pos);

	md->prm.alternate_draw = 0;
	md->fx.fog = .1f;
	md->autocam.acc_spd += 0.003f;
	if (fabsf(md->plr.pos.x - md->plr.start_pos.x) > EPSILON)
		md->plr.pos.x += (md->plr.start_pos.x - md->plr.pos.x) * mv_spd;
	if (fabsf(md->plr.pos.y - md->plr.start_pos.y) > EPSILON)
		md->plr.pos.y += (md->plr.start_pos.y - md->plr.pos.y) * mv_spd;
	if (fabsf(md->plr.pos.z - md->plr.start_pos.z) > EPSILON)
		md->plr.pos.z += (md->plr.start_pos.z - md->plr.pos.z) * mv_spd;
	if (fabsf(md->cam.rot.x - md->cam.x_dir_start) > EPSILON)
		md->cam.rot.x += (md->cam.x_dir_start - md->cam.rot.x) * rot_spd.x;
	if (md->cam.rot.y > 0)
		md->cam.rot.y -= md->cam.rot.y * rot_spd.y;
	md->plr.angle = md->cam.rot.x * (M_PI / 180.0f);
	md->plr.dir = (t_vec3f){cosf(md->plr.angle), sinf(md->plr.angle), 0};
	md->hud.floor_start = md->win_sz.y / 2 - (md->cam.rot.y * 8) + 1;
	render_background(md);
	update_cam(md, &md->cam);
	return (fabsf(diff.x) < .1 && fabsf(diff.y) < .1 && fabsf(diff.z) < .1);
}

int	update_autocam(t_md *md, t_autocam *autocam)
{
	if (md->timer.time <= 1)
		init_autocam(md, autocam);
	if (md->key_click == Q_KEY)
		return (exit_autocam(md, autocam));
	else if (md->key_click == ESC_KEY)
		free_and_quit(md, NULL, NULL);
	if (!autocam->quitting && (md->key_click != -1 || md->mouse.click))
		autocam->quitting = 1;
	else if (autocam->quitting && move_cam_to_start(md))
		return (exit_autocam(md, autocam));
	else if (!autocam->quitting)
		update_player_orbit(md, autocam);
	play_loop(md, &md->au.mus_pid, AU_MUS, 1);
	play_loop(md, &md->au.wind_pid, AU_WIND, autocam->quitting);
	render_background(md);
	cast_ray_threads_lp(md);
	if (!autocam->quitting)
		render_autocam_text(md, 0);
	mlx_put_image_to_window(md->mlx, md->win, md->screen->img, 0, 0);
	reset_mlx_values(md);
	return (1);
}
