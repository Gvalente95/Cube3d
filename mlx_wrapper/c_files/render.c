/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:55:24 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/14 20:02:13 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

void	flush_img(t_image *src, int color, float transp)
{
	int		*img_data;
	int		total_pixels;
	int		i;

	if (!src || !src->src_data)
		return ;
	if (color == -1)
		color = 0x000000;
	img_data = src->src_data;
	total_pixels = src->size.x * src->size.y;
	i = -1;
	while (++i < total_pixels)
	{
		if (transp == -1)
			img_data[i] = color;
		else
			img_data[i] = blend_color(img_data[i], color, transp);
	}
}

int	is_in_screen(t_md *md, t_vec3 pos, t_vec2 size)
{
	return (pos.x > -size.x && pos.x < md->win_size.x + \
		size.x && pos.y > -size.y && pos.y < md->win_size.y + size.y);
}
