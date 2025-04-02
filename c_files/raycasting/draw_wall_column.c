/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall_column.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:01:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/01 22:01:26 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	skip_pxl(t_md *md, t_ray_draw_d *d, int pxl_color)
{
	t_ray			*ray;
	int				rgb;
	int				is_transparent;
	int				is_portal_color;
	int				portal_index;

	is_transparent = ((pxl_color >> 24) != 0x00);
	if (is_transparent)
		return (1);
	rgb = pxl_color & 0x00FFFFFF;
	is_portal_color = (rgb == 0x0000FF || rgb == 0xFF0000);
	ray = d->ray;
	portal_index = get_portal_index(md, ray, ray->wall_hit);
	if (is_portal_color && portal_index != -1)
	{
		if (d->pass == 0)
			d->has_portal++;
		else
		{
			translate_ray(md, ray, ray->wall_hit, portal_index);
			return (0);
		}
		return (1);
	}
	return (0);
}

static int	pxl_draw(t_md *md, t_ray_draw_d *d, t_vec2 win_sz)
{
	t_vec2	win_p;

	d->pxl_i = ((int)d->win_y * (d->img->size_line / 4)) + (int)d->txd_crd.x;
	d->pxl_clr = *(d->img->src + d->pxl_i);
	win_p.x = d->win_start.x;
	win_p.y = d->y_start + d->win_start.y - md->cam_pos.z;
	if (win_p.y > win_sz.y)
		return (0);
	if (win_p.y > win_sz.y || win_p.y < 0 || win_p.x < 0 || win_p.x > win_sz.x)
		return (1);
	if (skip_pxl(md, d, d->pxl_clr))
		return (1);
	if (d->ray->wall_hit->angle)
		draw_pixel(md->screen, win_p, d->pxl_clr, d->ray->wall_hit->angle);
	else
		draw_pixel(md->screen, win_p, d->pxl_clr, -1);
	if (md->fx.fog)
		draw_pixel(md->screen, win_p, md->rgb[RGB_BLACK], \
	minmaxf(0, .95, ((d->ray->distance / md->t_len) / 10) * md->fx.fog));
	if (md->plr.shot && !d->ray->had_door && \
		d->win_start.x == win_sz.x / 2 && \
		d->y_start + d->win_start.y - md->plr.pos.z == win_sz.y / 2)
		draw_portal(md, d->ray->wall_hit, get_v2((int)d->txd_crd.x, d->win_y));
	return (1);
}

static void	draw_strip(t_md *md, t_ray_draw_d *d, t_vec2 win_sz, int pass)
{
	const float		step = d->img->size.y / d->txd_crd.y;

	d->pass = pass;
	d->y_start = (win_sz.y / 2 - d->txd_crd.y / 2) - 1;
	d->y_end = (win_sz.y / 2 + d->txd_crd.y / 2);
	while (++d->y_start < d->y_end)
	{
		d->win_y = (d->y_start - (win_sz.y / 2 - d->txd_crd.y / 2)) * step;
		if (d->win_y < 0 || d->win_y >= d->img->size.y)
			continue ;
		if (!pxl_draw(md, d, win_sz))
			return ;
		if (d->has_portal && pass == 0)
			return ;
	}
	d->y_max = d->y_start + d->win_start.y - md->cam_pos.z;
}

static int	draw_pxl(t_md *md, t_ray *ray, t_vec3f txtr_crd, t_vec3 screen_p)
{
	t_image			*img;
	t_wrd_dir		dir;
	t_ray_draw_d	draw_d;

	img = ray->wall_hit->frame;
	if (ray->wall_hit->type == nt_wall)
	{
		dir = get_wall_orientation(ray);
		if (!ray->wall_hit->overlay)
			ray->wall_hit->overlay_dir = dir;
		img = ray->wall_hit->frames[(int)dir];
		if (ray->wall_hit->overlay && ray->wall_hit->overlay_dir == dir)
			img = ray->wall_hit->overlay;
		ray->wall_hit->frame = img;
	}
	draw_d = (t_ray_draw_d){ray, ray->wall_hit->frame, \
		screen_p, screen_p, 0, 0, txtr_crd, 0, 0, 0, 9999, 0, 0, 0};
	draw_strip(md, &draw_d, md->win_sz, 0);
	if (draw_d.has_portal)
		draw_strip(md, &draw_d, md->win_sz, 1);
	md->hud.new_floor_start = minf(md->hud.new_floor_start, draw_d.y_max);
	if (ray->floor_y_start > draw_d.y_max)
		ray->floor_y_start = draw_d.y_max;
	return (1);
}

int	draw_wall_line(t_md *md, float dist, t_ent *wall, t_ray *ray)
{
	t_vec3f	txtr_cord;
	t_vec3	screen_pos;

	ray->wall_hit = wall;
	ray->distance = maxf(0.01, dist);
	wall->hp = ray->distance > md->t_len * 2;
	txtr_cord.y = correct_fisheye(md, ray, wall, dist);
	if (ray->vertical_hit)
		txtr_cord.x = (int)fmod(ray->pos.y, wall->size.y);
	else
		txtr_cord.x = (int)fmod(ray->pos.x, wall->size.x);
	screen_pos.x = ray->index;
	screen_pos.y = compute_row_start(md, wall, ray->distance);
	screen_pos.z = ray->hits_len > 0;
	draw_pxl(md, ray, txtr_cord, screen_pos);
	return (1);
}
