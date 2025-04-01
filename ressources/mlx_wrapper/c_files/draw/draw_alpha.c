/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_alpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:51:34 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/29 12:39:52 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	set_alpha(int color, float new_alpha)
{
	int	alpha;

	if (new_alpha < 0.0f)
		new_alpha = 0.0f;
	if (new_alpha > 1.0f)
		new_alpha = 1.0f;
	new_alpha = 1 - new_alpha;
	alpha = (int)(new_alpha * 255.0f);
	color = (color & 0x00FFFFFF) | (alpha << 24);
	return (color);
}

float	get_alpha(int color)
{
	int	alpha;

	alpha = (color >> 24) & 0xFF;
	return ((float)alpha / 255.0f);
}

void	fill_transparency(t_image *src, int color)
{
	int		*img_data;
	int		total_pixels;
	int		i;

	if (!src || !src->src)
		return ;
	if (color == -1)
		color = 0x000000;
	img_data = src->src;
	total_pixels = src->size.x * src->size.y;
	i = -1;
	while (++i < total_pixels)
	{
		if ((img_data[i] & 0xFF000000) == 0x00000000)
			continue ;
		img_data[i] = color;
	}
}

void	draw_alpha_img(t_image *src, t_image *dst, t_vec2 pos, float trnsp)
{
	t_draw_d	draw_d;
	t_vec2		draw_pos;

	if (!src || !dst || !src->src || !dst->src)
		return ;
	draw_d.src = src;
	draw_d.dst = dst;
	draw_pos = get_v2(-1, -1);
	while (++draw_pos.y < src->size.y)
	{
		draw_d.src_pos.y = draw_pos.y;
		draw_d.dst_pos.y = pos.y + draw_pos.y;
		if (draw_d.dst_pos.y < 0 || draw_d.dst_pos.y >= dst->size.y)
			continue ;
		draw_pos.x = -1;
		while (++draw_pos.x < src->size.x)
		{
			draw_d.src_pos.x = draw_pos.x;
			draw_d.dst_pos.x = pos.x + draw_pos.x;
			put_pxl_if_vis(&draw_d, -1, 1, trnsp);
		}
	}
}

void	init_draw_d(t_draw_d *draw_d, t_image *src, t_image *dst, \
	t_vec2 src_pos)
{
	draw_d->src = src;
	draw_d->dst = dst;
	draw_d->src_pos = src_pos;
}
