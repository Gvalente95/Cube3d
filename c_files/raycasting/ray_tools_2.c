/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_tools_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 02:04:12 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 12:44:42 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	calculate_verticality(t_md *md, t_ray *ray)
{
	t_vec2f	off;
	t_vec2f	edge_dist;
	float	tile;
	int		is_on_grid;

	is_on_grid = 0;
	tile = md->t_len;
	off = get_v2f(fmodf(ray->pos.x, tile), fmodf(ray->pos.y, tile));
	if (off.x < 0)
		off.x += tile;
	if (off.y < 0)
		off.y += tile;
	edge_dist = get_v2f(fminf(off.x, tile - off.x), fminf(off.y, tile - off.y));
	if (edge_dist.x <= 1 || edge_dist.y <= 1)
	{
		ray->vertical_hit = edge_dist.y > edge_dist.x;
		if (ray->index > 0 && fabsf(edge_dist.x - edge_dist.y) < 1.0f)
			ray->vertical_hit = md->rays[ray->index - 1].vertical_hit;
		is_on_grid = 1;
	}
	if (md->prm.real_mode || !md->prm.show_rays)
		return (is_on_grid);
	ray->color = md->rgb[RGB_GREEN + ray->hits_len] + \
	(350 * (ray->vertical_hit));
	if (ray->check_hit != NULL)
		ray->color = md->rgb[RGB_GOLD];
	if (ray->vrcl)
		ray->color = md->rgb[RGB_RED];
	return (is_on_grid);
}

int	validate_check_hit(t_md *md, t_ray *ray, t_ent *ent, t_ent_type type)
{
	if (type == nt_plr || ray->hits_len >= MAX_RAY_SPRITE)
		return (0);
	if (!md->prm.ent_mode && type != nt_door)
		return (0);
	if (ray->hits_len > 0 && ray->hit_data[ray->hits_len - 1].hit == ent)
		return (0);
	if (ray->check_hit && ray->check_hit != ent)
		return (0);
	if (type == nt_pickup && !ent->is_active)
		return (0);
	if (!v3f_bounds(ray->pos, v3f(0), \
		ent->pos, get_v3f(ent->frame->size.x + 1, ent->frame->size.y, 0)))
		return (0);
	return (1);
}

int	correct_fisheye(t_md *md, t_ray *ray, t_ent *e, float dist)
{
	float	fov_correction_factor;
	float	angle_diff;
	float	angle_projection;
	float	projection_plane_height;
	float	fisheye_corrector;

	fov_correction_factor = 60.0f / (float)md->prm.fov;
	angle_diff = (ray->angle - md->plr.angle) * fov_correction_factor;
	angle_projection = fabsf(cosf(angle_diff));
	projection_plane_height = md->win_size.y * e->size.y;
	fisheye_corrector = projection_plane_height / (dist * angle_projection);
	if (fisheye_corrector > md->win_size.y * 1.5)
		fisheye_corrector = md->win_size.y * 1.5;
	return (fisheye_corrector);
}