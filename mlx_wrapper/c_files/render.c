/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:55:24 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/16 22:43:08 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

int	blend_color(int color_a, int color_b, float factor)
{
	t_vec4	rgb_a;
	t_vec4	rgb_b;
	t_vec4	blend;
	int		result;

	rgb_a.a = (color_a >> 24) & 0xFF;
	rgb_a.r = (color_a >> 16) & 0xFF;
	rgb_a.g = (color_a >> 8)  & 0xFF;
	rgb_a.b = color_a & 0xFF;
	rgb_b.a = (color_b >> 24) & 0xFF;
	rgb_b.r = (color_b >> 16) & 0xFF;
	rgb_b.g = (color_b >> 8)  & 0xFF;
	rgb_b.b = color_b & 0xFF;
	blend.a = (rgb_a.a * (1 - factor) + rgb_b.a * factor);
	blend.r = (rgb_a.r * (1 - factor) + rgb_b.r * factor);
	blend.g = (rgb_a.g * (1 - factor) + rgb_b.g * factor);
	blend.b = (rgb_a.b * (1 - factor) + rgb_b.b * factor);
	result = \
	(((int)blend.a << 24) | \
	((int)blend.r << 16) | \
	((int)blend.g << 8) | \
	((int)blend.b));
	return (result);
}

void	flush_img(t_image *src, int color, float transp, int ignore_alpha_pixels)
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
	{
		if (ignore_alpha_pixels && (img_data[i] & 0xFF000000) != 0x00000000)
			continue ;
		if (transp == -1)
			img_data[i] = color;
		else
			img_data[i] = blend_color(img_data[i], color, transp);
	}
}

void	fill_transparency(t_image *src, int color)
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
	{
		if ((img_data[i] & 0xFF000000) == 0x00000000)
			continue ;
		img_data[i] = color;
	}
}

int	is_in_screen(t_md *md, t_vec3 pos, t_vec2 size)
{
	return (pos.x > -size.x && pos.x < md->win_size.x + \
		size.x && pos.y > -size.y && pos.y < md->win_size.y + size.y);
}
