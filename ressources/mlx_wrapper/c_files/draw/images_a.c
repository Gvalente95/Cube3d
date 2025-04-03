/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images_a.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:31:53 by gvalente          #+#    #+#             */
/*   Updated: 2025/04/03 10:43:24 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

t_vec3f	set_new_size(t_image *q, t_vec2 *old_size, t_vec2 *new_size)
{
	double	aspect_ratio;
	t_vec3f	ratio;

	aspect_ratio = (double)old_size->x / old_size->y;
	if ((double)new_size->x / new_size->y > aspect_ratio)
		new_size->x = (int)(new_size->y * aspect_ratio);
	else
		new_size->y = (int)(new_size->x / aspect_ratio);
	ratio.x = (double)old_size->x / new_size->x;
	ratio.y = (double)old_size->y / new_size->y;
	return (ratio);
}

void	*scale_img_keep_ratio(t_md *md, void *img, \
	t_vec2 *old_size, t_vec2 new_size)
{
	t_image	m;
	t_vec3f	ratio;

	if (!img)
		return (printf("Error -> img to scale not find\n"), NULL);
	ratio = set_new_size(&m, old_size, &new_size);
	m.img = mlx_new_image(md->mlx, new_size.x, new_size.y);
	if (!m.img)
		return (NULL);
	m.src = (int *)mlx_get_data_addr(img, &m.bps, &m.size_line, &m.endian);
	m.scl_d = (int *)mlx_get_data_addr(m.img, &m.bps, &m.size_line, &m.endian);
	m.pos.y = -1;
	while (++m.pos.y < new_size.y)
	{
		m.pos.x = -1;
		while (++m.pos.x < new_size.x)
		{
			m.size.x = (int)(m.pos.x * (double)ratio.x);
			m.size.y = (int)(m.pos.y * (double)ratio.y);
			m.scl_d[m.pos.y * new_size.x + m.pos.x] = \
				m.src[m.size.y * old_size->x + m.size.x];
		}
	}
	mlx_destroy_image(md->mlx, img);
	return (img = m.img, *old_size = get_v2(new_size.x, new_size.y), m.img);
}

void	*resize_img(t_md *md, void *img, t_vec2 *old_size, t_vec2 new_size)
{
	t_image	q;
	t_vec3f	ratio;

	q.img = mlx_new_image(md->mlx, new_size.x, new_size.y);
	if (!q.img)
		return (NULL);
	q.src = (int *)mlx_get_data_addr(img, &q.bps, &q.size_line, &q.endian);
	q.scl_d = (int *)mlx_get_data_addr(q.img, &q.bps, &q.size_line, &q.endian);
	ratio.x = (float)old_size->x / new_size.x;
	ratio.y = (float)old_size->y / new_size.y;
	q.pos.y = -1;
	while (++q.pos.y < new_size.y)
	{
		q.pos.x = -1;
		while (++q.pos.x < new_size.x)
		{
			q.size.x = (int)(q.pos.x * (double)ratio.x);
			q.size.y = (int)(q.pos.y * (double)ratio.y);
			q.scl_d[q.pos.y * new_size.x + q.pos.x] = \
				q.src[q.size.y * old_size->x + q.size.x];
		}
	}
	mlx_destroy_image(md->mlx, img);
	*old_size = get_v2(new_size.x, new_size.y);
	return (q.img);
}

void	replace_image(t_md *md, t_image **dst, t_image *src)
{
	if (!md || !dst || !src)
		return ;
	if (*dst)
		free_image_data(md, *dst);
	*dst = copy_image(md, src, src->size, -1);
}

void	draw_random_pixel(t_image *img, int scale, int base_color, float rand)
{
	t_vec2	pos;
	t_vec4	clr;
	int		rand_amount;
	int		rand_rescale;

	pos.x = r_range(0, img->size.x);
	pos.y = r_range(0, img->size.y);
	if (rand > 0)
	{
		rand_amount = (int)(rand * 100);
		clr = color_to_v4(base_color);
		clr.r = minmax(0, 255, clr.r + r_range(-rand_amount, rand_amount));
		clr.g = minmax(0, 255, clr.g + r_range(-rand_amount, rand_amount));
		clr.b = minmax(0, 255, clr.b + r_range(-rand_amount, rand_amount));
		clr.a = 125;
		base_color = v4_to_color(clr.r, clr.g, clr.b, clr.a);
		rand_rescale = scale - (int)(scale * rand);
		scale += r_range(-rand_rescale, rand_rescale);
	}
	draw_pixels(img, pos, v2(scale), base_color);
}
