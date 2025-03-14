/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_ray.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:06:31 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/13 23:04:36 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	dda_draw_txt_line(t_md *md, t_ray *ray, t_ent *col)
{
	t_vec2	txt_p;
	float	height;
	float	dist;

	dist = ray->distance;
	if (dist < 0.01)
		dist = 0.01;
	height = (md->win_size.y * md->t_len) / \
	(dist * fabs(cos(ray->angle - md->plr.angle)));
	if (height > md->win_size.y * 1.5)
		height = md->win_size.y * 1.5;
	if (ray->hit_vrt)
		txt_p.x = (int)(fmod(ray->pos.y, md->t_len) * col->size.x / md->t_len);
	else
		txt_p.x = (int)(fmod(ray->pos.x, md->t_len) * col->size.x / md->t_len);
	txt_p.y = ray->distance;
	draw_pixel(md->screen.buffer, txt_p.x, txt_p.y, ray->color);
}

static void	show_ray(t_md *md, t_vec3f ray_pos, t_ent *col, int color)
{
	t_vec3	scrn_pos;
	t_vec2	draw_pos;
	t_vec2	draw_siz;
	int		clr;

	if (md->ray_mode || !md->show_rays)
		return ;
	scrn_pos = get_v3(ray_pos.x - md->cam_ofst.x, \
		ray_pos.y - md->cam_ofst.y, 0);
	if (!is_in_screen(md, scrn_pos, get_v2(1, 1)))
		return ;
	clr = color;
	if (col)
		clr = str_to_color("255,0,0");
	draw_siz = get_v2(md->t_len / 10, md->t_len / 10);
	draw_pos = get_v2(scrn_pos.x - draw_siz.x / 2, scrn_pos.y - draw_siz.y / 2);
	while (++draw_pos.y < scrn_pos.y + draw_siz.y / 2)
	{
		draw_pos.x = scrn_pos.x - draw_siz.x / 2;
		while (draw_pos.x < scrn_pos.x + draw_siz.x / 2)
			draw_pixel(md->screen.buffer, draw_pos.x++, draw_pos.y, clr);
	}
}

static void	move_ray(t_md *md, t_ray *ray, t_vec2 grid, int ray_color)
{
	t_ent	*col;
	int		depth;

	depth = 0;
	while (depth++ < RAY_DEPTH)
	{
		if (ray->side_dst.x < ray->side_dst.y)
		{
			ray->side_dst.x += ray->delta_dst.x;
			grid.x += ray->step_p.x;
			ray->hit_vrt = 1;
		}
		else
		{
			ray->side_dst.y += ray->delta_dst.y;
			grid.y += ray->step_p.y;
			ray->hit_vrt = 0;
		}
		col = check_in_map(md, ray);
		show_ray(md, ray->pos, col, ray_color);
		if (col)
			break ;
	}
	if (col && md->ray_mode)
		dda_draw_txt_line(md, ray, col);
}

void	dda_ray(t_md *md, t_ray *ray, int ray_color)
{
	int		tln;
	t_vec2	grid;

	tln = md->t_len;
	grid = get_v2((int)ray->pos.x / tln, (int)ray->pos.y / tln);
	ray->step_p = get_v2(1, 1);
	if (ray->dir.x < 0)
		ray->step_p.x = -1;
	if (ray->dir.y < 0)
		ray->step_p.y = -1;
	if (ray->dir.x < 0)
		ray->side_dst.x = (ray->pos.x - grid.x * tln) / fabs(ray->dir.x);
	else
		ray->side_dst.x = ((grid.x + 1) * tln - ray->pos.x) / fabs(ray->dir.x);
	if (ray->dir.y < 0)
		ray->side_dst.y = (ray->pos.y - grid.y * tln) / fabs(ray->dir.y);
	else
		ray->side_dst.y = ((grid.y + 1) * tln - ray->pos.y) / fabs(ray->dir.y);
	ray->delta_dst = get_v3f(tln / fabs(ray->dir.x), tln / fabs(ray->dir.y), 0);
	move_ray(md, ray, grid, ray_color);
}
