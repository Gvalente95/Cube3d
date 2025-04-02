/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_img.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:55:24 by gvalente          #+#    #+#             */
/*   Updated: 2025/04/02 13:57:50 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

//	data: x = color | y = intensity | z = overlap_color
void	draw_clr_img(t_image *src, t_image *dst, t_vec2 pos, t_vec3 data)
{
	t_vec2	draw_pos;
	t_vec2	src_pos;
	t_vec2	dst_pos;
	t_vec2	pxl_i;

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
			pxl_i.x = src_pos.y * (src->size_line / 4) + src_pos.x;
			pxl_i.y = dst_pos.y * (dst->size_line / 4) + dst_pos.x;
			dst->src[pxl_i.y] = blend_color(dst->src[pxl_i.y], \
					data.x, (float)data.y / 10);
		}
	}
}

int	draw_img(t_image *src, t_image *dst, t_vec2 pos, int overlap_color)
{
	t_draw_d	draw_d;
	t_vec2		draw_pos;

	if (!src || !dst || !src->src || !dst->src)
		return (printf("Error\ndraw img on null >> %s\n", src->path), 0);
	draw_d.src = src;
	draw_d.dst = dst;
	draw_pos = get_v2(-1, -1);
	while (++draw_pos.y < src->size.y)
	{
		draw_d.src_pos.y = draw_pos.y;
		draw_d.dst_pos.y = pos.y + draw_pos.y;
		if (draw_d.dst_pos.y < 0 || draw_d.dst_pos.y >= dst->size.y)
			continue ;
		draw_pos.x = -1;
		while (++draw_pos.x < src->size.x)
		{
			draw_d.src_pos.x = draw_pos.x;
			draw_d.dst_pos.x = pos.x + draw_pos.x;
			put_pxl_if_vis(&draw_d, overlap_color, 0, 0);
		}
	}
	return (1);
}

void	flipx_image_data(t_image *img)
{
	t_vec2	pos;
	int		tmp_pxl;
	int		end_pxl;
	int		cur_index;
	int		end_index;

	pos = get_v2(-1, -1);
	while (++pos.y < img->size.y)
	{
		pos.x = -1;
		while (++pos.x < img->size.x / 2)
		{
			cur_index = pos.y * (img->size_line / 4) + pos.x;
			end_index = pos.y * (img->size_line / 4) + \
			(img->size_line / 4 - 1 - pos.x);
			tmp_pxl = img->src[cur_index];
			img->src[cur_index] = img->src[end_index];
			img->src[end_index] = tmp_pxl;
		}
	}
}

void	flipy_image_data(t_image *img)
{
	t_vec2	pos;
	int		top_index;
	int		bot_index;
	int		tmp_pxl;
	int		width;

	width = img->size_line / 4;
	pos.y = -1;
	while (++pos.y < img->size.y / 2)
	{
		pos.x = -1;
		while (++pos.x < img->size.x)
		{
			top_index = pos.y * width + pos.x;
			bot_index = (img->size.y - 1 - pos.y) * width + pos.x;
			tmp_pxl = img->src[top_index];
			img->src[top_index] = img->src[bot_index];
			img->src[bot_index] = tmp_pxl;
		}
	}
}

void	flush_img(t_image *src, int color, \
	float transp, int ignore_alpha)
{
	int		*img_data;
	int		total_pixels;
	int		i;

	if (!src || !src->src)
		return ;
	if (color == -1)
		color = 0x000000;
	img_data = src->src;
	total_pixels = src->size.x * src->size.y;
	if (transp > 0)
		transp = 1 - transp;
	i = -1;
	while (++i < total_pixels)
	{
		if (ignore_alpha > 0 && (img_data[i] & 0xFF000000) != 0x00000000)
			continue ;
		if (transp < 0)
			img_data[i] = color;
		else
			img_data[i] = blend_color(img_data[i], color, transp);
	}
}

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
