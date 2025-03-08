/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:31:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/07 16:21:46 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	draw_txt_line(t_md *md, float distance, t_vec3 pos, t_ent *col, t_ray *ray)
{
	int		screen_x;
	int		draw_start;
	int		draw_end;
	float	wall_height;
	int		texture_x;
	int		color;

	screen_x = ray->index;
	draw_start = md->win_size.y / 2 - col->size.y / 2;
	draw_end = md->win_size.y / 2 + col->size.y / 2;
	wall_height = (md->win_size.y * md->t_len) / (distance * cos(ray->angle));
	texture_x = (int)(fmod(pos.x, md->t_len) / md->t_len * col->size.x);
	for (int y = draw_start; y < draw_end; y++) {
		int texture_y = (y - (md->win_size.y / 2) + (col->size.y / 2)) * (col->size.y / wall_height);
		color = mlx_get_color_value(md->mlx, *(int *)(col->frame + (texture_y * col->size.x + texture_x) * sizeof(int)));
		mlx_pixel_put(md->mlx, md->win, screen_x, y, color);
	}
}

t_ent	*check_in_entities(t_md *md, t_vec3 grid_pos)
{
	t_dblist	*node;
	t_ent		*e;

	node = md->entities;
	while (node)
	{
		e = (t_ent *)node->content;
		if (e && e->is_active)
		{
			if (cmp_vec3(grid_pos, e->coord_pos))
				return (e);
		}
		node = node->next;
	}
	return (NULL);
}

void	render_ray(t_md *md, t_ray *ray, int color)
{
	t_vec3	grid_pos;
	t_vec3f	pos;
	t_ent	*col;
	int		i;

	pos = ray->start;
	i = -1;
	while (++i < RAY_DEPTH)
	{
		pos.x += ray->direction.x;
		pos.y += ray->direction.y;
		pos.z += ray->direction.z;
		grid_pos = get_v3(pos.x / md->t_len, pos.y / md->t_len, 0);
		col = check_in_entities(md, grid_pos);
		if (col)
			break ;
		if (!md->debug_mode || md->ray_mode)
			continue ;
		if (is_in_screen(md, get_v3(pos.x - md->cam_ofst.x, pos.y - md->cam_ofst.y, 0), get_v2(1, 1)))
			mlx_pixel_put(md->mlx, md->win, pos.x - md->cam_ofst.x, pos.y - md->cam_ofst.y, color);
	}
	if (col)
	{
		if (md->ray_mode)
			draw_txt_line(md, i, get_v3(pos.x, pos.y, 0), col, ray);
		else
		{
			for (int y = -5; y < 5; y ++)
				for (int x = -5; x < 5; x ++)
					mlx_pixel_put(md->mlx, md->win, x + pos.x - md->cam_ofst.x, y + pos.y - md->cam_ofst.y, str_to_color("255,0,0"));
		}
	}
}

void	render_rays(t_md *md, t_vec3f start_pos)
{
	int		i;
	float	fov;
	float	angle_step;
	float	ray_yaw;
	float	yaw;
	float	pitch;
	int		color;

	color = str_to_color("0,255,0");
	yaw = md->plr.rot.x * 0.01f;
	if (yaw < 0)
    	yaw += 2 * M_PI;
	else if (yaw >= 2 * M_PI)
    	yaw -= 2 * M_PI;
	pitch = md->plr.rot.y * 0.01f;
	fov = FOV * (M_PI / 180.0f);
	angle_step = fov / (float)(RAYS_AMOUNT - 1);
	i = -1;
	while (++i < RAYS_AMOUNT)
	{
		ray_yaw = yaw - (fov / 2.0f) + (angle_step * i);
		md->rays[i].index = i;
		md->rays[i].start = start_pos;
		md->rays[i].angle = ray_yaw;
		md->rays[i].direction.x = cosf(pitch) * cosf(ray_yaw);
		md->rays[i].direction.y = cosf(pitch) * sinf(ray_yaw);
		md->rays[i].direction.z = sinf(pitch);
		md->rays[i].distance = 0;
		md->rays[i].median = 0;
		render_ray(md, &md->rays[i], color);
	}
}
