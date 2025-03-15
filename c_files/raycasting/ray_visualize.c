/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_visualize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 02:01:00 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/15 03:14:55 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	render_ray(t_md *md, t_ray *ray, t_ent *col)
{
	t_vec3	ray_2dp;
	t_vec2	draw_end;
	t_vec2	cam_2d_ofs;
	t_vec2	center_ofs;
	t_vec2	centr;

	if (md->mmap.active && ray->index == md->win_size.x / 2 && \
			md->mmap.mray_len < MAX_RAYS - 1)
		md->mmap.ray_pos[md->mmap.mray_len++] = ray->pos;
	if (md->ray_mode || !md->show_rays)
		return (0);
		cam_2d_ofs = get_v2(md->cam_ofst.x / md->t_len, md->cam_ofst.y / md->t_len);
	center_ofs = get_v2(cam_2d_ofs.x * md->size_2d, cam_2d_ofs.y * md->size_2d);
	centr.x = md->win_size.x / 4 + md->size_2d / 2 - center_ofs.x;
	centr.y = md->win_size.y / 4 + md->size_2d / 2 - center_ofs.y;
	ray_2dp.x = centr.x + (ray->pos.x / md->t_len) * md->size_2d;
	ray_2dp.y = centr.y + (ray->pos.y / md->t_len) * md->size_2d;
	ray_2dp.z = 0;
	if (!is_in_screen(md, ray_2dp, get_v2(1, 1)))
		return (0);
	if (!col)
		return (draw_pixel(md->screen, get_v2(ray_2dp.x, ray_2dp.y), ray->color, -1));
	draw_end = get_v2(ray_2dp.x + 3, ray_2dp.y + 3);
	ray_2dp = get_v3(ray_2dp.x - 3, ray_2dp.y - 3, 0);
	while (++ray_2dp.y < draw_end.y)
	{
		ray_2dp.x = draw_end.x - 6;
		while (++ray_2dp.x < draw_end.x)
			draw_pixel(md->screen, get_v2(ray_2dp.x, ray_2dp.y), ray->color, -1);
	}
	return (1);
}
