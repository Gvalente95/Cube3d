/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMAGES_b.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:52:38 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 13:21:38 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

void	*set_img_color(void *frame, t_vec2 size, int col, float str)
{
	t_image	q;
	int		r;
	int		g;
	int		b;
	int		pixel_index;

	q.src_data = (int *)mlx_get_data_addr(frame, &q.bpp, &q.len, &q.endian);
	q.pos.y = -1;
	while (++q.pos.y < size.y)
	{
		q.pos.x = -1;
		while (++q.pos.x < size.x)
		{
			pixel_index = (q.pos.y * q.len / 4) + q.pos.x;
			q.x_ratio = q.src_data[pixel_index];
			if (get_t(q.x_ratio) == 0)
			{
				r = (int)((get_r(q.x_ratio) * (1 - str)) + (get_r(col) * str));
				g = (int)((get_g(q.x_ratio) * (1 - str)) + (get_g(col) * str));
				b = (int)((get_b(q.x_ratio) * (1 - str)) + (get_b(col) * str));
				q.src_data[pixel_index] = get_trgb(get_t(q.x_ratio), r, g, b);
			}
		}
	}
	return (frame);
}

void	set_transparency(void *src, void *dest, t_vec2 size, float trnsp)
{
	int		*src_data;
	int		*dst;
	int		pixel;
	t_vec2	pos;
	t_vec3	rgb;

	src_data = (int *)mlx_get_data_addr(src, &(int){0}, &(int){0}, &(int){0});
	dst = (int *)mlx_get_data_addr(dest, &(int){0}, &(int){0}, &(int){0});
	pos.y = -1;
	while (++pos.y < size.y)
	{
		pos.x = -1;
		while (++pos.x < size.x)
		{
			pixel = src_data[pos.y * size.x + pos.x];
			rgb.x = ((pixel >> 16) & 0xFF) * trnsp;
			rgb.y = ((pixel >> 8) & 0xFF) * trnsp;
			rgb.z = (pixel & 0xFF) * trnsp;
			dst[pos.y * size.x + pos.x] = (rgb.x << 16) | (rgb.y << 8) | rgb.z;
		}
	}
}

void	*split_img_at_x(void *img, t_vec2 img_size, int cutoff, t_dir dir)
{
	t_image	q;
	t_vec2	pos;

	q.src_data = (int *)mlx_get_data_addr(img, &q.bpp, &q.len, &q.endian);
	pos.y = -1;
	if (cutoff < 0 || (cutoff > img_size.x && (dir == left || dir == right)) \
		|| (cutoff > img_size.y && (dir == up || dir == down)))
		return (img);
	while (++pos.y < img_size.y)
	{
		pos.x = -1;
		if (dir == right)
			pos.x = cutoff;
		if (dir == left)
			while (++pos.x < cutoff)
				q.src_data[pos.y * (q.len / 4) + pos.x] = 0xFF000000;
		else if (dir == right)
			while (++pos.x < img_size.x)
				q.src_data[pos.y * (q.len / 4) + pos.x] = 0xFF000000;
		else if (dir == down && pos.y < cutoff)
			while (++pos.x < img_size.x)
				q.src_data[pos.y * (q.len / 4) + pos.x] = 0xFF000000;
		else if (dir == up && pos.y >= img_size.y - cutoff)
			while (++pos.x < img_size.x)
				q.src_data[pos.y * (q.len / 4) + pos.x] = 0xFF000000;
	}
	return (img);
}

void	*get_image_copy(t_md *md, void *src, t_vec2 src_size)
{
	t_image	img;
	t_vec2	src_pos;
	t_vec2	dst_pos;

	img.dest = mlx_new_image(md->mlx, src_size.x, src_size.y);
	if (img.dest == NULL)
		return (NULL);
	img.src_data = (int *)mlx_get_data_addr(src, &img.bpp, &src_pos.x, \
		&img.endian);
	img.dst_data = (int *)mlx_get_data_addr(img.dest, &img.bpp, &dst_pos.x, \
		&img.endian);
	img.pos.y = 0;
	while (img.pos.y < src_size.y)
	{
		img.pos.x = 0;
		while (img.pos.x < src_size.x)
		{
			src_pos.y = (img.pos.y * src_pos.x / 4) + img.pos.x;
			dst_pos.y = (img.pos.y * dst_pos.x / 4) + img.pos.x;
			img.dst_data[dst_pos.y] = img.src_data[src_pos.y];
			img.pos.x++;
		}
		img.pos.y++;
	}
	return (img.dest);
}
