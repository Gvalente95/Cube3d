/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:55:24 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/14 03:13:35 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

void	flush_img(t_image *src, int color)
{
	int		*img_data;
	int		total_pixels;
	int		i;

	if (!src || !src->src_data)
		return ;
	if (color == -1)
		color = 0x000000;
	img_data = src->src_data;
	total_pixels = src->size.x * src->size.y;
	i = -1;
	while (++i < total_pixels)
		img_data[i] = color;
}

int	draw_img(t_image *src, t_image *dst, t_vec2 pos, int overlap_color)
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
		dst_pos.y = pos.y + draw_pos.y;
		if (dst_pos.y < 0 || dst_pos.y >= dst->size.y)
			continue ;
		draw_pos.x = -1;
		while (++draw_pos.x < src->size.x)
		{
			src_pos.x = draw_pos.x;
			dst_pos.x = pos.x + draw_pos.x;
			if (dst_pos.x < 0 || dst_pos.x >= dst->size.x)
				continue ;
			src_index = src_pos.y * (src->size_line / 4) + src_pos.x;
			dst_index = dst_pos.y * (dst->size_line / 4) + dst_pos.x;
			if (overlap_color != -1 && dst->src_data[dst_index] != (unsigned int)overlap_color)
   				continue;
			if ((src->src_data[src_index] & 0xFF000000) == 0)
				dst->src_data[dst_index] = src->src_data[src_index];
		}
	}
}

int	draw_pixel(t_image *texture, int x, int y, int color)
{
	int	index;

	if (!texture || !texture->src_data)
		return (0);
	if (x < 0 || x >= texture->size.x || y < 0 || y >= texture->size.y)
		return (0);
	if ((texture->size_line / 4) != texture->size.x)
	{
		printf("Warning: size_line / 4 != size.x (%d != %d)\n",
			texture->size_line / 4, texture->size.x);
		exit(0);
		return (0);
	}
	index = y * (texture->size_line / 4) + x;
	texture->src_data[index] = color;
	return (1);
}

int	draw_pixels(t_image *txtr, t_vec2 start_coord, t_vec2 end_coord, int color)
{
	t_vec2	draw_pos;
	int		draw_count;

	if (!txtr || !txtr->src_data)
		return (0);
	draw_count = 0;
	draw_pos = start_coord;
	while (draw_pos.y <= end_coord.y)
	{
		draw_pos.x = start_coord.x;
		while (draw_pos.x <= end_coord.x)
			draw_count += draw_pixel(txtr, draw_pos.x++, draw_pos.y, color);
		draw_pos.y++;
	}
	return (draw_count);
}
