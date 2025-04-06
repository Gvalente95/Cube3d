/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_rot.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:52:38 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/04 10:24:37 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

void	draw_rot_z(t_md *md, t_image *from, t_image *onto, t_vec3f pos)
{
	const t_vec2	src_sz = (t_vec2){from->size.x, from->size.y};
	t_vec2f			scale;
	t_vec2			d_pos;
	t_vec2			sc_pos;
	t_vec3			draw_d;

	if (!from || !onto || !from->src || !onto->src)
		return ;
	scale.x = maxf(0.05f, cosf(pos.z * (M_PI / 180.0f)));
	d_pos.x = -1;
	while (++d_pos.x < src_sz.x)
	{
		sc_pos.x = (int)((float)d_pos.x * scale.x);
		d_pos.y = -1;
		while (++d_pos.y < src_sz.y)
		{
			draw_d.z = from->src[d_pos.y * src_sz.x + d_pos.x];
			sc_pos.y = d_pos.y;
			draw_d.x = pos.x + sc_pos.x - (src_sz.x * scale.x / 2);
			draw_d.y = pos.y + sc_pos.y - (src_sz.y / 2);
			draw_d.y += (int)((d_pos.x - src_sz.x / 2) * \
				cosf(pos.x * (M_PI / 180.0f)) * 0.5f);
			draw_pixel(onto, get_v2(draw_d.x, draw_d.y), draw_d.z, -1);
		}
	}
}

void	draw_rot_y(t_md *md, t_image *from, t_image *onto, t_vec3f pos)
{
	const t_vec2	src_sz = (t_vec2){from->size.x, from->size.y};
	t_vec2f			scale;
	t_vec2			d_pos;
	t_vec2			sc_pos;
	t_vec3			draw_d;

	if (!from || !onto || !from->src || !onto->src)
		return ;
	scale.y = maxf(0.05f, cosf(pos.z * (M_PI / 180.0f)));
	d_pos.y = -1;
	while (++d_pos.y < src_sz.y)
	{
		sc_pos.y = (int)((float)d_pos.y * scale.y);
		d_pos.x = -1;
		while (++d_pos.x < src_sz.x)
		{
			draw_d.z = from->src[d_pos.y * src_sz.x + d_pos.x];
			sc_pos.x = d_pos.x;
			draw_d.x = pos.x + sc_pos.x - (src_sz.x / 2);
			draw_d.y = pos.y + sc_pos.y - (src_sz.y * scale.y / 2);
			draw_pixel(onto, get_v2(draw_d.x, draw_d.y), draw_d.z, -1);
		}
	}
}

void	draw_rot_yz(t_md *md, t_image *from, t_image *onto, t_vec4f pos)
{
	const t_vec2	src_sz = (t_vec2){from->size.x, from->size.y};
	t_vec2f			scale;
	t_vec2			d_pos;
	t_vec2			sc_pos;
	t_vec3			draw_d;

	if (!from || !onto || !from->src || !onto->src)
		return ;
	scale.x = maxf(0.05f, cosf(pos.b * (M_PI / 180.0f)));
	scale.y = maxf(0.05f, cosf(pos.a * (M_PI / 180.0f)));
	d_pos.y = -1;
	while (++d_pos.y < src_sz.y)
	{
		sc_pos.y = (int)((float)d_pos.y * scale.y);
		d_pos.x = -1;
		while (++d_pos.x < src_sz.x)
		{
			draw_d.z = from->src[d_pos.y * src_sz.x + d_pos.x];
			sc_pos.x = (int)((float)d_pos.x * scale.x);
			draw_d.x = pos.r + sc_pos.x - (src_sz.x * scale.x / 2);
			draw_d.y = pos.g + sc_pos.y - (src_sz.y * scale.y / 2);
			draw_pixel(onto, get_v2(draw_d.x, draw_d.y), draw_d.z, -1);
		}
	}
}
