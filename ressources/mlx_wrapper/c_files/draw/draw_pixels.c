/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_pixels.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:55:24 by gvalente          #+#    #+#             */
/*   Updated: 2025/04/01 18:18:25 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

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

int	put_pxl_if_vis(t_draw_d *dd, int overlap, int blend, float opacty)
{
	t_image	*src_img;
	t_image	*dst;
	int		src_i;
	int		dst_i;
	int		clr;

	if ((dd->dst_pos.y < 0 || dd->dst_pos.y > dd->dst->size.y) || \
	dd->dst_pos.x < 0 || dd->dst_pos.x >= dd->dst->size.x)
		return (0);
	src_img = dd->src;
	dst = dd->dst;
	dst_i = dd->dst_pos.y * (dst->size_line / 4) + dd->dst_pos.x;
	if (overlap != -1 && dst->src[dst_i] != (unsigned int)overlap)
		return (0);
	clr = dd->src_pos.x;
	if (src_img && src_img->src)
	{
		src_i = dd->src_pos.y * (src_img->size_line / 4) + dd->src_pos.x;
		if (((src_img->src[src_i] >> 24) & 0xFF) == 255)
			return (0);
		clr = src_img->src[src_i];
	}
	if (blend)
		return (dst->src[dst_i] = blend_color(dst->src[dst_i], clr, opacty), 1);
	return (dst->src[dst_i] = clr, 1);
}

int	draw_pixel(t_image *texture, t_vec2 pos, int color, float opacity)
{
	int	index;

	if (!texture || !texture->src)
		return (0);
	if (pos.x < 0 || pos.x >= texture->size.x || \
		pos.y < 0 || pos.y >= texture->size.y)
		return (0);
	if ((texture->size_line / 4) != texture->size.x)
		return (printf("Warning: size_line / 4 != size.x\n"), 0);
	index = pos.y * (texture->size_line / 4) + pos.x;
	if (opacity < 0)
		texture->src[index] = color;
	else
		texture->src[index] = blend_color(texture->src[index], color, opacity);
	return (1);
}

int	draw_safe_pixel(t_image *texture, t_vec2 pos, int color, int drawover)
{
	int	index;
	int	cur_clr;

	if (!texture || !texture->src)
		return (0);
	if (pos.x < 0 || pos.x >= texture->size.x || \
		pos.y < 0 || pos.y >= texture->size.y)
		return (0);
	if ((texture->size_line / 4) != texture->size.x)
		return (printf("Warning: size_line / 4 != size.x\n"), 0);
	index = pos.y * (texture->size_line / 4) + pos.x;
	cur_clr = texture->src[index];
	if (cur_clr != drawover)
		return (0);
	texture->src[index] = color;
	return (1);
}

int	draw_pixels(t_image *txtr, t_vec2 pos, t_vec2 draw_size, int color)
{
	t_vec2	draw_pos;
	t_vec2	end_coord;
	int		draw_count;
	float	alpha;

	if (!txtr || !txtr->src || pos.x < 0 || pos.y < 0)
		return (0);
	alpha = 1.0f - ((float)((color >> 24) & 0xFF) / 255.0f);
	if (alpha < 0.2f)
		alpha = 0.2f;
	end_coord = get_v2(pos.x + draw_size.x, pos.y + draw_size.y);
	draw_count = 0;
	draw_pos = pos;
	while (draw_pos.y < end_coord.y)
	{
		draw_pos.x = pos.x;
		while (draw_pos.x < end_coord.x)
		{
			draw_count += draw_pixel(txtr, draw_pos, color, alpha);
			draw_pos.x++;
		}
		draw_pos.y++;
	}
	return (draw_count);
}

//		color_d: x = colr - y = thickness
void	draw_line(t_image *onto, t_vec2 start, t_vec2 end, t_vec2 color_d)
{
	t_vec3f	delta;
	t_vec3f	step;
	float	steps;
	float	i;
	t_vec3f	pos;

	delta.x = end.x - start.x;
	delta.y = end.y - start.y;
	steps = fmaxf(fabsf(delta.x), fabsf(delta.y));
	if (steps == 0)
		return ;
	step.x = delta.x / steps;
	step.y = delta.y / steps;
	pos.x = start.x;
	pos.y = start.y;
	i = -1;
	while (++i <= steps)
	{
		start = (t_vec2){(int)(pos.x + 0.5f), (int)(pos.y + 0.5f)};
		if (color_d.y > 1)
			draw_pixels(onto, start, v2(color_d.y), color_d.x);
		else
			draw_pixel(onto, start, color_d.x, get_alpha(color_d.x));
		pos = get_v3f(pos.x + step.x, pos.y + step.y, pos.z);
	}
}
