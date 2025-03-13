/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:31:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/13 02:47:18 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	show_ray(t_md *md, t_ray *ray, t_ent *col)
{
	t_vec3	ray_2dp;
	t_vec2	centr;
	t_vec2	draw_end;

	if (md->mmap.active && ray->index == md->win_size.x / 2 && \
			md->mmap.mray_len < MAX_RAYS - 1)
		md->mmap.ray_pos[md->mmap.mray_len++] = ray->pos;
	if (md->ray_mode || !md->show_rays)
		return (0);
	centr = get_v2((md->win_size.x / 4 + md->size_2d / 2 - (md->cam_ofst.x / md->t_len * md->size_2d)), md->win_size.y / 4 + md->size_2d / 2 - ((md->cam_ofst.y / md->t_len) * md->size_2d));
	ray_2dp.x = centr.x + (ray->pos.x / md->t_len) * md->size_2d;
	ray_2dp.y = centr.y + (ray->pos.y / md->t_len) * md->size_2d;
	ray_2dp.z = 0;
	if (!is_in_screen(md, ray_2dp, get_v2(1, 1)))
		return (0);
	if (!col)
		return (mlx_pixel_put(md->mlx, md->win, ray_2dp.x, ray_2dp.y, ray->color));
	draw_end = get_v2(ray_2dp.x + 3, ray_2dp.y + 3);
	ray_2dp = get_v3(ray_2dp.x - 3, ray_2dp.y - 3, 0);
	while (++ray_2dp.y < draw_end.y)
	{
		ray_2dp.x = draw_end.x - 6;
		while (++ray_2dp.x < draw_end.x)
			mlx_pixel_put(md->mlx, md->win, ray_2dp.x, ray_2dp.y, ray->color);
	}
	return (1);
}

int	is_ray_collision(t_ray *ray, t_ent *a)
{
	return (ray->pos.x >= a->pos.x + a->mov.x && \
		ray->pos.x <= a->pos.x + a->mov.x + a->size.x && \
		ray->pos.y >= a->pos.y + a->mov.y && \
		ray->pos.y <= a->pos.y + a->mov.y + a->size.y);
}

void	check_in_ents(t_md *md, t_ray *ray)
{
	t_dblst	*node;
	t_ent	*e;

	node = md->entities;
	while (node)
	{
		e = (t_ent *)node->content;
		if (!e || !e->frame || !e->is_active || \
				e->type == nt_wall || e->type == nt_empty || e->type == nt_plr)
		{
			node = node->next;
			continue ;
		}
		if (is_ray_collision(ray, e))
		{
			ray->pos_at_e = ray->pos;
			ray->found_e = e;
			ray->hit_vrt_at_e = ray->hit_vrt;
			break ;
		}
		node = node->next;
	}
}

t_ent	*check_in_map(t_md *md, t_ray *ray)
{
	int		index;
	t_ent	*e;

	index = (int)(ray->pos.x / md->t_len) + ((md->map.size.x + 1) * (int)(ray->pos.y / md->t_len));
	if (index >= 0 && index < md->map.len && md->map.buffer[index] == '1')
	{
		e = (t_ent *)md->entities[0].content;
		return (e);
	}
	check_in_ents(md, ray);
	return (NULL);
}

void	render_ray(t_md *md, t_ray *ray)
{
	t_ent	*col;
	int		i;
	float	hordist;
	float	verdist;

	i = -1;
	while (++i < RAY_DEPTH)
	{
		ray->pos = get_v3f(ray->pos.x + ray->dir.x, \
			ray->pos.y + ray->dir.y, \
			ray->pos.z + ray->dir.z);
		hordist = fabs(fmod(ray->pos.x, md->t_len));
		verdist = fabs(fmod(ray->pos.y, md->t_len));
		if (hordist <= 1.3 || verdist <= 1.3)
			ray->hit_vrt = verdist > hordist;
		ray->color = md->rgb[RGB_GREEN + ray->hit_vrt] + \
			(1000 * (ray->found_e != NULL));
		col = check_in_map(md, ray);
		show_ray(md, ray, col);
		if (col && col->type == nt_wall)
			break ;
	}
	if (!md->ray_mode)
		return ;
	if (col)
		draw_txt_line(md, i, col, ray);
	if (!ray->found_e)
		return ;
	ray->hit_vrt = ray->hit_vrt_at_e;
	ray->pos = ray->pos_at_e;
	draw_txt_line(md, i, ray->found_e, ray);
}

void	precompute_rays(t_md *md, float *cos_vals, float *sin_vals)
{
	float	fov;
	float	angle_step;
	float	yaw;
	int		i;
	float	ray_yaw;

	yaw = md->plr.rot.x * (M_PI / 180.0f);
	if (yaw < -M_PI)
		yaw += 2 * M_PI;
	else if (yaw >= M_PI)
		yaw -= 2 * M_PI;
	fov = FOV * (M_PI / 180.0f);
	angle_step = fov / (float)(md->win_size.x - 1);
	i = -1;
	while (++i < md->win_size.x)
	{
		ray_yaw = yaw - (fov / 2.0f) + (angle_step * i);
		cos_vals[i] = cosf(ray_yaw);
		sin_vals[i] = sinf(ray_yaw);
	}
}

void	render_rays(t_md *md, t_vec3f start_pos)
{
	float	cos_vals[MAX_RAYS];
	float	sin_vals[MAX_RAYS];
	int		i;

	precompute_rays(md, cos_vals, sin_vals);
	i = -1;
	while (++i < md->win_size.x)
	{
		md->rays[i].index = i;
		md->rays[i].hit_vrt = 0;
		md->rays[i].hit_vrt_at_e = 0;
		md->rays[i].found_e = NULL;
		md->rays[i].hit = get_v3f(0, 0, 0);// who am i if i am useles??
		md->rays[i].start = start_pos;
		md->rays[i].pos = start_pos;
		md->rays[i].side_dst = get_v3f(0, 0, 0);
		md->rays[i].angle = atan2f(sin_vals[i], cos_vals[i]);
		md->rays[i].dir = get_v3f(cos_vals[i], sin_vals[i], 0);
		md->rays[i].distance = 0;
		md->rays[i].median = 0;
		render_ray(md, &md->rays[i]);
	}
}
