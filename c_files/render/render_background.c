/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_background.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:45:20 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/16 23:08:50 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	draw_sky(t_md *md, t_image *bufr, t_image *sky, t_vec2	*sky_pos)
{
	int		i;
	int		drew_len;
	t_vec2	win_sz;
	t_vec3	trimm;

	drew_len = 0;
	win_sz = md->win_size;
	trimm = get_v3(md->win_size.x, md->hud.floor_start, md->hud.bgr_color);
	i = -1;
	while (++i < 6)
	{
		if (sky_pos[i].y < -win_sz.y)
			continue ;
		if (sky_pos[i].y > md->hud.floor_start)
			continue ;
		if (sky_pos[i].x < -md->win_size.x)
			continue ;
		if (sky_pos[i].x > md->win_size.x)
			continue ;
		draw_trimmed_img(sky, bufr, sky_pos[i], trimm);
		drew_len++;
	}
	return (drew_len);
}

static int	render_sky(t_md *md, t_image *bufr, t_image *sky)
{
	t_vec2	sky_pos[6];
	t_vec3f	speed;
	t_vec3f	offst;
	t_vec2	scrl;
	t_vec2	win_sz;

	speed = get_v3f(1, 1, 0);
	win_sz = md->win_size;
	offst.x = win_sz.x * speed.x;
	offst.y = win_sz.y * speed.y;
	scrl.x = fmod((((md->plr.rot.x + 180.0)) / 360.0) * offst.x, win_sz.x);
	scrl.y = fmod((((md->plr.rot.y + 90.0)) / 180.0) * offst.y, win_sz.y);
	sky_pos[0] = get_v2(-scrl.x, -scrl.y);
	sky_pos[1] = get_v2(-scrl.x, win_sz.y - scrl.y);
	sky_pos[2] = get_v2(win_sz.x - scrl.x, -scrl.y);
	sky_pos[3] = get_v2(win_sz.x - scrl.x, -win_sz.y - scrl.y);
	sky_pos[4] = get_v2(-scrl.x, -scrl.y + win_sz.y);
	sky_pos[5] = get_v2(win_sz.x - scrl.x, -scrl.y + win_sz.y);
	return (draw_sky(md, bufr, sky, sky_pos));
}

static int	draw_floor(t_md *md, t_image *bufr, t_image *floor, t_vec2	*floor_pos)
{
	int		i;
	int		drew_len;
	t_vec2	win_sz;

	drew_len = 0;
	win_sz = md->win_size;
	i = -1;
	while (++i < 9)
	{
		if (floor_pos[i].y < md->hud.floor_start - win_sz.y)
			continue ;
		if (floor_pos[i].y > win_sz.y)
			continue ;
		if (floor_pos[i].x < -md->win_size.x)
			continue ;
		if (floor_pos[i].x > md->win_size.x)
			continue ;
		draw_img(floor, bufr, floor_pos[i], md->hud.bgr_color);
		drew_len++;
	}
	return (drew_len);
}

static int	render_floor(t_md *md, t_image *bufr, t_image *floor)
{
	t_vec3f	speed;
	t_vec2	scroll;
	t_vec2	floor_pos[9];
	t_vec2	win_sz;

	win_sz = md->win_size;
	speed = get_v3f(10.0, 6.0, 0);
	scroll.x = fmod((md->plr.rot.x - md->input_offst.x) * speed.x, win_sz.x);
	scroll.y = fmod(\
	(-(md->plr.rot.y) + (md->input_offst.y * .2)) * speed.y, win_sz.y);
	floor_pos[0] = get_v2(-scroll.x, scroll.y);
	floor_pos[1] = get_v2(-scroll.x, scroll.y + win_sz.y);
	floor_pos[2] = get_v2(-scroll.x, scroll.y - win_sz.y);
	floor_pos[3] = get_v2(-scroll.x + win_sz.x, scroll.y);
	floor_pos[4] = get_v2(-scroll.x - win_sz.x, scroll.y);
	floor_pos[5] = get_v2(-scroll.x + win_sz.x, scroll.y - win_sz.y);
	floor_pos[6] = get_v2(-scroll.x + win_sz.x, scroll.y + win_sz.y);
	floor_pos[7] = get_v2(-scroll.x - win_sz.x, scroll.y + win_sz.y);
	floor_pos[8] = get_v2(-scroll.x - win_sz.x, scroll.y - win_sz.y);
	return (draw_floor(md, bufr, floor, floor_pos));
}

void	render_background(t_md *md)
{
	if (!md->ray_mode)
		draw_img(md->hud.floor, md->screen, get_v2(0, 0), md->hud.bgr_color);
	else if (!md->hud.active_background)
	{
		draw_trimmed_img(md->hud.base_sky, md->screen, get_v2(0, 0), \
		get_v3(md->win_size.x, md->hud.floor_start, md->hud.bgr_color));
		draw_img(md->hud.base_floor, md->screen, \
			get_v2(0, md->hud.floor_start), md->hud.bgr_color);
	}
	else
	{
		render_sky(md, md->screen, md->hud.sky);
		render_floor(md, md->screen, md->hud.floor);
	}
}
