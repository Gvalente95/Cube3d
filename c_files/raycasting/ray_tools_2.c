/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_tools_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 02:04:12 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/01 01:51:33 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	set_ray_color(t_md *md, t_ray *ray)
{
	ray->color = md->rgb[RGB_GREEN + ray->hits_len] + \
	(350 * (ray->vertical_hit));
	if (ray->check_hit != NULL)
		ray->color = md->rgb[RGB_GOLD];
}

int	update_ray_grid_pos(t_md *md, t_ray *ray)
{
	const float	tlen = md->t_len;
	t_vec2f		off;
	t_vec2f		edge_dist;
	int			is_on_grid;

	ray->corner = 0;
	is_on_grid = 0;
	off = get_v2f(fmodf(ray->pos.x, tlen), fmodf(ray->pos.y, tlen));
	if (off.x < 0)
		off.x += tlen;
	if (off.y < 0)
		off.y += tlen;
	edge_dist = get_v2f(fminf(off.x, tlen - off.x), fminf(off.y, tlen - off.y));
	if (edge_dist.x <= 1 || edge_dist.y <= 1)
	{
		is_on_grid = 1;
		ray->vertical_hit = edge_dist.y > edge_dist.x;
		if (ray->index > 0 && fabsf(edge_dist.x - edge_dist.y) < 1.0f)
		{
			ray->corner = 1;
			ray->vertical_hit = md->rays[ray->index - 1].vertical_hit;
		}
	}
	if (!md->prm.ray_mode && md->prm.show_rays)
		set_ray_color(md, ray);
	return (is_on_grid);
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
	if (md->prm.height > 0)
	{
		float vertical_scale = md->t_len / (md->t_len + (e->pos.z - md->cam_pos.z));
		projection_plane_height = md->win_sz.y * e->size.y * vertical_scale;
	}
	else
		projection_plane_height = md->win_sz.y * e->size.y;
	fisheye_corrector = projection_plane_height / (dist * angle_projection);
	if (fisheye_corrector > md->win_sz.y * 1.5)
		fisheye_corrector = md->win_sz.y * 1.5;
	return (fisheye_corrector);
}

int	get_wall_orientation(t_ray *ray)
{
	if (ray->vertical_hit)
	{
		if (ray->dir.x > 0)
			return (EAST);
		return (WEST);
	}
	else if (ray->dir.y > 0)
		return (SOUTH);
	return (NORTH);
}

int	compute_row_start(t_md *md, t_ent *e, float ray_dst)
{
	float	pitch_factor;
	int		pitch_offset;
	float	vertical_offset;

	(void)e;
	pitch_factor = tanf(md->plr_rot.y * (M_PI / 180.0f));
	vertical_offset = (md->cam_pos.z * md->win_sz.y) / (ray_dst + 1.0f);
	pitch_offset = (-pitch_factor * md->win_sz.y / 2) - vertical_offset;
	return (pitch_offset);
}
