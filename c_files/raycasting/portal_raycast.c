/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_raycast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 01:58:28 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 12:16:57 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	dir_to_rotation(t_wrd_dir a, t_wrd_dir b)
{
	if (a == b)
		return (2);
	if ((a == NORTH && b == SOUTH) || (a == SOUTH && b == NORTH) || \
	(a == EAST && b == WEST) || (a == WEST && b == EAST))
		return (0);
	if ((a == NORTH && b == EAST) || (a == EAST && b == SOUTH) || \
		(a == SOUTH && b == WEST) || (a == WEST && b == NORTH))
		return (1);
	return (3);
}

void	rotate_90_deg(t_vec3f *vec, int times)
{
	float	x;
	float	y;

	times = (times % 4 + 4) % 4;
	x = vec->x;
	y = vec->y;
	if (times == 1)
	{
		vec->x = y;
		vec->y = -x;
	}
	else if (times == 2)
	{
		vec->x = -x;
		vec->y = -y;
	}
	else if (times == 3)
	{
		vec->x = -y;
		vec->y = x;
	}
}

int	check_portal_validity(t_md *md, t_ray *ray, t_ent *portal)
{
	if (ray->teleported_once > 2)
		return (0);
	if (!md->portal_gun.entrance || !md->portal_gun.exit)
		return (0);
	if (portal != md->portal_gun.entrance && portal != md->portal_gun.exit)
		return (0);
	return (1);
}

int	translate_ray(t_md *md, t_ray *ray, t_ent *portal, float distance)
{
	t_ent	*src;
	t_ent	*dst;
	t_vec3f	rel_pos;
	int		rot_offset;

	if (!md->portal_gun.entrance || !md->portal_gun.exit)
		return (0);
	if (portal != md->portal_gun.entrance && portal != md->portal_gun.exit)
		return (0);
	src = md->portal_gun.entrance;
	dst = md->portal_gun.exit;
	if (portal == md->portal_gun.exit)
	{
		src = md->portal_gun.exit;
		dst = md->portal_gun.entrance;
	}
	rel_pos = get_v3f(ray->pos.x - src->pos.x, ray->pos.y - src->pos.y, 0);
	rot_offset = dir_to_rotation(src->overlay_dir, dst->overlay_dir);
	rotate_90_deg(&rel_pos, rot_offset);
	rotate_90_deg(&ray->dir, rot_offset);
	int	val = md->t_len * 1;
	ray->pos.x = dst->pos.x + rel_pos.x + ray->dir.x * val;
	ray->pos.y = dst->pos.y + rel_pos.y + ray->dir.y * val;
	init_base_ray(ray, ray->index, ray->pos, distance + val);
	return (ray->teleported_once++, cast_ray(md, ray, get_2d_ray_pos(md)), 1);
}
