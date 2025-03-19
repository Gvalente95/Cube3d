/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dithering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 23:17:47 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 04:30:24 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

unsigned int	apply_dither(unsigned char *rgb1, \
		unsigned int *palette, int palette_size, int best_distance)
{
	int				best_match;
	int				distance;
	int				i;
	unsigned char	rgb2[3];
	t_vec4			drgb;

	best_match = palette[0];
	i = -1;
	while (++i < palette_size)
	{
		rgb2[0] = (palette[i] >> 16) & 0xFF;
		rgb2[1] = (palette[i] >> 8) & 0xFF;
		rgb2[2] = palette[i] & 0xFF;
		drgb.r = rgb1[0] - rgb2[0];
		drgb.g = rgb1[1] - rgb2[1];
		drgb.b = rgb1[2] - rgb2[2];
		distance = (drgb.r * drgb.r) + (drgb.g * drgb.g) + (drgb.b * drgb.b);
		if (distance < best_distance)
		{
			best_distance = distance;
			best_match = palette[i];
		}
	}
	return (best_match);
}

unsigned int	find_near_clr(unsigned int pxl, unsigned int *plt, int plt_size)
{
	int				best_distance;
	unsigned char	rgb1[3];

	best_distance = 0x7FFFFFFF;
	rgb1[0] = (pxl >> 16) & 0xFF;
	rgb1[1] = (pxl >> 8) & 0xFF;
	rgb1[2] = pxl & 0xFF;
	return (apply_dither(rgb1, plt, plt_size, best_distance));
}

void	apply_error(t_image *img, t_vec2 pos, t_vec4 err_rgb, t_vec3f data)
{
	float			factor;
	float			dither_strength;
	unsigned int	pixel;
	t_vec4			rgb;

	factor = data.x;
	dither_strength = data.y;
	pixel = img->src_data[pos.x + pos.y * img->size.x];
	rgb.r = ((pixel >> 16) & 0xFF) + \
		(int)(err_rgb.r * factor * dither_strength);
	rgb.g = ((pixel >> 8) & 0xFF) + (int)(err_rgb.g * factor * dither_strength);
	rgb.b = (pixel & 0xFF) + (int)(err_rgb.b * factor * dither_strength);
	rgb.r = minmax(0, 255, rgb.r);
	rgb.g = minmax(0, 255, rgb.g);
	rgb.b = minmax(0, 255, rgb.b);
	img->src_data[pos.x + pos.y * img->size.x] = \
		(rgb.r << 16) | (rgb.g << 8) | rgb.b;
}

void	dither(t_image *img, t_vec3 pos, unsigned int *palette, float str)
{
	t_vec4	old;
	t_vec4	rgb;
	t_vec4	new;
	t_vec2	pxl;

	pxl.x = img->src_data[pos.x + pos.y * img->size.x];
	pxl.y = find_near_clr(pxl.x, palette, pos.z);
	img->src_data[pos.x + pos.y * img->size.x] = pxl.y;
	old = get_v4((pxl.x >> 16) & 0xFF, (pxl.x >> 8) & 0xFF, pxl.x & 0xFF, 0);
	new = get_v4((pxl.y >> 16) & 0xFF, (pxl.y >> 8) & 0xFF, pxl.y & 0xFF, 0);
	rgb = get_v4(old.r - new.r, old.g - new.g, \
		old.b - new.b, -1);
	if (pos.x + 1 < img->size.x)
		apply_error(\
	img, get_v2(pos.x + 1, pos.y), rgb, get_v3f(7.0 / 16.0, str, 0));
	if (pos.x - 1 >= 0 && pos.y + 1 < img->size.y)
		apply_error(\
	img, get_v2(pos.x - 1, pos.y + 1), rgb, get_v3f(3.0 / 16.0, str, 0));
	if (pos.y + 1 < img->size.y)
		apply_error(\
	img, get_v2(pos.x, pos.y + 1), rgb, get_v3f(5.0 / 16.0, str, 0));
	if (pos.x + 1 < img->size.x && pos.y + 1 < img->size.y)
		apply_error(\
	img, get_v2(pos.x + 1, pos.y + 1), rgb, get_v3f(1.0 / 16.0, str, 0));
}

void	apply_dithering(t_image *img, float dither_strength)
{
	unsigned int	palette[11];
	int				palette_size;
	t_vec3			pos;
	t_vec2			size;

	palette[0] = 0x000000;
	palette[1] = 0x555555;
	palette[2] = 0xAAAAAA;
	palette[3] = 0xFFFFFF;
	palette[4] = 0xFF0000;
	palette[6] = 0x00FF00;
	palette[7] = 0x0000FF;
	palette[8] = 0xFFFF00;
	palette[9] = 0xFF00FF;
	palette[10] = 0x00FFFF;
	size = img->size;
	palette_size = sizeof(palette) / sizeof(palette[0]);
	pos.y = -1;
	pos.z = palette_size;
	while (++pos.y < size.y)
	{
		pos.x = -1;
		while (++pos.x < size.x)
			dither(img, pos, palette, dither_strength);
	}
}
