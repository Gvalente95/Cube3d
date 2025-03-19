/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:31:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/18 22:03:43 by giuliovalen      ###   ########.fr       */
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
	if (index <= 0 || index > md->map.len || ! md->mapped_ents[index])
		return (NULL);
	ent = md->mapped_ents[index];
	found_type = ent->type;
	if (found_type == nt_wall)
		return (ent);
	if (found_type == nt_plr || ray->hits_amount >= MAX_OVERLAP_SPRITE)
		return (NULL);
	if (ray->hits_amount > 0 && ray->hit_data[ray->hits_amount - 1].hit == ent)
		return (NULL);
	if (!v3f_bounds(ray->pos, get_v3f(0, 0, 0), \
		ent->pos, get_v3f(ent->frame->size.x + 1, ent->frame->size.y, 0)))
		return (NULL);
	ray->hit_data[ray->hits_amount].post_at_hit = ray->pos;
	ray->hit_data[ray->hits_amount].hit = md->mapped_ents[index];
	ray->hit_data[ray->hits_amount].vertical_hit_at_e = ray->vertical_hit;
	ray->hit_data[ray->hits_amount].dist_at_e = distance;
	ray->hits_amount++;
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
			(350 * (ray->vertical_hit == 1)) + (350 * (ray->hits_amount));
		if (step < 10)
			continue ;
		hit = search_in_grid(md, ray, step);
		render_ray(md, ray, hit, visu_offset);
		if (!hit || hit->type != nt_wall)
			continue ;
		return (ray->wall_hit = hit, step);
	}
	return (-1);
}

int	cast_ray(t_md *md, t_ray *ray, t_vec2 visu_offset)
{
	t_hit_data	*hit_data;
	int			iterated_steps;

	ray->hit_data[0].hit = NULL;
	ray->hits_amount = 0;
	ray->vertical_hit = 0;
	ray->distance = 0;
	iterated_steps = iterate_steps(md, ray, visu_offset);
	if (!md->real_mode)
		return (0);
	if (ray->wall_hit)
		draw_wall_line(md, iterated_steps, ray->wall_hit, ray);
	while (ray->hits_amount > 0)
	{
		hit_data = &ray->hit_data[ray->hits_amount - 1];
		if (!hit_data->hit)
			break ;
		ray->vertical_hit = hit_data->vertical_hit_at_e;
		ray->pos = hit_data->post_at_hit;
		draw_sprite(md, ray, *hit_data);
		hit_data->hit = NULL;
		ray->hits_amount--;
	}
	return (iterated_steps + 1);
}

void	compute_ray_directions(t_md *md, float *cos_vals, float *sin_vals)
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
	int		wall_hits;

	md->hud.new_floor_start = md->win_size.y;
	md->texture_accumulator = 0;
	wall_hits = 0;
	center_ray_visu_pos = get_centered_ray_position(md);
	compute_ray_directions(md, cos_vals, sin_vals);
	i = -1;
	while (++i < md->win_size.x)
	{
		md->rays[i].index = i;
		md->rays[i].pos = start_pos;
		md->rays[i].angle = atan2f(sin_vals[i], cos_vals[i]);
		md->rays[i].dir = get_v3f(cos_vals[i], sin_vals[i], 0);
		wall_hits += cast_ray(md, &md->rays[i], center_ray_visu_pos);
	}
	if (wall_hits)
		md->hud.floor_start = \
			minmax(0, md->win_size.x, md->hud.new_floor_start);
}
