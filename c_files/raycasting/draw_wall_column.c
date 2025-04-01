/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall_column.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:01:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/01 18:20:36 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	skip_pxl(t_md *md, t_ray_draw_d *d, int pxl_color, t_vec2 win_sz)
{
	t_ray	*ray;
	int		rgb;

	ray = d->ray;
	if ((pxl_color >> 24) != 0x00)
		return (1);
	rgb = pxl_color & 0x00FFFFFF;
	if ((rgb == 0x0000FF || rgb == 0xFF0000) && \
		get_portal_index(md, ray, ray->wall_hit) != -1)
	{
		if (d->pass == 0)
			d->has_portal = 1;
		else if (translate_ray(md, ray, ray->wall_hit, ray->distance))
			return (win_sz.y);
		return (1);
	}
	return (0);
}

static int	pxl_draw(t_md *md, t_ray_draw_d *d, t_vec2 win_sz)
{
	t_vec2	win_p;
	t_ray	*ray;
	int		skip_this;

	d->pxl_i = ((int)d->win_y * (d->img->size_line / 4)) + (int)d->txd_crd.x;
	d->pxl_clr = *(d->img->src + d->pxl_i);
	skip_this = skip_pxl(md, d, d->pxl_clr, win_sz);
	if (skip_this)
		return (skip_this);
	win_p.x = d->win_start.x;
	win_p.y = d->y_start + d->win_start.y - md->cam_pos.z;
	if (d->ray->wall_hit->angle)
		draw_pixel(md->screen, win_p, d->pxl_clr, d->ray->wall_hit->angle);
	else
		draw_pixel(md->screen, win_p, d->pxl_clr, -1);
	ray = d->ray;
	if (md->fx.fog)
		draw_pixel(md->screen, win_p, md->rgb[RGB_BLACK], \
	minmaxf(0, .95, ((ray->distance / md->t_len) / 10) * md->fx.fog));
	if (md->plr.shot && !ray->had_door && \
		d->win_start.x == win_sz.x / 2 && \
		d->y_start + d->win_start.y - md->plr.pos.z == win_sz.y / 2)
		draw_portal(md, ray->wall_hit, get_v2((int)d->txd_crd.x, d->win_y));
	return (1);
}

static int	draw_strip(t_md *md, t_ray_draw_d *d, t_vec2 win_sz, int pass)
{
	const float		step = d->img->size.y / d->txd_crd.y;
	int				ret;

	d->pass = pass;
	d->y_start = (win_sz.y / 2 - d->txd_crd.y / 2) - 1;
	d->y_end = (win_sz.y / 2 + d->txd_crd.y / 2);
	while (++d->y_start < d->y_end)
	{
		d->win_y = (d->y_start - (win_sz.y / 2 - d->txd_crd.y / 2)) * step;
		if (d->win_y < 0 || d->win_y >= d->img->size.y)
			continue ;
		ret = pxl_draw(md, d, win_sz);
		if (ret > 1)
			return (ret);
	}
	d->y_max = d->y_start + d->win_start.y - md->cam_pos.z;
	return (d->y_max);
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
		screen_p, screen_p, 0, 0, txtr_crd, 0, 0, 0, 0, 0, 0, 0};
	draw_strip(md, &draw_d, md->win_sz, 0);
	if (draw_d.has_portal)
		draw_strip(md, &draw_d, md->win_sz, 1);
	md->hud.new_floor_start = minf(md->hud.new_floor_start, draw_d.y_max);
	if (md->hud.active_bgr && ray->is_floor_worker)
		draw_floor(md, ray, draw_d.y_max, \
			get_v2f(-md->plr.dir.y * .66, md->plr.dir.x * .66));
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
