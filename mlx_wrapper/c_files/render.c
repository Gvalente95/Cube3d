/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RENDER.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:55:24 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/13 20:17:46 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

void	flush_img(t_image *src)
{
	int		*img_data;
	int		total_pixels;
	int		i;

	if (!src || !src->src_data)
		return ;
	img_data = src->src_data;
	total_pixels = src->size.x * src->size.y;
	i = -1;
	while (++i < total_pixels)
		img_data[i] = 0x000000;
}

int	draw_img(t_image *src, t_image *dst, int x, int y)
{
	t_vec2	draw_pos;
	t_vec2	src_pos;
	t_vec2	dst_pos;
	int		dst_index;
	int		src_index;

	if (!src || !dst || !src->src_data || !dst->src_data)
		return (0);
	draw_pos = get_v2(-1, -1);
	while (++draw_pos.y < src->size.y)
	{
		src_pos.y = draw_pos.y;
		dst_pos.y = y + draw_pos.y;
		if (dst_pos.y < 0 || dst_pos.y >= dst->size.y)
			continue ;
		draw_pos.x = -1;
		while (++draw_pos.x < src->size.x)
		{
			src_pos.x = draw_pos.x;
			dst_pos.x = x + draw_pos.x;
			if (dst_pos.x < 0 || dst_pos.x >= dst->size.x)
				continue ;
			src_index = src_pos.y * (src->size_line / 4) + src_pos.x;
			dst_index = dst_pos.y * (dst->size_line / 4) + dst_pos.x;
			if ((src->src_data[src_index] & 0xFF000000) != 0)
				dst->src_data[dst_index] = src->src_data[src_index];
		}
	}
}

int	draw_pixel(t_image *src, int x, int y, int color)
{
	int index;

	if (!src || !src->src_data)
		return (0);
	if (x < 0 || x >= src->size.x || y < 0 || y >= src->size.y)
		return (0);
	index = y * (src->size_line / 4) + x;
	src->src_data[index] = color;
	return (1);
}
