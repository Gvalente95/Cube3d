/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images_a.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:31:53 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/25 02:31:57 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

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

void	*add_img(char *relative_path, int *width, int *height, t_md *md)
{
	void	*img;

	if (!relative_path)
	{
		ft_printf("Tried to add image with null path\n");
		return (NULL);
	}
	if (access(relative_path, F_OK) == -1)
		return (printf("no file for %s\n", relative_path), NULL);
	img = md->mlx_make(md->mlx, relative_path, width, height);
	return (img);
}

void	replace_image(t_md *md, t_image **dst, t_image *src)
{
	if (!md || !dst || !src)
		return ;
	if (*dst)
		free_image_data(md, *dst);
	*dst = copy_image(md, src, src->size, -1);
}
