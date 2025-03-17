/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall_column.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:01:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 14:20:57 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

t_wrd_dir	determine_texture(t_ray *ray)
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

void	project_particle(t_md *md, t_image *img, t_vec2	pos)
{
	t_vec2	r_size;
	t_vec2	start_draw;

	r_size = get_v2(10, 10);
	start_draw = get_v2((int)pos.x - r_size.x / 2, (int)pos.y - r_size.y / 2);
	draw_sphere(img, start_draw, r_size, md->rgb[RGB_BLACK]);
}

int	draw_wall_column(t_md *md, t_image *img, t_vec2 win_p, t_vec3f img_coords)
{
	t_vec2	y;
	t_vec2	pxl;
	int		img_y;
	float	step;

	y.x = (md->win_size.y / 2 - img_coords.y / 2) - 1;
	step = img->size.y / img_coords.y;
	y.y = (md->win_size.y / 2 + img_coords.y / 2);
	while (++y.x < y.y)
	{
		img_y = (y.x - (md->win_size.y / 2 - img_coords.y / 2)) * step;
		if (img_y < 0 || img_y >= img->size.y)
			continue ;
		pxl.x = ((int)img_y * (img->size_line / 4)) + (int)img_coords.x;
		pxl.y = *(img->src_data + pxl.x);
		if ((pxl.y >> 24) != 0x00)
			continue ;
		draw_pixel(md->screen, get_v2(win_p.x, y.x + win_p.y - md->plr.pos.z), pxl.y, -1);
		if (md->plr.shot && win_p.x == md->win_size.x / 2 && \
			y.x + win_p.y - md->plr.pos.z == md->win_size.y / 2)
			project_particle(md, img, get_v2((int)img_coords.x, (int)img_y));
	}
	return (y.x + win_p.y - md->plr.pos.z);
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

void	draw_pxl(t_md *md, t_ray *ray, t_vec3f win_pos)
{
	t_vec2		draw_start;
	t_image		*img;
	t_wrd_dir	dir;
	float		vrt_offset;
	int			vertical_end;

	vrt_offset = compute_perspective_change(md, NULL, ray->distance);
	img = ray->wall_hit->frame;
	dir = -1;
	if (ray->wall_hit->type == nt_wall)
	{
		dir = determine_texture(ray);
		img = ray->wall_hit->frames[(int)dir];
	}
	draw_start = get_v2(ray->index, vrt_offset);
	vertical_end = draw_wall_column(md, img, draw_start, win_pos);
	if (vertical_end < md->hud.floor_start)
		md->hud.floor_start = vertical_end;
}

void	draw_wall_line(t_md *md, float dist, t_ent *wall, t_ray *ray)
{
	t_vec3f	win_pos;
	float	fisheye_corrector;

	ray->distance = maxf(0.01, dist);
	fisheye_corrector = (md->win_size.y * wall->size.y) / \
	(dist * fabs(cos((ray->angle - md->plr.angle) * (60.0 / (int)md->fov))));

	if (fisheye_corrector > md->win_size.y * 1.5)
		fisheye_corrector = md->win_size.y * 1.5;
	if (ray->vertical_hit)
		win_pos.x = (int)fmod(ray->pos.y, wall->size.y);
	else
		win_pos.x = (int)fmod(ray->pos.x, wall->size.x);
	win_pos.y = fisheye_corrector;
	draw_pxl(md, ray, win_pos);
}
