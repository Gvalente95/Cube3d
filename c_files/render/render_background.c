/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_background.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:45:20 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/15 02:27:06 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"


void	render_sky(t_md *md, t_image *bufr, t_image *sky, int bgr_clr)
{
	t_vec3f	scroll_speed;
	t_vec3f	offst;
	t_vec2	scrl;
	t_vec2	win_sz;

	scroll_speed = get_v3f(1, 1, 0);
	win_sz = md->win_size;
	offst.x = win_sz.x * scroll_speed.x;
	offst.y = win_sz.y * scroll_speed.y;
	scrl.x = fmod((((md->plr.rot.x + 180.0)) / 360.0) * offst.x, win_sz.x);
	scrl.y = fmod((((md->plr.rot.y + 90.0)) / 180.0) * offst.y, win_sz.y * 2);
	draw_img(sky, bufr, get_v2(-scrl.x, -scrl.y), bgr_clr);
	draw_img(sky, bufr, get_v2(-scrl.x, win_sz.y - scrl.y), bgr_clr);
	draw_img(sky, bufr, get_v2(win_sz.x - scrl.x, -scrl.y), bgr_clr);
	draw_img(sky, bufr, get_v2(win_sz.x - scrl.x, win_sz.y - scrl.y), bgr_clr);
}

void	render_floor(t_md *md, t_image *bufr, t_image	*floor, int bgr_clr)
{
	t_vec3f	scroll_speed;
	t_vec2	scroll;
	float	offset;
	t_vec2	win_size;

	scroll.y = md->floor_start;
	win_size = md->win_size;
	scroll_speed = get_v3f(5.0, 5.0, 0);
	offset = win_size.x * scroll_speed.x - md->wrd_mv_offst.x;
	scroll.x = fmod((((md->plr.rot.x + 180.0)) / 360.0) * offset, win_size.x);
	draw_img(floor, bufr, get_v2(-scroll.x, scroll.y), bgr_clr);
	draw_img(floor, bufr, get_v2(scroll.x, scroll.y), bgr_clr);
	draw_img(floor, bufr, get_v2(win_size.x, scroll.y), bgr_clr);
	draw_img(floor, bufr, get_v2(win_size.x - scroll.x, scroll.y), bgr_clr);
}

void	render_background(t_md *md)
{
	if (!md->ray_mode)
		draw_img(md->hud.floor, md->screen, get_v2(0, 0), md->rgb[2]);
	else
	{
		render_floor(md, md->screen, md->hud.floor, md->rgb[2]);
		render_sky(md, md->screen, md->hud.sky, md->rgb[2]);
	}
}
