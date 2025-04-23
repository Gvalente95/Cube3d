/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_autocam_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:31:03 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/23 12:28:44 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	init_fade_intro(t_md *md, double duration)
{
	float	alpha;
	double	elapsed;

	elapsed = get_time_in_seconds() - md->timer.game_start;
	render_background(md);
	if (elapsed > duration)
	{
		md->autocam.fade = 0;
		return ;
	}
	alpha = 1.0f - (elapsed / duration);
	draw_alpha_img(md->hud.overlay, md->screen, v2(0), alpha);
}

void	render_part(t_md *md, t_vec2 base_p, t_txtd d, float t)
{
	d.scale = 15;
	d.x = base_p.x + 200;
	d.y = base_p.y + 80 * 1.3 + sinf(t) * 10;
	rnd_fast_txt(md, d, "By Gvalente");
	d.x = base_p.x - 50;
	d.scale = 20;
	d.y += sinf(t) * 5;
	d.y += 20;
	rnd_fast_txt(md, d, "Press any button to start");
}


void	render_autocam_text(t_md *md, float t)
{
	const char	*txt[] = {"C", "U", "B", "E", " ", "3", "D"};
	int			scale;
	t_vec2		base_p;
	t_txtd		d;
	int			i;

	scale = 80;
	base_p = (t_vec2){md->win_sz.x / 2 - (scale * 3), scale * 1.5};
	d = (t_txtd){base_p.x, base_p.y, md->hud.fog_color, scale, NULL};
	i = -1;
	while (++i < 7)
	{
		d.scale = scale;
		if (i >= 4)
			d.scale = 30;
		if (v2_touch((t_vec2){d.x, d.y}, v2(scale), md->mouse.real, v2(4)))
			d.scale += 10;
		d.color = -1;
		if (i % 2 != 0)
			d.color = md->hud.fog_color;
		t = (md->timer.time++) * 0.01f + i * 0.5f;
		d.y = base_p.y + sinf(t) * 10 + (i >= 4) * 30;
		rnd_fast_txt(md, d, txt[i]);
		d.x += d.scale;
	}
	render_part(md, base_p, d, t);
}
