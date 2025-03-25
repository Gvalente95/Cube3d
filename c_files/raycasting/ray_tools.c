/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 02:01:00 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 01:54:04 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	draw_blood(t_md *md, t_image *img, t_vec2 pos, int color)
{
	t_vec2	r_size;
	t_vec2	start_draw;
	t_vec3	sphere_data;

	r_size = v2((md->t_len / 30) * md->hud.wpn_index);
	start_draw = get_v2((int)pos.x - r_size.x / 2, (int)pos.y - r_size.y / 2);
	sphere_data = get_v3(color, 10, 0);
	draw_sphere(img, start_draw, r_size, sphere_data);
}

t_vec2	get_2d_ray_pos(t_md *md)
{
	t_vec2	cam_2d_ofs;
	t_vec2	center_ofs;
	t_vec2	centr;

	cam_2d_ofs.x = md->win_size.x / 2 - md->txd.size_2d * 2;
	cam_2d_ofs.y = md->win_size.y / 2 - md->txd.size_2d * 2;
	center_ofs.x = (md->cam_ofst.x / md->t_len * md->txd.size_2d);
	center_ofs.y = ((md->cam_ofst.y / md->t_len) * md->txd.size_2d);
	centr = get_v2(cam_2d_ofs.x - center_ofs.x, cam_2d_ofs.y - center_ofs.y);
	return (centr);
}

int	render_ray(t_md *md, t_ray *ray, t_ent *ray_hit, t_vec2 visu_offset)
{
	t_vec2	_2d_pos;

	(void)ray_hit;
	if (md->mmap.active && ray->index == md->win_size.x / 2 && \
			md->mmap.mray_len < MAX_RAYS - 1)
		md->mmap.ray_pos[md->mmap.mray_len++] = ray->pos;
	if (md->prm.real_mode || !md->prm.show_rays)
		return (0);
	_2d_pos.x = visu_offset.x + (ray->pos.x / md->t_len) * md->txd.size_2d;
	_2d_pos.y = visu_offset.y + (ray->pos.y / md->t_len) * md->txd.size_2d;
	if (!is_in_screen(md, v2_to_v3(_2d_pos), get_v2(1, 1)))
		return (0);
	draw_pixel(md->screen, _2d_pos, ray->color, .3);
	return (1);
}

int	cast_check_ray(t_md *md, t_ray *ray, t_vec3f start_pos, t_ent *check)
{
	t_ray	*new_ray;
	float	fov;
	float	angle_step;
	float	yaw;
	float	ray_yaw;

	if (!ray || !check || ray->index >= md->t_len * 3)
		return (0);
	new_ray = &md->rays[ray->index + 1];
	new_ray->index = ray->index + 1;
	new_ray->check_hit = check;
	new_ray->check_steps = ray->steps + 1;
	new_ray->pos = start_pos;
	new_ray->had_door = ray->had_door;
	yaw = (md->plr_rot.x) * (M_PI / 180.0f);
	if (yaw < -M_PI)
		yaw += 2 * M_PI;
	else if (yaw >= M_PI)
		yaw -= 2 * M_PI;
	fov = (int)md->prm.fov * (M_PI / 180.0f);
	angle_step = fov / (float)(md->win_size.x - 1);
	ray_yaw = yaw - (fov / 2.0f) + (angle_step * (-new_ray->index));
	new_ray->angle = atan2f(sinf(ray_yaw), cosf(ray_yaw));
	new_ray->dir = get_v3f(cosf(ray_yaw), sinf(ray_yaw), 0);
	return (cast_ray(md, new_ray, get_2d_ray_pos(md)));
}

void	init_base_ray(t_ray *ray, int index, t_vec3f start_pos, float distance)
{
	ray->index = index;
	ray->check_hit = NULL;
	ray->had_door = 0;
	ray->check_steps = 0;
	ray->color = -1;
	ray->steps = 0;
	ray->start = start_pos;
	ray->pos = start_pos;
	ray->distance = distance;
	ray->vrcl = 0;
	ray->teleported_once = 0;
}
