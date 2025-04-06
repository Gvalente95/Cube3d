/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_move.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:28:02 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/04 13:02:22 by giuliovalen      ###   ########.fr       */
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
	if (!ent->revealed && ray->distance < md->t_len * REVEAL_DISTANCE)
		show_minimap_entity(md, ent, md->mmap.bg, 1);
	type = ent->type;
	if (type == nt_wall)
		return (ent);
	if (!validate_check_hit(md, ray, ent, type))
		return (NULL);
	if (!md->prm.use_thrd && (ray->index == 0 || ray->check_hit))
		cast_check_ray(md, ray, ray->start, ent);
	ray->hit_data[ray->hits_len].post_at_hit = ray->pos;
	ray->hit_data[ray->hits_len].vertical_hit_at_e = ray->vertical_hit;
	ray->hit_data[ray->hits_len].dist_at_e = distance;
	ray->hit_data[ray->hits_len++].hit = ent;
	if (ent->type == nt_door)
		ray->had_door = 1;
	return (NULL);
}

static int	ray_can_look(t_md *md, t_ray *ray, int on_grid)
{
	if (ray->steps < md->prm.zoom)
		return (0);
	if (!on_grid && !md->prm.ent_mode)
		return (0);
	return (1);
}

static int	ray_can_stop(t_ray *ray, t_ent *hit)
{
	if (hit && ray->check_hit && ray->check_hit == hit)
		return (1);
	if (ray->check_hit && ray->steps > ray->init_steps + 2)
		return (1);
	return (0);
}

int	ray_move(t_md *md, t_ray *ray, t_vec2 visu_offset)
{
	t_ent	*hit;
	int		on_grid;

	while (++ray->steps < md->prm.ray_depth)
	{
		ray->distance++;
		ray->pos = add_vec3f(ray->pos, ray->dir);
		on_grid = update_ray_grid_pos(md, ray);
		if (md->prm.view_2d && md->prm.show_rays)
			set_ray_color(md, ray);
		if (!ray_can_look(md, ray, on_grid))
			continue ;
		hit = search_in_grid(md, ray, ray->steps);
		render_ray(md, ray, visu_offset);
		if (ray_can_stop(ray, hit))
			return (-1);
		if (hit && hit->type == nt_wall && !ray->check_hit)
			return (ray->wall_hit = hit, ray->steps);
	}
	ray->wall_hit = NULL;
	return (-1);
}
