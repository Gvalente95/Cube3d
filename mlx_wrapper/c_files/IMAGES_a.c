/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMAGES_a.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:31:53 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/05 13:22:02 by giuliovalen      ###   ########.fr       */
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
	t_image	q;

	if (!img)
		return (printf("Error -> img to scale not find\n"), NULL);
	set_new_size(&q, old_size, &new_size);
	q.dest = mlx_new_image(md->mlx, new_size.x, new_size.y);
	if (!q.dest)
		return (NULL);
	q.src_data = (int *)mlx_get_data_addr(img, &q.bps, &q.len, &q.endian);
	q.scl_d = (int *)mlx_get_data_addr(q.dest, &q.bps, &q.len, &q.endian);
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
	return (img = q.dest, *old_size = get_v2(new_size.x, new_size.y), q.dest);
}

void	*scale_abs_img(t_md *md, void *img, t_vec2 *old_size, t_vec2 new_size)
{
	t_image	q;

	q.dest = mlx_new_image(md->mlx, new_size.x, new_size.y);
	if (!q.dest)
		return (NULL);
	q.src_data = (int *)mlx_get_data_addr(img, &q.bps, &q.len, &q.endian);
	q.scl_d = (int *)mlx_get_data_addr(q.dest, &q.bps, &q.len, &q.endian);
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
	return (q.dest);
}

void	*add_img(char *relative_path, int *width, int *height, t_md *md)
{
	void	*img;

	if (!relative_path)
	{
		ft_printf("Tried to add image with null path\n");
		return (NULL);
	}
	if (!md)
	{
		ft_printf("Error: mlx instance is NULL\n");
		return (NULL);
	}
	img = mlx_png_file_to_image(md->mlx, relative_path, width, height);
	if (!img)
	{
		ft_printf("ERROR: Failed to load image at path: %s\n", relative_path);
		return (NULL);
	}
	return (img);
}

void	render_cursor(t_md *md, int has_hov)
{
	if (md->mouse_pressed && has_hov)
		mlx_put_image_to_window(md->mlx, md->win, \
md->cursor_grab, md->mouse_pos.x, md->mouse_pos.y);
	else if (has_hov)
		mlx_put_image_to_window(md->mlx, md->win, \
md->cursor_detect, md->mouse_pos.x, md->mouse_pos.y);
	else
		mlx_put_image_to_window(md->mlx, md->win, \
md->cursor, md->mouse_pos.x, md->mouse_pos.y);
}
