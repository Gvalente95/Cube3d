/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_visualize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 02:01:00 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 04:50:18 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	draw_blood(t_md *md, t_image *img, t_vec2	pos, int color)
{
	t_vec2	r_size;
	t_vec2	start_draw;

	r_size = get_v2(md->t_len / 10, md->t_len / 10);
	start_draw = get_v2((int)pos.x - r_size.x / 2, (int)pos.y - r_size.y / 2);
	draw_sphere(img, start_draw, r_size, color);
}

t_vec2	get_centered_ray_position(t_md *md)
{
	t_vec2	cam_2d_ofs;
	t_vec2	center_ofs;
	t_vec2	centr;

	cam_2d_ofs.x = md->win_size.x / 4 + md->size_2d / 2;
	cam_2d_ofs.y = md->win_size.y / 4 + md->size_2d / 2;
	center_ofs.x = (md->cam_ofst.x / md->t_len * md->size_2d);
	center_ofs.y = ((md->cam_ofst.y / md->t_len) * md->size_2d);
	centr = get_v2(cam_2d_ofs.x - center_ofs.x, cam_2d_ofs.y - center_ofs.y);
	return (centr);
}

static void	render_hit_position(t_md *md, t_ray *ray, t_vec2 _2d_position)
{
	t_vec2	draw_end;

	draw_end = get_v2(_2d_position.x + 3, _2d_position.y + 3);
	_2d_position = get_v2(_2d_position.x - 3, _2d_position.y - 3);
	while (++_2d_position.y < draw_end.y)
	{
		_2d_position.x = draw_end.x - 6;
		while (++_2d_position.x < draw_end.x)
			draw_pixel(md->screen, _2d_position, ray->color, -1);
	}
}

int	render_ray(t_md *md, t_ray *ray, t_ent *ray_hit, t_vec2 visu_offset)
{
	t_vec2	_2d_position;

	if (md->mmap.active && ray->index == md->win_size.x / 2 && \
			md->mmap.mray_len < MAX_RAYS - 1)
		md->mmap.ray_pos[md->mmap.mray_len++] = ray->pos;
	if (md->real_mode || !md->show_rays)
		return (0);
	_2d_position.x = visu_offset.x + (ray->pos.x / md->t_len) * md->size_2d;
	_2d_position.y = visu_offset.y + (ray->pos.y / md->t_len) * md->size_2d;
	if (!is_in_screen(md, v2_to_v3(_2d_position), get_v2(1, 1)))
		return (0);
	if (ray_hit)
		render_hit_position(md, ray, _2d_position);
	else
		draw_pixel(md->screen, _2d_position, ray->color, .3);
	return (1);
}
