/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMAGES_b.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:52:38 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/14 20:04:10 by giuliovalen      ###   ########.fr       */
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
			x_rat = img_data->src_data[pixel_index];
			if (get_t(x_rat) == 0)
			{
				rgb.r = (int)((get_r(x_rat) * (1 - str)) + (get_r(col) * str));
				rgb.g = (int)((get_g(x_rat) * (1 - str)) + (get_g(col) * str));
				rgb.b = (int)((get_b(x_rat) * (1 - str)) + (get_b(col) * str));
				img_data->src_data[pixel_index] = \
					get_trgb(get_t(x_rat), rgb.r, rgb.g, rgb.a);
			}
		}
	}
}


void	*get_image_copy(t_md *md, void *src, t_vec2 src_size)
{
	t_image	img_data;
	t_vec2	src_pos;
	t_vec2	dst_pos;

	img_data.img = mlx_new_image(md->mlx, src_size.x, src_size.y);
	if (img_data.img == NULL)
		return (NULL);
	img_data.src_data = (int *)mlx_get_data_addr(src, &img_data.bpp, &src_pos.x, \
		&img_data.endian);
	img_data.dst_data = (int *)mlx_get_data_addr(img_data.img, &img_data.bpp, &dst_pos.x, \
		&img_data.endian);
	img_data.pos.y = 0;
	while (img_data.pos.y < src_size.y)
	{
		img_data.pos.x = 0;
		while (img_data.pos.x < src_size.x)
		{
			src_pos.y = (img_data.pos.y * src_pos.x / 4) + img_data.pos.x;
			dst_pos.y = (img_data.pos.y * dst_pos.x / 4) + img_data.pos.x;
			img_data.dst_data[dst_pos.y] = img_data.src_data[src_pos.y];
			img_data.pos.x++;
		}
		img_data.pos.y++;
	}
	return (img_data.img);
}
