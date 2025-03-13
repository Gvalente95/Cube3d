/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:02:48 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/13 05:13:30 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

//	d = (start.x, start.y, end.x, end.y)
void	color_img(void *frame, t_vec2 size, int col, t_vec4 d)
{
	t_image	q;
	int		pixel_index;

	q.src_data = (int *)mlx_get_data_addr(frame, &q.bpp, &q.len, &q.endian);
	q.pos.y = -1;
	while (++q.pos.y < size.y)
	{
		q.pos.x = -1;
		while (++q.pos.x < size.x)
		{
			if (q.pos.x < d.r || q.pos.x > d.b || \
				q.pos.y < d.g || q.pos.y > d.a)
				continue ;
			pixel_index = (q.pos.y * q.len / 4) + q.pos.x;
			q.src_data[pixel_index] = col;
		}
	}
}

int	my_put_pixel(void *frame, int x, int y, int col)
{
	t_image	q;
	int		pixel_index;

	if (!frame)
		return (0);
	q.src_data = (int *)mlx_get_data_addr(frame, &q.bpp, &q.len, &q.endian);
	if (!q.src_data)
		return (0);
	if (x < 0 || y < 0 || x >= q.len / (q.bpp / 8) || y >= q.len / (q.len / 4))
		return (0);
	pixel_index = (y * q.len / 4) + x;
	q.src_data[pixel_index] = col;
	return (1);
}

int	is_in_screen(t_md *md, t_vec3 pos, t_vec2 size)
{
	return (pos.x > -size.x && pos.x < md->win_size.x + \
		size.x && pos.y > -size.y && pos.y < md->win_size.y + size.y);
}
