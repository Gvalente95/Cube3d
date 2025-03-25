/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall_column.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:01:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 11:39:17 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static t_wrd_dir	get_wall_orientation(t_ray *ray)
{
	if (ray->vrcl)
		return (EAST);
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

static int	draw_strip(t_md *md, t_ray *ray, t_vec3 win_p, t_vec3f img_coords, int first_pass, int *has_portal)
{
	t_vec2	y;
	t_vec3	pxl;
	float	step;
	t_vec2	win_draw_pos;
	t_image	*img;
	int		rgb;

	img = ray->wall_hit->frame;
	y.x = (md->win_size.y / 2 - img_coords.y / 2) - 1;
	step = img->size.y / img_coords.y;
	y.y = (md->win_size.y / 2 + img_coords.y / 2);
	while (++y.x < y.y)
	{
		pxl.z = (y.x - (md->win_size.y / 2 - img_coords.y / 2)) * step;
		if (pxl.z < 0 || pxl.z >= img->size.y)
			continue ;
		pxl.x = ((int)pxl.z * (img->size_line / 4)) + (int)img_coords.x;
		pxl.y = *(img->src + pxl.x);
		win_draw_pos = get_v2(win_p.x, y.x + win_p.y - md->cam_pos.z);
		if ((pxl.y >> 24) != 0x00)
			continue ;
		rgb = pxl.y & 0x00FFFFFF;
		if ((rgb == 0x0000FF || rgb == 0xFF0000) && check_portal_validity(md, ray, ray->wall_hit))
		{
			if (first_pass)
				*has_portal = 1;
			else if (translate_ray(md, ray, ray->wall_hit, ray->distance))
				return (md->win_size.y);
			continue ;
		}
		draw_pixel(md->screen, win_draw_pos, pxl.y, -1);
		if (md->fx.fog)
			draw_pixel(md->screen, win_draw_pos, md->rgb[RGB_BLACK], \
		minmaxf(0, .9, ((ray->distance / md->t_len) / 10) * md->fx.fog));
		if (md->plr.shot && win_p.x == md->win_size.x / 2 && y.x + win_p.y - md->plr.pos.z == md->win_size.y / 2)
			draw_portal(md, ray->wall_hit, get_v2((int)img_coords.x, (int)pxl.z));
	}
	return (y.x + win_p.y - md->cam_pos.z);
}

int	compute_row_start(t_md *md, float ray_dst)
{
	float	pitch_factor;
	int		pitch_offset;
	float	vertical_offset;

	pitch_factor = tanf(md->plr_rot.y * (M_PI / 180.0f));
	vertical_offset = (md->cam_pos.z * md->win_size.y) / (ray_dst + 1.0f);
	pitch_offset = (-pitch_factor * md->win_size.y / 2) - vertical_offset;
	return (pitch_offset);
}

static int	draw_pxl(t_md *md, t_ray *ray, \
	t_vec3f txtr_coord, t_vec3 screen_pos)
{
	t_ent		*wall;
	t_image		*img;
	t_wrd_dir	dir;
	int			vertical_end;
	int			portal;

	wall = ray->wall_hit;
	img = wall->frame;
	if (wall->type == nt_wall)
	{
		dir = get_wall_orientation(ray);
		if (!wall->overlay)
			wall->overlay_dir = dir;
		img = wall->frames[(int)dir];
		if (wall->overlay && wall->overlay_dir == dir)
			img = wall->overlay;
		wall->frame = img;
	}
	portal = 0;
	vertical_end = draw_strip(md, ray, screen_pos, txtr_coord, 1, &portal);
	if (portal)
		return (draw_strip(md, ray, screen_pos, txtr_coord, 0, &portal), 1);
	if (vertical_end < md->hud.new_floor_start)
		md->hud.new_floor_start = vertical_end;
	if (!wall->overlay)
		wall->overlay_dir = -1;
	return (1);
}

int	draw_wall_line(t_md *md, float dist, t_ent *wall, t_ray *ray)
{
	t_vec3f	txtr_cord;
	t_vec3	screen_pos;

	ray->wall_hit = wall;
	ray->distance = maxf(0.01, dist);
	txtr_cord.y = correct_fisheye(md, ray, wall, dist);
	if (ray->vertical_hit)
		txtr_cord.x = (int)fmod(ray->pos.y, wall->size.y);
	else
		txtr_cord.x = (int)fmod(ray->pos.x, wall->size.x);
	screen_pos.x = ray->index;
	screen_pos.y = compute_row_start(md, ray->distance);
	screen_pos.z = ray->hits_len > 0;
	draw_pxl(md, ray, txtr_cord, screen_pos);
	return (1);
}
