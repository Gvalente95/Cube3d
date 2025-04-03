/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_fe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:24:01 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/03 14:32:01 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	reset_grass(t_md *md, t_fe *fe)
{
	fe->cut_len = fe->size.y + (fe->growth_factor * md->timer.fe_time);
	fe->age = 0;
}

int	render_fe(t_md *md, t_fe *fe, int width)
{
	float				shade;
	t_vec4				rgb;
	float				shading;
	t_vec2				pos;
	const t_vec2		draw_sz = (t_vec2){width, 1};

	pos = fe->pos;
	while (++pos.y < fe->end_y)
	{
		if (pos.y >= md->win_sz.y || pos.y < 0)
			continue ;
		shade = 0.5f + 0.5f * ((float)(fe->end_y - pos.y) / (float)fe->height);
		rgb = color_to_v4(fe->base_color);
		shading = shade * (1.0f - fe->fogalpha);
		rgb.r *= shading;
		rgb.g *= shading;
		rgb.b *= shading;
		rgb.r += fe->age * fe->growth_factor;
		rgb.b += (fe->age / 2) * fe->growth_factor;
		draw_pixels(md->screen, pos, draw_sz, \
			(rgb.r << 16) | (rgb.g << 8) | rgb.b);
	}
	if (rgb.r >= 220)
		return (reset_grass(md, fe), 0);
	return (fe->height);
}

void	draw_stored_fe(t_md *md)
{
	t_dblst	*node;
	t_fe	*f;

	node = dblst_first(md->env.stored_blades);
	while (node)
	{
		f = (t_fe *)node->content;
		render_fe(md, f, 1);
		node = node->next;
	}
	dblst_clear(&md->env.stored_blades, NULL);
	md->env.stored_blades = NULL;
}

void	update_fe(t_md *md, t_vec2 start, \
	t_fe *fe, t_floor_draw_d d)
{
	t_vec2		pos;
	int			height;

	height = fe->size.y + (fe->growth_factor * md->timer.fe_time) - fe->cut_len;
	if (height > fe->height_max)
	{
		fe->age = height - fe->height_max;
		height = fe->height_max;
	}
	height /= d.rwd;
	pos.x = start.x;
	pos.y = start.y - height;
	fe->pos = pos;
	fe->end_y = start.y;
	fe->fogalpha = d.fogalpha;
	fe->height = height;
	if (md->mouse.click && d.rwd < 1)
		reset_grass(md, fe);
}
