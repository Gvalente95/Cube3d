/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 12:51:08 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 05:28:11 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	set_hue(t_image *img, t_vec4f rgb_factors)
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

void	apply_d(t_image *img, unsigned int *new_d, t_vec2 pos, float half_krnfl)
{
	t_vec4			rgb;
	t_vec3			ij;
	unsigned int	pixel;
	unsigned int	rgba[4];
	t_vec2			p;

	rgb = get_v4(0, 0, 0, 0);
	ij.z = 0;
	ij.x = -half_krnfl;
	while (ij.x <= half_krnfl)
	{
		ij.y = -half_krnfl;
		while (ij.y <= half_krnfl)
		{
			p = get_v2(pos.x + ij.x, pos.y + ij.y);
			ij.y++;
			if (p.x < 0 || p.x >= img->size.x || p.y < 0 || p.y >= img->size.y)
				continue ;
			ij.y--;
			pixel = img->src_data[p.y * img->size.x + p.x];
			rgb.x += (pixel >> 16) & 0xFF;
			rgb.y += (pixel >> 8) & 0xFF;
			rgb.z += pixel & 0xFF;
			rgb.w += (pixel >> 24) & 0xFF;
			ij = get_v3(ij.x, ij.y++, ij.z++);
		}
		ij.x++;
	}
	if (ij.z == 0)
		ij.z = 1;
	rgba[0] = (unsigned int)(rgb.x / ij.z);
	rgba[1] = (unsigned int)(rgb.y / ij.z);
	rgba[2] = (unsigned int)(rgb.z / ij.z);
	rgba[3] = (unsigned int)(rgb.w / ij.z);
	new_d[pos.y * img->size.x + pos.x] = \
		(rgba[3] << 24) | (rgba[0] << 16) | (rgba[1] << 8) | rgba[2];
}

void	apply_antialiasing(t_image *img)
{
	t_vec2			pos;
	float			half_kernel;
	unsigned int	*new_data;
	t_vec2			size;

	size = img->size;
	half_kernel = 3.0 / 2.0;
	new_data = malloc(size.x * size.y * sizeof(unsigned int));
	if (!new_data)
		return ;
	pos = get_v2(-1, -1);
	while (++pos.y < size.y)
	{
		pos.x = -1;
		while (++pos.x < size.x)
			apply_d(img, new_data, pos, half_kernel);
	}
	ft_memcpy(img->src_data, new_data, size.x * size.y * sizeof(unsigned int));
	free(new_data);
}

void	apply_scanlines(t_image *img, float factor)
{
	t_vec2			pos;
	unsigned char	rgba[4];
	unsigned int	pixel;

	pos.y = -1;
	while (++pos.y < img->size.y)
	{
		pos.x = -1;
		while (++pos.x < img->size.x)
		{
			if (pos.y % 2 == 0)
				continue ;
			pixel = img->src_data[pos.y * img->size.x + pos.x];
			rgba[0] = (pixel >> 16) & 0xFF;
			rgba[1] = (pixel >> 8) & 0xFF;
			rgba[2] = pixel & 0xFF;
			rgba[3] = (pixel >> 24) & 0xFF;
			rgba[0] = (int)(rgba[0] * factor);
			rgba[1] = (int)(rgba[1] * factor);
			rgba[2] = (int)(rgba[2] * factor);
			img->src_data[pos.y * img->size.x + pos.x] = \
			(rgba[3] << 24) | (rgba[0] << 16) | (rgba[1] << 8) | rgba[2];
		}
	}
}

void	apply_rgb_glitch(t_image *img, int intensity)
{
	t_vec2			pos;
	unsigned char	rgb[3];
	unsigned int	pixel;
	int				offset;
	int				new_x;

	pos = get_v2(-1, -1);
	while (++pos.y < img->size.y)
	{
		pos.x = -1;
		while (++pos.x < img->size.x)
		{
			offset = (rand() % (intensity * 2)) - intensity;
			new_x = pos.x + offset;
			if (new_x < 0 || new_x >= img->size.x)
				continue ;
			pixel = img->src_data[pos.y * img->size.x + new_x];
			rgb[0] = (pixel >> 16) & 0xFF;
			rgb[1] = (pixel >> 8) & 0xFF;
			rgb[2] = pixel & 0xFF;
			img->src_data[pos.y * img->size.x + pos.x] = \
				((rgb[0] << 16) | (rgb[1] << 8) | rgb[2]);
		}
	}
}
