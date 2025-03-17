/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:31:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 14:13:44 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

t_ent	*search_in_grid(t_md *md, t_ray *ray, float distance)
{
	int			index;
	t_ent_type	found_type;
	t_ent		*ent;

	index = (int)(ray->pos.x / md->t_len) + \
		((md->map.size.x + 1) * (int)(ray->pos.y / md->t_len));
	if (index <= 0 || index > md->map.len)
		return (NULL);
	ent = md->mapped_ents[index];
	if (!ent)
		return (NULL);
	found_type = ent->type;
	if (found_type == nt_wall)
		return (ent);
	if (ray->hit || found_type == nt_plr || found_type == nt_empty || \
		(ray->hit && ray->hit->row_draw_index >= ray->hit->size.x))
		return (NULL);
	if (!v3f_bounds(ray->pos, ent->pos, get_v3f(md->size_2d, md->size_2d, 0)))
		return (NULL);
	ray->pos_at_e = ray->pos;
	ray->hit = md->mapped_ents[index];
	ray->hit_vrt_at_e = ray->vertical_hit;
	ray->dist_at_e = distance;
	return (NULL);
}

int	iterate_steps(t_md *md, t_ray *ray, t_vec2 visu_offset)
{
	t_vec3f	grid_distance;
	t_ent	*hit;
	int		step;

	step = -1;
	while (++step < md->ray_depth)
	{
		ray->pos = get_v3f(ray->pos.x + ray->dir.x, \
			ray->pos.y + ray->dir.y, \
			ray->pos.z + ray->dir.z);
		grid_distance.x = fabs(fmod(ray->pos.x, md->t_len));
		grid_distance.y = fabs(fmod(ray->pos.y, md->t_len));
		if (grid_distance.x <= 1.3 || grid_distance.y <= 1.3)
			ray->vertical_hit = grid_distance.y > grid_distance.x;
		ray->color = md->rgb[RGB_VIOLET] + \
			(350 * (ray->vertical_hit == 1)) + \
			(350 * (ray->hit != NULL));
		hit = search_in_grid(md, ray, step);
		render_ray(md, ray, hit, visu_offset);
		if (!hit || hit->type != nt_wall)
			continue ;
		ray->wall_hit = hit;
		return (step);
	}
	return (-1);
}

void	cast_ray(t_md *md, t_ray *ray, t_vec2 visu_offset)
{
	int	iterated_steps;

	ray->hit = NULL;
	ray->vertical_hit = 0;
	ray->hit_vrt_at_e = 0;
	ray->distance = 0;
	ray->dist_at_e = 0;
	iterated_steps = iterate_steps(md, ray, visu_offset);
	if (!md->ray_mode)
		return ;
	if (ray->wall_hit)
		draw_wall_line(md, iterated_steps, ray->wall_hit, ray);
	if (ray->hit)
	{
		ray->vertical_hit = ray->hit_vrt_at_e;
		ray->pos = ray->pos_at_e;
		draw_sprite(md, ray->dist_at_e, ray->hit, ray);
	}
}

void	precompute_rays(t_md *md, float *cos_vals, float *sin_vals)
{
	float	fov;
	float	angle_step;
	float	yaw;
	int		i;
	float	ray_yaw;

	yaw = md->plr.rot.x * (M_PI / 180.0f);
	if (yaw < -M_PI)
		yaw += 2 * M_PI;
	else if (yaw >= M_PI)
		yaw -= 2 * M_PI;
	fov = (int)md->fov * (M_PI / 180.0f);
	angle_step = fov / (float)(md->win_size.x - 1);
	i = -1;
	while (++i < md->win_size.x)
	{
		ray_yaw = yaw - (fov / 2.0f) + (angle_step * i);
		cos_vals[i] = cosf(ray_yaw);
		sin_vals[i] = sinf(ray_yaw);
	}
}

void	cast_rays(t_md *md, t_vec3f start_pos)
{
	float	cos_vals[MAX_RAYS];
	float	sin_vals[MAX_RAYS];
	int		i;
	t_vec2	center_ray_visu_pos;

	center_ray_visu_pos = get_centered_ray_position(md);
	md->hud.floor_start = md->win_size.y;
	precompute_rays(md, cos_vals, sin_vals);
	i = -1;
	while (++i < md->win_size.x)
	{
		md->rays[i].index = i;
		md->rays[i].pos = start_pos;
		md->rays[i].angle = atan2f(sin_vals[i], cos_vals[i]);
		md->rays[i].dir = get_v3f(cos_vals[i], sin_vals[i], 0);
		cast_ray(md, &md->rays[i], center_ray_visu_pos);
	}
	if (md->plr.shot)
		launch_prt(md, &md->plr, md->plr.pos, md->rays[md->win_size.x / 2].dir);
	if (md->hud.floor_start < 0)
		md->hud.floor_start = 0;
}
