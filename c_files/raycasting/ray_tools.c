/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 02:01:00 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/03 15:07:15 by giuliovalen      ###   ########.fr       */
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

	cam_2d_ofs.x = md->win_sz.x / 2 - md->txd.size_2d * 2;
	cam_2d_ofs.y = md->win_sz.y / 2 - md->txd.size_2d * 2;
	center_ofs.x = (md->cam_ofst.x / md->t_len * md->txd.size_2d);
	center_ofs.y = ((md->cam_ofst.y / md->t_len) * md->txd.size_2d);
	centr = get_v2(cam_2d_ofs.x - center_ofs.x, cam_2d_ofs.y - center_ofs.y);
	return (centr);
}

int	render_ray(t_md *md, t_ray *ray, t_vec2 visu_offset)
{
	t_vec2	_2d_pos;

	if (md->prm.ray_mode || !md->prm.show_rays)
		return (0);
	_2d_pos.x = visu_offset.x + (ray->pos.x / md->t_len) * md->txd.size_2d;
	_2d_pos.y = visu_offset.y + (ray->pos.y / md->t_len) * md->txd.size_2d;
	if (!is_in_screen(md, v2_to_v3(_2d_pos), get_v2(1, 1)))
		return (0);
	draw_pixel(md->screen, _2d_pos, ray->color, .3);
	return (1);
}

void	init_base_ray(t_ray *ray, int index, t_vec3f start_pos, float distance)
{
	ray->index = index;
	ray->check_hit = NULL;
	ray->had_door = 0;
	ray->check_steps = 0;
	ray->color = -1;
	ray->steps = -1;
	ray->start = start_pos;
	ray->pos = start_pos;
	ray->distance = distance;
	ray->flr_y = 9999;
	ray->teleported_once = 0;
}

int	validate_check_hit(t_md *md, t_ray *ray, t_ent *ent, t_ent_type type)
{
	if (type == nt_empty || type == nt_plr || ray->hits_len >= MAX_RAY_SPRITE)
		return (0);
	if (!md->prm.ent_mode && type != nt_door)
		return (0);
	if (ray->hits_len > 0 && ray->hit_data[ray->hits_len - 1].hit == ent)
		return (0);
	if (ray->check_hit && ray->check_hit != ent)
		return (0);
	if (type == nt_pickup && !ent->is_active)
		return (0);
	if (md->prm.use_thrd && ent->type != nt_door)
	{
		if (!md->prm.ray_mode || is_in_list(md->ray_manager.ents_to_draw, ent))
			return (0);
		if (!cmp_vec2f((t_vec2f){ray->pos.x, ray->pos.y}, \
	(t_vec2f){ent->pos.x + (float)(md->t_len / 2), \
	ent->pos.y + (float)(md->t_len / 2)}, .49))
			return (0);
		dblst_add_back(&md->ray_manager.ents_to_draw, dblst_new((t_ent *)ent));
		return (ent->hit_dist = ray->steps, ent->ray_hit_index = ray->index, 0);
	}
	else if (!v3f_bounds(ray->pos, v3f(0), \
		ent->pos, get_v3f(ent->frame->size.x + 1, ent->frame->size.y, 0)))
		return (0);
	return (1);
}
