/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 12:51:08 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/16 23:20:30 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void apply_fxaa(t_image *img, float edge_threshold, float blend_factor)
{
	int x, y;
	int width = img->size.x;
	int height = img->size.y;
	unsigned int *new_data = malloc(width * height * sizeof(unsigned int));

	if (!new_data)
		return ;
	for (y = 1; y < height - 1; y++)
	{
		for (x = 1; x < width - 1; x++)
		{
			int idx = y * width + x;
			unsigned int center = img->src_data[idx];
			unsigned int left = img->src_data[idx - 1];
			unsigned int right = img->src_data[idx + 1];
			unsigned int top = img->src_data[idx - width];
			unsigned int bottom = img->src_data[idx + width];
			float lum_left = (((left >> 16) & 0xFF) * 0.299) + 
							 (((left >> 8) & 0xFF) * 0.587) + 
							 ((left & 0xFF) * 0.114);
			float lum_right = (((right >> 16) & 0xFF) * 0.299) + 
							  (((right >> 8) & 0xFF) * 0.587) + 
							  ((right & 0xFF) * 0.114);
			float lum_top = (((top >> 16) & 0xFF) * 0.299) + 
							(((top >> 8) & 0xFF) * 0.587) + 
							((top & 0xFF) * 0.114);
			float lum_bottom = (((bottom >> 16) & 0xFF) * 0.299) + 
							   (((bottom >> 8) & 0xFF) * 0.587) + 
							   ((bottom & 0xFF) * 0.114);
			float edge_strength = fabs(lum_right - lum_left) + fabs(lum_bottom - lum_top);
			if (edge_strength > edge_threshold)
			{
				int r = (((center >> 16) & 0xFF) * (1 - blend_factor)) +
						((((left >> 16) & 0xFF) + ((right >> 16) & 0xFF)) / 2 * blend_factor);
				int g = (((center >> 8) & 0xFF) * (1 - blend_factor)) +
						((((left >> 8) & 0xFF) + ((right >> 8) & 0xFF)) / 2 * blend_factor);
				int b = ((center & 0xFF) * (1 - blend_factor)) +
						((((left & 0xFF) + (right & 0xFF)) / 2) * blend_factor);

				new_data[idx] = (0xFF << 24) | (r << 16) | (g << 8) | b;
			}
			else
				new_data[idx] = center;
		}
	}
	ft_memcpy(img->src_data, new_data, width * height * sizeof(unsigned int));
	free(new_data);
}

void	shift_rgb(t_image *img, t_vec4f rgb_factors)
{
	int				num_pixels;
	t_vec4			rgba;
	unsigned int	color;
	int				i;

	num_pixels = img->size.x * img->size.y;
	i = -1;
	while (++i < num_pixels)
	{
		color = img->src_data[i];
		rgba.r = ((color >> 16) & 0xFF) * rgb_factors.r;
		rgba.g = ((color >> 8) & 0xFF) * rgb_factors.g;
		rgba.b = (color & 0xFF) * rgb_factors.b;
		rgba.a = (color >> 24) & 0xFF;
		rgba.r = minmaxf(0, 255, rgba.r);
		rgba.g = minmaxf(0, 255, rgba.g);
		rgba.b = minmaxf(0, 255, rgba.b);
		color = (rgba.a << 24) | (rgba.r << 16) | (rgba.g << 8) | rgba.b;
		img->src_data[i] = color;
	}
}

void	apply_antialiasing(t_image *img)
{
	t_vec2			pos;
	t_vec2			ij;
	int				half_kernel;
	unsigned int	*new_data;
	t_vec3			n;
	t_vec4			rgba;
	t_vec2			size;
	int				len;

	size = img->size;
	half_kernel = 3 / 2;
	new_data = malloc(size.x * size.y * sizeof(unsigned int));
	if (!new_data)
		return ;
	pos = get_v2(-1, -1);
	while (++pos.y < size.y)
	{
		pos.x = -1;
		while (++pos.x < size.x)
		{
			rgba = get_v4(0, 0, 0, 0);
			len = 0;
			ij.x = -half_kernel - 1;
			while (++ij.x <= half_kernel)
			{
				ij.y = -half_kernel -1;
				while (++ij.y <= half_kernel)
				{
					n = get_v3(pos.x + ij.x, pos.y + ij.y, 0);
					if (n.x >= 0 && n.x < size.x && n.y >= 0 && n.y < size.y)
					{
						n.z = img->src_data[n.y * size.x + n.x];
						rgba.r += (n.z >> 16) & 0xFF;
						rgba.g += (n.z >> 8) & 0xFF;
						rgba.b += n.z & 0xFF;
						rgba.a += (n.z >> 24) & 0xFF;
						len++;
					}
				}
			}
			new_data[pos.y * size.x + pos.x] = \
	((rgba.a / len) << 24) | ((rgba.r / len) << 16) | ((rgba.g / len) << 8) | (rgba.b / len);
		}
	}
	ft_memcpy(img->src_data, new_data, size.x * size.y * sizeof(unsigned int));
	free(new_data);
}
