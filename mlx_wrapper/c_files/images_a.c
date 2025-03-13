/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMAGES_a.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:31:53 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/13 19:54:37 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

void	set_new_size(t_image *q, t_vec2 *old_size, t_vec2 *new_size)
{
	double	aspect_ratio;

	aspect_ratio = (double)old_size->x / old_size->y;
	if ((double)new_size->x / new_size->y > aspect_ratio)
		new_size->x = (int)(new_size->y * aspect_ratio);
	else
		new_size->y = (int)(new_size->x / aspect_ratio);
	q->x_ratio = (double)old_size->x / new_size->x;
	q->y_ratio = (double)old_size->y / new_size->y;
}

void	*scale_img(t_md *md, void *img, t_vec2 *old_size, t_vec2 new_size)
{
	t_image	m;

	if (!img)
		return (printf("Error -> img to scale not find\n"), NULL);
	set_new_size(&m, old_size, &new_size);
	m.img = mlx_new_image(md->mlx, new_size.x, new_size.y);
	if (!m.img)
		return (NULL);
	m.src_data = (int *)mlx_get_data_addr(img, &m.bps, &m.size_line, &m.endian);
	m.scl_d = (int *)mlx_get_data_addr(m.img, &m.bps, &m.size_line, &m.endian);
	m.pos.y = -1;
	while (++m.pos.y < new_size.y)
	{
		m.pos.x = -1;
		while (++m.pos.x < new_size.x)
		{
			m.size.x = (int)(m.pos.x * m.x_ratio);
			m.size.y = (int)(m.pos.y * m.y_ratio);
			m.scl_d[m.pos.y * new_size.x + m.pos.x] = \
				m.src_data[m.size.y * old_size->x + m.size.x];
		}
	}
	mlx_destroy_image(md->mlx, img);
	return (img = m.img, *old_size = get_v2(new_size.x, new_size.y), m.img);
}

void	*scale_abs_img(t_md *md, void *img, t_vec2 *old_size, t_vec2 new_size)
{
	t_image	q;

	q.img = mlx_new_image(md->mlx, new_size.x, new_size.y);
	if (!q.img)
		return (NULL);
	q.src_data = (int *)mlx_get_data_addr(img, &q.bps, &q.size_line, &q.endian);
	q.scl_d = (int *)mlx_get_data_addr(q.img, &q.bps, &q.size_line, &q.endian);
	q.x_ratio = (float)old_size->x / new_size.x;
	q.y_ratio = (float)old_size->y / new_size.y;
	q.pos.y = -1;
	while (++q.pos.y < new_size.y)
	{
		q.pos.x = -1;
		while (++q.pos.x < new_size.x)
		{
			q.size.x = (int)(q.pos.x * q.x_ratio);
			q.size.y = (int)(q.pos.y * q.y_ratio);
			q.scl_d[q.pos.y * new_size.x + q.pos.x] = \
				q.src_data[q.size.y * old_size->x + q.size.x];
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
