/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:01:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/14 06:17:58 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	determine_texture(t_ray *ray)
{
	if (ray->hit_vrt)
	{
		if (ray->dir.x > 0)
			return ((int)EAST);
		return ((int)WEST);
	}
	else if (ray->dir.y > 0)
		return ((int)SOUTH);
	return ((int)NORTH);
}

void	draw_texture_pixels(t_md *md, t_image *img, t_vec2 size, t_vec2 start_pos, float height, t_vec2 txt_p)
{
	int		color;
	int		offset;
	int		y_pos;
	int		y_end;
	int		vertical_point;
	float	step;

	vertical_point = md->win_size.y;
	y_pos = (md->win_size.y / 2 - height / 2) - 1;
	step = size.y / height;
	y_end = (md->win_size.y / 2 + height / 2);
	while (++y_pos < y_end)
	{
		txt_p.y = (y_pos - (md->win_size.y / 2 - height / 2)) * step;
		if (txt_p.y < 0 || txt_p.y >= size.y)
			continue ;
		offset = ((int)txt_p.y * (img->size_line / 4)) + (int)txt_p.x;
		color = *(img->src_data + offset);
		if ((color >> 24) != 0x00)
			continue ;
		vertical_point = y_pos + start_pos.y - md->plr.pos.z;
		draw_pixel(md->screen.buffer, start_pos.x, y_pos + start_pos.y - md->plr.pos.z, color);
	}
	if (vertical_point < md->floor_start)
		md->floor_start = vertical_point;
}

int	compute_perspective_change(t_md *md, float *height, float ray_dst)
{
	float	prsp_scale;
	float	pitch_factor;
	int		pitch_offset;
	float	vertical_offset;
	float	denom;

	denom = ray_dst + fabs(md->plr.pos.y) + 1.0f;
	if (denom != 0)
		prsp_scale = md->win_size.y / denom;
	else
		prsp_scale = 1.0f;
	pitch_factor = tanf(md->plr.rot.y * (M_PI / 180.0f));
	if (height)
	{
		prsp_scale = (md->win_size.y) / (ray_dst + fabs(md->plr.pos.z) + 1.0f);
		*height *= prsp_scale;
	}
	vertical_offset = (md->plr.pos.z * md->win_size.y) / (ray_dst + 1.0f);
	pitch_offset = (-pitch_factor * md->win_size.y / 2) - vertical_offset;
	return (pitch_offset);
}

void	draw_pxl(t_md *md, t_ray *ray, float height, t_ent *wall, t_vec2 txt_p)
{
	t_vec2		start_pos;
	t_image		*img;
	float		vrt_offset;
	t_wrd_dir	dir;

	vrt_offset = compute_perspective_change(md, NULL, ray->distance);
	img = wall->frame;
	dir = -1;
	if (wall->type == nt_wall)
	{
		dir = determine_texture(ray);
		img = md->wall_txtr[dir];
	}
	start_pos = get_v2(ray->index, vrt_offset);
	draw_texture_pixels(md, img, wall->size, start_pos, height, txt_p);
}

void	draw_wall_line(t_md *md, float dist, t_ent *wall, t_ray *ray)
{
	t_vec2	txt_p;
	float	height;

	if (dist < 0.01)
		dist = 0.01;
	height = (md->win_size.y * wall->size.y) / \
		(dist * fabs(cos(ray->angle - md->plr.angle)));
	if (height > md->win_size.y * 1.5)
		height = md->win_size.y * 1.5;
	if (ray->hit_vrt)
		txt_p.x = (int)fmod(ray->pos.y, wall->size.y);
	else
		txt_p.x = (int)fmod(ray->pos.x, wall->size.x);
	ray->distance = dist;
	draw_pxl(md, ray, height, wall, txt_p);
}

// int	validate_ent_line(t_md *md, t_vec2 grid_pos, t_vec2 ray_pos, t_ent *e)
// {
	
// }

// void draw_entity_line(t_md *md, float dist, t_ent *col, t_ray *ray)
// {
	
// }