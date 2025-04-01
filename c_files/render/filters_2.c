/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filters_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:52:06 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/26 16:48:56 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	apply_barrel_distortion(t_image *img, float intensity)
{
	t_vec2	pos;
	t_vec2	center;
	t_vec2	n;
	t_vec3f	norm;
	t_vec3f	rad;

	rad.y = intensity * 0.3f;
	center.x = img->size.x / 2;
	center.y = img->size.y / 2;
	pos.y = -1;
	while (++pos.y < img->size.y)
	{
		pos.x = -1;
		while (++pos.x < img->size.x)
		{
			norm.x = (pos.x - center.x) / (float)center.x;
			norm.y = (pos.y - center.y) / (float)center.y;
			rad.x = sqrt(norm.x * norm.x + norm.y * norm.y);
			n.x = center.x + norm.x * (1.0 + rad.y * rad.x) * center.x;
			n.y = center.y + norm.y * (1.0 + rad.y * rad.x) * center.y;
			if (n.x >= 0 && n.x < img->size.x && n.y >= 0 && n.y < img->size.y)
				img->src[pos.y * img->size.x + pos.x] = \
				img->src[n.y * img->size.x + n.x];
		}
	}
}

void	apply_color_banding(t_image *img, float intensity)
{
	t_vec2			pos;
	unsigned int	pixel;
	unsigned char	rgba[4];
	int				levels;
	int				factor;

	intensity = 1 - intensity;
	levels = 2 + (int)(intensity * 14.0f);
	factor = 255 / levels;
	pos.y = -1;
	while (++pos.y < img->size.y)
	{
		pos.x = -1;
		while (++pos.x < img->size.x)
		{
			pixel = img->src[pos.y * img->size.x + pos.x];
			rgba[0] = ((pixel >> 16) & 0xFF) / factor * factor;
			rgba[1] = ((pixel >> 8) & 0xFF) / factor * factor;
			rgba[2] = (pixel & 0xFF) / factor * factor;
			rgba[3] = (pixel >> 24) & 0xFF;
			img->src[pos.y * img->size.x + pos.x] = \
				(rgba[3] << 24) | (rgba[0] << 16) | (rgba[1] << 8) | rgba[2];
		}
	}
}

void	apply_bloom(t_image *img, float intensity)
{
	t_vec3			pos;
	unsigned int	blrx[2];
	unsigned char	rgba[4];

	pos = get_v3(0, -1, 150 - (int)(intensity * 100.0f));
	while (++pos.y < img->size.y - 1)
	{
		pos.x = -1;
		while (++pos.x < img->size.x - 1)
		{
			blrx[1] = img->src[pos.y * img->size.x + pos.x];
			rgba[0] = (blrx[1] >> 16) & 0xFF;
			rgba[1] = (blrx[1] >> 8) & 0xFF;
			rgba[2] = blrx[1] & 0xFF;
			rgba[3] = (blrx[1] >> 24) & 0xFF;
			if (rgba[0] + rgba[1] + rgba[2] <= pos.z)
				continue ;
			blrx[0] = img->src[(pos.y + 1) * img->size.x + pos.x] >> 1;
			blrx[0] += img->src[(pos.y - 1) * img->size.x + pos.x] >> 1;
			blrx[0] += img->src[pos.y * img->size.x + pos.x + 1] >> 1;
			blrx[0] += img->src[pos.y * img->size.x + pos.x - 1] >> 1;
			img->src[pos.y * img->size.x + pos.x] = \
				(rgba[3] << 24) | (blrx[0] & 0xFFFFFF);
		}
	}
}

void	apply_vignette(t_image *img, float intensity, int color)
{
	t_vec2	pos;
	int		txtr_color;
	float	distance;
	float	max_distance;
	float	alpha;

	max_distance = sqrt(pow(img->size.x / 2.0f, 2) + \
		pow(img->size.y / 2.0f, 2));
	pos = get_v2(-1, -1);
	while (++pos.y < img->size.y)
	{
		pos.x = -1;
		while (++pos.x < img->size.x)
		{
			txtr_color = img->src[pos.y * img->size.x + pos.x];
			distance = sqrt(pow(pos.x - img->size.x / 2.0f, 2) + \
				pow(pos.y - img->size.y / 2.0f, 2));
			alpha = 1.0f - (distance / max_distance);
			alpha = fmax(0.0f, alpha);
			alpha *= intensity;
			img->src[pos.y * img->size.x + pos.x] = \
				blend_color(txtr_color, color, alpha);
		}
	}
}

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
		color = img->src[i];
		rgba.r = ((color >> 16) & 0xFF) * rgb_factors.r;
		rgba.g = ((color >> 8) & 0xFF) * rgb_factors.g;
		rgba.b = (color & 0xFF) * rgb_factors.b;
		rgba.a = (color >> 24) & 0xFF;
		rgba.r = minmaxf(0, 255, rgba.r);
		rgba.g = minmaxf(0, 255, rgba.g);
		rgba.b = minmaxf(0, 255, rgba.b);
		color = (rgba.a << 24) | (rgba.r << 16) | (rgba.g << 8) | rgba.b;
		img->src[i] = color;
	}
}
