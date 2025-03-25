/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images_b.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:52:38 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/23 17:31:33 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

void	set_img_data_color(t_image *img_data, t_vec2 size, int col, float str)
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
