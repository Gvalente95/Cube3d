/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_fe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:24:01 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/23 01:47:26 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

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
	return (fe->height);
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
}

void	render_world_map(t_md *md, t_floor_draw_d d, int y_size)
{
	const t_vec2	draw_p = (t_vec2){d.win.x, d.win.y - y_size};
	char			c;
	int				map_i;
	int				clr;
	t_vec2			cord_transl;

	cord_transl = div_v2(d.txp, 8);
	if (cord_transl.x == md->plr.coord.x && cord_transl.y == md->plr.coord.y)
	{
		draw_pixels(md->screen, get_v2(d.win.x, d.win.y - y_size * 3), \
			get_v2(1, y_size * 2), _RED);
		clr = _GOLD;
		draw_pixels(md->screen, draw_p, get_v2(1, y_size), clr);
		return ;
	}
	map_i = cord_transl.y * (md->map.size.x + 1) + cord_transl.x;
	if (map_i < 0 || map_i > md->map.len)
		return ;
	draw_pixel(md->screen, get_v2(draw_p.x, draw_p.y + 10), _BLACK, .5f);
	c = md->map.buffer[map_i];
	if (c == '0' || c == ' ')
		return ;
	clr = md->rgb[get_char_index(md->txd.ents_tp_map[0], c)];
	draw_pixels(md->screen, get_v2(draw_p.x, draw_p.y), get_v2(1, y_size), clr);
}

int	update_and_render_fe(t_md *md, t_floor_draw_d d, t_fe **prv_fe)
{
	t_vec2				local;
	t_vec2f				rel;
	t_fe				*fe;
	const t_env_manager	*env = &md->env;

	if ((int)d.flr.x >= md->map.size.x - 1 || (int)d.flr.x < 0 || \
	(int)d.flr.y >= md->map.size.y - 1 || (int)d.flr.y < 0)
		return (1);
	rel.x = d.flr.x * (md->t_len / md->prm.grass_w);
	rel.y = d.flr.y * (md->t_len / md->prm.grass_w);
	local.x = minmax(0, safe_mod((int)rel.x, md->t_len), md->t_len - 1);
	local.y = minmax(0, safe_mod((int)rel.y, md->t_len), md->t_len - 1);
	fe = &env->grass[(int)d.flr.y][(int)d.flr.x][local.y][local.x];
	if (!fe->active)
		return (1);
	update_fe(md, d.win, fe, d);
	render_fe(md, fe, 1);
	return (*prv_fe = NULL, 1);
}
