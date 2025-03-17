
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:55:24 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/14 19:05:48 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

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
   				continue ;
			if ((src->src_data[src_index] & 0xFF000000) == 0)
				dst->src_data[dst_index] = src->src_data[src_index];
		}
	}
	return (1);
}

int	draw_pixel(t_image *texture, t_vec2 pos, int color, float transp)
{
	int	index;

	if (!texture || !texture->src_data)
		return (0);
	if (pos.x < 0 || pos.x >= texture->size.x || pos.y < 0 || pos.y >= texture->size.y)
		return (0);
	if ((texture->size_line / 4) != texture->size.x)
	{
		printf("Warning: size_line / 4 != size.x (%d != %d)\n",
			texture->size_line / 4, texture->size.x);
		exit(0);
		return (0);
	}
	index = pos.y * (texture->size_line / 4) + pos.x;
	if (transp == -1)
		texture->src_data[index] = color;
	else
		texture->src_data[index] = blend_color(texture->src_data[index], color, transp);
	return (1);
}

int	draw_pixels(t_image *txtr, t_vec2 pos, t_vec2 draw_size, int color)
{
	t_vec2	draw_pos;
	t_vec2	end_coord;
	int		draw_count;

	if (!txtr || !txtr->src_data || pos.x < 0 || pos.y < 0)
		return (0);
	end_coord = get_v2(pos.x + draw_size.x, pos.y + draw_size.y);
	draw_count = 0;
	draw_pos = pos;
	while (draw_pos.y <= end_coord.y)
	{
		draw_pos.x = pos.x;
		while (draw_pos.x <= end_coord.x)
		{
			draw_count += draw_pixel(txtr, draw_pos, color, -1);
			draw_pos.x++;
		}
		draw_pos.y++;
	}
	return (draw_count);
}

void	draw_transp_img(t_image *src, t_image *dst, t_vec2 pos, float trnsp)
{
	t_vec2	draw_pos;
	t_vec2	src_pos;
	t_vec2	dst_pos;
	t_vec2	pixel;
	int		blended;

	if (!src || !dst || !src->src_data || !dst->src_data)
		return ;
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
			pixel.x = src_pos.y * (src->size_line / 4) + src_pos.x;
			pixel.y = dst_pos.y * (dst->size_line / 4) + dst_pos.x;
			dst->src_data[pixel.y] = blend_color(src->src_data[pixel.x], dst->src_data[pixel.y], trnsp);
		}
	}
}

int	draw_trimmed_img(t_image *src, t_image *dst, t_vec2 pos, t_vec3 draw_end)
{
	t_vec2	draw_pos;
	t_vec2	src_pos;
	t_vec2	dst_pos;
	t_vec2	pxl_indexes;

	if (!src || !dst || !src->src_data || !dst->src_data)
		return (0);
	draw_pos = get_v2(-1, -1);
	while (++draw_pos.y < src->size.y)
	{
		src_pos.y = draw_pos.y;
		dst_pos.y = pos.y + draw_pos.y;
		if (dst_pos.y >= draw_end.y)
			break ;
		if (dst_pos.y < 0 || dst_pos.y >= dst->size.y)
			continue ;
		draw_pos.x = -1;
		while (++draw_pos.x < src->size.x)
		{
			src_pos.x = draw_pos.x;
			dst_pos.x = pos.x + draw_pos.x;
			if (dst_pos.x >= draw_end.x)
				break ;
			if (dst_pos.x < 0 || dst_pos.x >= dst->size.x)
				continue ;
			pxl_indexes.x = src_pos.y * (src->size_line / 4) + src_pos.x;
			pxl_indexes.y = dst_pos.y * (dst->size_line / 4) + dst_pos.x;
			if (draw_end.z != -1 && dst->src_data[pxl_indexes.y] != (unsigned int)draw_end.z)
   				continue ;
			if ((src->src_data[pxl_indexes.x] & 0xFF000000) == 0)
				dst->src_data[pxl_indexes.y] = src->src_data[pxl_indexes.x];
		}
	}
	return (1);
}

int	draw_sphere(t_image *txtr, t_vec2 pos, t_vec2 draw_size, int color)
{
	t_vec2	draw_pos;
	t_vec2	end_coord;
	int		draw_count;
	float	radius;
	float	center_x, center_y;
	float	distance, probability;

	if (!txtr || !txtr->src_data || pos.x < 0 || pos.y < 0)
		return (0);

	// Define the sphere radius (half of draw_size)
	radius = draw_size.x / 2.0f;
	center_x = pos.x + radius;
	center_y = pos.y + radius;

	end_coord = get_v2(pos.x + draw_size.x, pos.y + draw_size.y);
	draw_count = 0;
	draw_pos = pos;

	while (draw_pos.y <= end_coord.y)
	{
		draw_pos.x = pos.x;
		while (draw_pos.x <= end_coord.x)
		{
			// Compute distance from center
			distance = sqrtf((draw_pos.x - center_x) * (draw_pos.x - center_x) +
							 (draw_pos.y - center_y) * (draw_pos.y - center_y));

			// Normalize distance: 0 (center) to 1 (edge)
			probability = 1.0f - (distance / radius);

			// Add randomness: pixels closer to center have higher chance
			if (probability > 0 && r_range(0, 100) < probability * 100)
				draw_count += draw_pixel(txtr, draw_pos, color, -1);

			draw_pos.x++;
		}
		draw_pos.y++;
	}
	return (draw_count);
}
