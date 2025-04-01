/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:31:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/31 13:33:56 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

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
	angle_step = fov / (float)(md->win_sz.x - 1);
	ray_yaw = yaw - (fov / 2.0f) + (angle_step * (-new_ray->index));
	new_ray->angle = atan2f(sinf(ray_yaw), cosf(ray_yaw));
	new_ray->dir = get_v3f(cosf(ray_yaw), sinf(ray_yaw), 0);
	return (cast_ray(md, new_ray, get_2d_ray_pos(md)));
}

int	cast_ray(t_md *md, t_ray *ray, t_vec2 visu_offset)
{
	t_hit_data	*hit_data;
	int			wall_collision;

	ray->hit_data[0].hit = NULL;
	ray->hits_len = 0;
	ray->vertical_hit = 0;
	wall_collision = ray_move(md, ray, visu_offset);
	if (!md->prm.ray_mode)
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
	angle_step = fov / (float)(md->win_sz.x - 1);
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

	md->hud.new_floor_start = md->win_sz.y;
	wall_hits = 0;
	center_ray_visu_pos = get_2d_ray_pos(md);
	compute_ray_directions(md, dir_vals, md->win_sz.x);
	i = -1;
	while (++i < md->win_sz.x)
	{
		init_base_ray(&md->rays[i], i, start, 0);
		md->rays[i].dir = get_v3f(dir_vals[i].x, dir_vals[i].y, 0);
		md->rays[i].angle = dir_vals[i].z;
		wall_hits += cast_ray(md, &md->rays[i], center_ray_visu_pos);
	}
	if (wall_hits)
		md->hud.floor_start = \
			minmax(0, md->win_sz.x, md->hud.new_floor_start);
}

int	is_in_list(t_dblst *lst, t_ent *e)
{
	t_dblst	*node;
	t_ent	*node_e;

	node = dblst_first(lst);
	while (node)
	{
		node_e = (t_ent *)node->content;
		if (node_e == e)
			return (1);
		node = node->next;
	}
	return (0);
}
