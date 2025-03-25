/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_img.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:55:24 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/25 14:00:18 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

//	data: x = color | y = intensity | z = overlap_color
void	draw_colored_img(t_image *src, t_image *dst, t_vec2 pos, t_vec3 data)
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
	t_draw_data	draw_d;
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
			put_pixel_if_visible(&draw_d, overlap_color, 0, 0);
		}
	}
	return (1);
}

void	draw_transp_img(t_image *src, t_image *dst, t_vec2 pos, float trnsp)
{
	t_draw_data	draw_d;
	t_vec2		draw_pos;

	if (!src || !dst || !src->src || !dst->src)
		return ;
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
			put_pixel_if_visible(&draw_d, -1, 1, trnsp);
		}
	}
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

void	draw_contour(t_image *img, int color, int thickness, float transparency)
{
	t_vec2	lines[4][2];
	int		half_thck;
	int		i;

	color = set_alpha(color, transparency);
	half_thck = thickness / 2;
	if (half_thck < 1)
		half_thck = 1;
	lines[0][0] = v2(0);
	lines[0][1] = get_v2(img->size.x - half_thck, 0);
	lines[1][0] = v2(0);
	lines[1][1] = get_v2(0, img->size.y - half_thck);
	lines[2][0] = get_v2(img->size.x - thickness, 0);
	lines[2][1] = get_v2(img->size.x - thickness, img->size.y);
	lines[3][0] = get_v2(0, img->size.y - thickness);
	lines[3][1] = get_v2(img->size.x - half_thck, img->size.y - thickness);
	i = -1;
	while (++i < 4)
		draw_line(img, lines[i][0], lines[i][1], get_v2(color, thickness));
}

void	flipy_image_data(t_image *img)
{
	t_vec2	pos;
	int		top_index;
	int		bot_index;
	int		tmp_pxl;
	int		width = img->size_line / 4;

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
