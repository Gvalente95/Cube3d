/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_threads.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:31:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/05 18:24:13 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	update_ray_data(t_md *md, t_ray *ray, t_vec3f dir_val)
{
	ray->hit_data[0].hit = NULL;
	ray->hits_len = 0;
	ray->vertical_hit = 0;
	init_base_ray(ray, ray->index, md->cam.pos, 0);
	ray->dir = get_v3f(dir_val.x, dir_val.y, 0);
	ray->angle = dir_val.z;
}

int	draw_stored_sprite_hits(t_md *md, t_ray *ray)
{
	t_hit_data		*hit_data;

	while (ray->hits_len--)
	{
		hit_data = &ray->hit_data[ray->hits_len];
		if (!hit_data->hit)
			return (0);
		if (ray->check_hit && ray->check_hit->type == nt_door)
			return (1);
		ray->vertical_hit = hit_data->vertical_hit_at_e;
		ray->pos = hit_data->post_at_hit;
		draw_sprite(md, ray, *hit_data);
	}
	return (1);
}

int	cast_thread_ray(t_md *md, int index)
{
	t_ray			*ray;

	ray = &md->rays[index];
	ray->index = index;
	update_ray_data(md, ray, md->thrd_manager.dir_vals[ray->index]);
	draw_raycast_background(md, ray);
	ray_move(md, ray, md->thrd_manager.ray_visu_offset);
	if (!ray->check_hit && ray->wall_hit)
		draw_wall_line(md, ray->distance, ray->wall_hit, ray);
	if (!ray->wall_hit || ray->distance >= md->prm.ray_depth)
		ray->flr_y = md->win_sz.y * .5 - md->cam.rot.y * 8 - md->cam.pos.z;
	if (ray->hits_len > 0)
		return (draw_stored_sprite_hits(md, ray));
	return (1);
}

void	*cast_thread_batch(void *content)
{
	t_thread_worker	*thread;
	t_md			*md;
	t_ray			*ray;
	int				i;
	int				ray_index;

	thread = (t_thread_worker *)content;
	md = thread->md;
	i = -1;
	while (++i < THREADS_BATCH)
	{
		ray_index = thread->index + i;
		if (ray_index >= md->win_sz.x)
			break ;
		ray = &md->rays[thread->index + i];
		ray->is_floor_worker = ((i + 1) % FLOOR_WORKERS == 0);
		if (!cast_thread_ray(md, ray_index))
			break ;
	}
	return (NULL);
}

void	cast_ray_threads(t_md *md)
{
	t_thrd_manager	*mon;
	int				i;

	md->hud.new_floor_start = md->win_sz.y;
	mon = &md->thrd_manager;
	mon->ray_visu_offset = get_2d_ray_pos(md);
	compute_ray_directions(md, mon->dir_vals, md->win_sz.x);
	i = -1;
	while (++i < mon->threads_amount)
		pthread_create(&mon->threads[i].thread, NULL, \
			cast_thread_batch, &mon->threads[i]);
	i = -1;
	while (++i < mon->threads_amount)
		pthread_join(mon->threads[i].thread, NULL);
	if (mon->ents_to_draw)
		draw_found_ents(md, mon);
	md->hud.floor_start = minmax(0, md->win_sz.x, md->hud.new_floor_start);
}
