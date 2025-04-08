/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_autocam_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:31:03 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/07 18:32:26 by giuliovalen      ###   ########.fr       */
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

void	render_autocam_text(t_md *md, float t)
{
	const char	*txt[] = {"C", "U", "B", "E", " ", "3", "D"};
	int			scale;
	t_vec2		base_p;
	t_txtd		d;
	int			i;

	scale = 80;
	base_p = (t_vec2){md->win_sz.x / 2 - (scale * 3), scale * 1.5};
	d = (t_txtd){base_p.x, base_p.y, md->hud.sky_color, scale, NULL};
	i = -1;
	while (++i < 7)
	{
		if (i == 4)
			d.scale = 30;
		t = (md->timer.time++) * 0.01f + i * 0.5f;
		d.y = base_p.y + sinf(t) * 10 + (i >= 4) * 30;
		rnd_fast_txt(md, d, txt[i]);
		d.x += d.scale;
	}
	d.x = base_p.x - 50;
	d.y = base_p.y + scale * 1.3 + sinf(t) * 5;
	d.scale = 20;
	rnd_fast_txt(md, d, "Press any button to start");
}
