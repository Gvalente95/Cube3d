/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images_b.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:52:38 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/30 15:48:24 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

void	set_img_color(t_image *img_data, t_vec2 size, int col, float str)
{
	t_vec2	pos;
	t_vec2	ratio;
	t_vec4	rgb;
	double	x_rat;
	int		pixel_index;

	pos.y = -1;
	while (++pos.y < size.y)
	{
		pos.x = -1;
		while (++pos.x < size.x)
		{
			pixel_index = (pos.y * img_data->size_line / 4) + pos.x;
			x_rat = img_data->src[pixel_index];
			if (get_t(x_rat) == 0)
			{
				rgb.r = (int)((get_r(x_rat) * (1 - str)) + (get_r(col) * str));
				rgb.g = (int)((get_g(x_rat) * (1 - str)) + (get_g(col) * str));
				rgb.b = (int)((get_b(x_rat) * (1 - str)) + (get_b(col) * str));
				img_data->src[pixel_index] = \
					get_trgb(get_t(x_rat), rgb.r, rgb.g, rgb.a);
			}
		}
	}
}

void	*get_image_copy(t_md *md, void *src, t_vec2 src_size)
{
	t_image	imgd;
	t_vec2	src_pos;
	t_vec2	dst_pos;
	int		*dst_data;

	imgd.img = mlx_new_image(md->mlx, src_size.x, src_size.y);
	if (imgd.img == NULL)
		return (NULL);
	imgd.src = (int *)mlx_get_data_addr(src, &imgd.bpp, &src_pos.x, \
		&imgd.endian);
	dst_data = (int *)mlx_get_data_addr(imgd.img, &imgd.bpp, &dst_pos.x, \
		&imgd.endian);
	imgd.pos.y = -1;
	while (++imgd.pos.y < src_size.y)
	{
		imgd.pos.x = -1;
		while (++imgd.pos.x < src_size.x)
		{
			src_pos.y = (imgd.pos.y * src_pos.x / 4) + imgd.pos.x;
			dst_pos.y = (imgd.pos.y * dst_pos.x / 4) + imgd.pos.x;
			dst_data[dst_pos.y] = imgd.src[src_pos.y];
		}
	}
	return (imgd.img);
}

void	draw_rot_z(t_md *md, t_image *from, t_image *onto, t_vec3f pos)
{
	const t_vec2	src_sz = (t_vec2){from->size.x, from->size.y};
	t_vec2f			scale;
	t_vec2			d_pos;
	t_vec2			sc_pos;
	t_vec3			draw_d;

	if (!from || !onto || !from->src || !onto->src)
		return ;
	scale.x = maxf(0.05f, cosf(pos.z * (M_PI / 180.0f)));
	d_pos.x = -1;
	while (++d_pos.x < src_sz.x)
	{
		sc_pos.x = (int)((float)d_pos.x * scale.x);
		d_pos.y = -1;
		while (++d_pos.y < src_sz.y)
		{
			draw_d.z = from->src[d_pos.y * src_sz.x + d_pos.x];
			sc_pos.y = d_pos.y;
			draw_d.x = pos.x + sc_pos.x - (src_sz.x * scale.x / 2);
			draw_d.y = pos.y + sc_pos.y - (src_sz.y / 2);
			draw_d.y += (int)((d_pos.x - src_sz.x / 2) * \
				cosf(pos.x * (M_PI / 180.0f)) * 0.5f);
			draw_pixel(onto, get_v2(draw_d.x, draw_d.y), draw_d.z, -1);
		}
	}
}

void	draw_rot_y(t_md *md, t_image *from, t_image *onto, t_vec3f pos)
{
	const t_vec2	src_sz = (t_vec2){from->size.x, from->size.y};
	t_vec2f			scale;
	t_vec2			d_pos;
	t_vec2			sc_pos;
	t_vec3			draw_d;

	if (!from || !onto || !from->src || !onto->src)
		return ;
	scale.y = maxf(0.05f, cosf(pos.z * (M_PI / 180.0f)));
	d_pos.y = -1;
	while (++d_pos.y < src_sz.y)
	{
		sc_pos.y = (int)((float)d_pos.y * scale.y);
		d_pos.x = -1;
		while (++d_pos.x < src_sz.x)
		{
			draw_d.z = from->src[d_pos.y * src_sz.x + d_pos.x];
			sc_pos.x = d_pos.x;
			draw_d.x = pos.x + sc_pos.x - (src_sz.x / 2);
			draw_d.y = pos.y + sc_pos.y - (src_sz.y * scale.y / 2);
			draw_pixel(onto, get_v2(draw_d.x, draw_d.y), draw_d.z, -1);
		}
	}
}

void	draw_rot_yz(t_md *md, t_image *from, t_image *onto, t_vec4f pos)
{
	const t_vec2	src_sz = (t_vec2){from->size.x, from->size.y};
	t_vec2f			scale;
	t_vec2			d_pos;
	t_vec2			sc_pos;
	t_vec3			draw_d;

	if (!from || !onto || !from->src || !onto->src)
		return ;
	scale.x = maxf(0.05f, cosf(pos.b * (M_PI / 180.0f)));
	scale.y = maxf(0.05f, cosf(pos.a * (M_PI / 180.0f)));
	d_pos.y = -1;
	while (++d_pos.y < src_sz.y)
	{
		sc_pos.y = (int)((float)d_pos.y * scale.y);
		d_pos.x = -1;
		while (++d_pos.x < src_sz.x)
		{
			draw_d.z = from->src[d_pos.y * src_sz.x + d_pos.x];
			sc_pos.x = (int)((float)d_pos.x * scale.x);
			draw_d.x = pos.r + sc_pos.x - (src_sz.x * scale.x / 2);
			draw_d.y = pos.g + sc_pos.y - (src_sz.y * scale.y / 2);
			draw_pixel(onto, get_v2(draw_d.x, draw_d.y), draw_d.z, -1);
		}
	}
}
