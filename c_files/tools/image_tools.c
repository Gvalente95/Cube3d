/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:02:48 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 13:38:50 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

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
