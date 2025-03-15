/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprite_column.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:20:47 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/15 02:24:29 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	draw_column(t_md *md, t_image *img, t_vec2 start_pos, t_vec2 win_pos)
{
	t_vec2	y;
	t_vec2	draw_position;
	int		pixel;
	int		pixel_index;
	float	step;

	y.x = (md->win_size.y / 2 - win_pos.y / 2) - 1;
	y.y = (md->win_size.y / 2 + win_pos.y / 2);
	step = img->size.y / win_pos.y;
	while (++y.x < y.y)
	{
		win_pos.y = (y.y - (md->win_size.y / 2 - win_pos.y / 2)) * step;
		if (win_pos.y < 0 || win_pos.y >= img->size.y)
			continue ;
		pixel_index = ((int)win_pos.y * (img->size_line / 4)) + win_pos.x;
		pixel = *(img->src_data + pixel_index);
		if ((pixel >> 24) != 0x00)
			continue ;
		draw_position = get_v2(start_pos.x, y.y + start_pos.y - md->plr.pos.z);
		draw_pixel(md->screen, draw_position, pixel, -1);
	}
}

void	draw_sprite_pixel(t_md *md, t_ray *ray, t_vec2 screen_position)
{
	t_vec2	draw_start;
	t_image	*img;
	t_ent	*sprite;
	float	y_offset;

	sprite = ray->hit;
	img = md->e_frms[sprite->type][0][0];
	y_offset = compute_perspective_change(md, NULL, ray->distance);
	draw_start = get_v2(ray->index, y_offset);
	draw_column(md, img, draw_start, screen_position);
}

void	draw_sprite(t_md *md, float dist, t_ray *ray)
{
	t_vec2	screen_pos;
	t_ent	*sprite;
	int		sprite_height;
	float	fisheye_correct;
	int		sprite_with;

	dist = maxf(0.1, dist);
	ray->distance = dist;
	sprite = ray->hit;
	sprite_height = md->e_sizes[sprite->type].y;
	fisheye_correct = maxf(0.1, dist * fabs(cos(ray->angle - md->plr.angle)));
	screen_pos.y = (md->win_size.y * sprite_height) / fisheye_correct;
	screen_pos.y = minf(md->win_size.y * 1.5, screen_pos.y);
	sprite_with = sprite->size.x;
	if (ray->vertical_hit)
		screen_pos.x = (int)((ray->pos.y / sprite_height) * sprite_with);
	else
		screen_pos.x = (int)((ray->pos.x / sprite_with) * sprite_with);
	draw_sprite_pixel(md, ray, screen_pos);
}
