/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:31:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 12:43:48 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

t_ent	*search_in_grid(t_md *md, t_ray *ray, float distance)
{
	int			index;
	t_ent_type	type;
	t_ent		*ent;

	index = (int)(ray->pos.x / md->t_len) + \
		((md->map.size.x + 1) * (int)(ray->pos.y / md->t_len));
	if (index <= 0 || index >= md->map.len || !md->mapped_ents[index])
		return (NULL);
	ent = md->mapped_ents[index];
	type = ent->type;
	if (type == nt_wall)
		return (ent);
	if (!validate_check_hit(md, ray, ent, type))
		return (NULL);
	if (ray->index == 0 || ray->check_hit)
		cast_check_ray(md, ray, ray->start, ent);
	ray->hit_data[ray->hits_len].post_at_hit = ray->pos;
	ray->hit_data[ray->hits_len].hit = ent;
	ray->hit_data[ray->hits_len].vertical_hit_at_e = ray->vertical_hit;
	ray->hit_data[ray->hits_len].dist_at_e = distance;
	ray->hits_len++;
	if (ent->type == nt_door)
		ray->had_door = 1;
	return (NULL);
}

int	iterate_steps(t_md *md, t_ray *ray, t_vec2 visu_offset)
{
	t_ent	*hit;
	int		on_grid;

	ray->steps = -1;
	while (++ray->steps < md->prm.ray_depth)
	{
		ray->vrcl = 0;
		ray->distance++;
		ray->pos = get_v3f(ray->pos.x + ray->dir.x, \
			ray->pos.y + ray->dir.y, \
			ray->pos.z + ray->dir.z);
		on_grid = calculate_verticality(md, ray);
		if (ray->steps < md->prm.zoom)
			continue ;
		if (!on_grid && !md->prm.ent_mode)
			continue ;
		hit = search_in_grid(md, ray, ray->steps);
		render_ray(md, ray, hit, visu_offset);
		if (hit && ray->check_hit && ray->check_hit == hit)
			return (-1);
		if (ray->check_hit && ray->steps > ray->check_steps + 2)
			return (-1);
		if (!hit || hit->type != nt_wall || ray->check_hit)
			continue ;
		return (ray->wall_hit = hit, ray->steps);
	}
	return (-1);
}

int	cast_ray(t_md *md, t_ray *ray, t_vec2 visu_offset)
{
	t_hit_data	*hit_data;
	int			wall_collision;

	ray->hit_data[0].hit = NULL;
	ray->hits_len = 0;
	ray->vertical_hit = 0;
	wall_collision = iterate_steps(md, ray, visu_offset);
	if (!md->prm.real_mode)
		return (0);
	if (!ray->check_hit && ray->wall_hit)
		draw_wall_line(md, ray->distance, ray->wall_hit, ray);
	while (ray->hits_len > 0 && \
		(!ray->check_hit || ray->check_hit->type != nt_door))
	{
		hit_data = &ray->hit_data[ray->hits_len - 1];
		if (!hit_data->hit)
			break ;
		ray->vertical_hit = hit_data->vertical_hit_at_e;
		ray->pos = hit_data->post_at_hit;
		draw_sprite(md, ray, *hit_data);
		hit_data->hit = NULL;
		ray->hits_len--;
	}
	return (wall_collision + 1);
}

void	compute_ray_directions(t_md *md, t_vec3f *dir_vals, int rays_amount)
{
	float	fov;
	float	angle_step;
	float	yaw;
	int		i;
	float	ray_yaw;

	yaw = md->plr_rot.x * (M_PI / 180.0f);
	if (yaw < -M_PI)
		yaw += 2 * M_PI;
	else if (yaw >= M_PI)
		yaw -= 2 * M_PI;
	fov = (int)md->prm.fov * (M_PI / 180.0f);
	angle_step = fov / (float)(md->win_size.x - 1);
	i = -1;
	while (++i < rays_amount)
	{
		ray_yaw = yaw - (fov / 2.0f) + (angle_step * i);
		dir_vals[i].x = cosf(ray_yaw);
		dir_vals[i].y = sinf(ray_yaw);
		dir_vals[i].z = atan2f(dir_vals[i].y, dir_vals[i].x);
	}
}

void	cast_rays(t_md *md, t_vec3f start)
{
	t_vec3f	dir_vals[MAX_RAYS];
	t_vec2	center_ray_visu_pos;
	int		i;
	int		wall_hits;

	md->hud.new_floor_start = md->win_size.y;
	wall_hits = 0;
	center_ray_visu_pos = get_2d_ray_pos(md);
	compute_ray_directions(md, dir_vals, md->win_size.x);
	i = -1;
	while (++i < md->win_size.x)
	{
		init_base_ray(&md->rays[i], i, start, 0);
		md->rays[i].dir = get_v3f(dir_vals[i].x, dir_vals[i].y, 0);
		md->rays[i].angle = dir_vals[i].z;
		wall_hits += cast_ray(md, &md->rays[i], center_ray_visu_pos);
	}
	if (wall_hits)
		md->hud.floor_start = \
			minmax(0, md->win_size.x, md->hud.new_floor_start);
}
