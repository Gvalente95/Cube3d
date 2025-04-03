/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_color.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 10:42:14 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/03 10:50:04 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

void	remove_img_color(t_image *img, int remove_color)
{
	int		*img_data;
	int		total_pixels;
	int		i;
	t_vec4	target;
	t_vec4	clr;

	if (!img || !img->src)
		return ;
	total_pixels = img->size.x * img->size.y;
	i = -1;
	while (++i < total_pixels)
	{
		clr = color_to_v4(img->src[i]);
		target = color_to_v4(remove_color);
		if (clr.r == target.r && clr.g == target.g && clr.b == target.b)
			img->src[i] = 0xFF000000;
	}
}

int	blend_color(int color_a, int color_b, float opacity)
{
	t_vec4	rgb_a;
	t_vec4	rgb_b;
	t_vec4	b;
	int		result;

	if (opacity < 0.0f)
		opacity = 0.0f;
	if (opacity > 1.0f)
		opacity = 1.0f;
	rgb_a.a = (color_a >> 24) & 0xFF;
	rgb_a.r = (color_a >> 16) & 0xFF;
	rgb_a.g = (color_a >> 8) & 0xFF;
	rgb_a.b = color_a & 0xFF;
	rgb_b.a = (color_b >> 24) & 0xFF;
	rgb_b.r = (color_b >> 16) & 0xFF;
	rgb_b.g = (color_b >> 8) & 0xFF;
	rgb_b.b = color_b & 0xFF;
	if (opacity == 0.0f)
		opacity = 1.0f - ((float)rgb_b.a / 255.0f);
	b.r = (rgb_a.r * (1.0f - opacity)) + (rgb_b.r * opacity);
	b.g = (rgb_a.g * (1.0f - opacity)) + (rgb_b.g * opacity);
	b.b = (rgb_a.b * (1.0f - opacity)) + (rgb_b.b * opacity);
	b.a = (rgb_a.a * (1.0f - opacity)) + (rgb_b.a * opacity);
	result = ((int)b.a << 24) | ((int)b.r << 16) | ((int)b.g << 8) | (int)b.b;
	return (result);
}

//		color_d: x = colr - y = thickness
void	draw_line(t_image *onto, t_vec2 start, t_vec2 end, t_vec2 color_d)
{
	t_vec2	delta;
	t_vec2f	step;
	float	steps;
	float	i;
	t_vec2f	pos;

	delta = sub_vec2(end, start);
	steps = fmaxf(abs(delta.x), abs(delta.y));
	if (steps == 0)
		return ;
	step = get_v2f(delta.x / steps, delta.y / steps);
	pos = get_v2f(start.x, start.y);
	i = -1;
	while (++i <= steps)
	{
		start = (t_vec2){(int)(pos.x + 0.5f), (int)(pos.y + 0.5f)};
		if (get_alpha(onto->src[start.y * \
			(onto->size_line / 4) + start.x]) == 1)
			break ;
		if (color_d.y > 1)
			draw_pixels(onto, start, v2(color_d.y), color_d.x);
		else
			draw_pixel(onto, start, color_d.x, get_alpha(color_d.x));
		pos = add_vec2f(pos, step);
	}
}
