/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_threads.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:31:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/03 16:09:18 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

t_vec2f	get_flr_pln(t_md *md, float fov_factor)
{
	float	fov_rad;
	float	base_len;
	float	final_len;
	float	dyn_factor;

	fov_rad = (float)md->prm.fov * (M_PI / 180.0f);
	base_len = tanf(fov_rad / 2.0f);
	dyn_factor = 1.0f - (fabsf(md->plr_rot.y) / 90.0f) * 0.2f;
	final_len = base_len * minmaxf(0.0f, 1.0f, fov_factor * dyn_factor);
	return ((t_vec2f){
		-md->plr.dir.y * final_len,
		 md->plr.dir.x * final_len
	});
}
void	update_ray_data(t_md *md, t_ray *ray, t_vec3f dir_val)
{
	ray->hit_data[0].hit = NULL;
	ray->hits_len = 0;
	ray->vertical_hit = 0;
	init_base_ray(ray, ray->index, md->cam_pos, 0);
	ray->dir = get_v3f(dir_val.x, dir_val.y, 0);
	ray->angle = dir_val.z;
}

static int	cast_thread_ray(t_md *md, t_ray *ray)
{
	t_hit_data		*hit_data;

	update_ray_data(md, ray, md->ray_manager.dir_vals[ray->index]);
	ray_move(md, ray, md->ray_manager.ray_visu_offset);
	if (!md->prm.ray_mode)
		return (1);
	if (!ray->check_hit && ray->wall_hit)
		draw_wall_line(md, ray->distance, ray->wall_hit, ray);
	if (!ray->wall_hit || ray->distance >= md->prm.ray_depth)
		ray->flr_y = md->win_sz.y * .5 - md->plr_rot.y * 8 - md->cam_pos.z;
	if (ray->flr_y < md->win_sz.y && md->hud.active_bgr)
		draw_floor(md, ray, ray->flr_y, get_flr_pln(md, md->prm.floor_fov));
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
		if (!cast_thread_ray(md, ray))
			break ;
	}
	return (NULL);
}

void	init_ray_threads(t_md *md)
{
	int				i;
	int				threads_amount;
	t_ray_manager	*mon;

	mon = &md->ray_manager;
	mon->ents_to_draw = NULL;
	mon->e_distances = NULL;
	i = -1;
	threads_amount = (md->win_sz.x + THREADS_BATCH - 1) / THREADS_BATCH;
	while (++i < md->win_sz.x)
	{
		md->rays[i].index = i;
		if (i < threads_amount)
		{
			mon->threads[i].md = md;
			mon->threads[i].index = i * THREADS_BATCH;
		}
	}
	mon->threads_amount = threads_amount;
	printf("%d threads will each handle %d rays for \
		%d width total handled %d\n", threads_amount, \
		THREADS_BATCH, md->win_sz.x, threads_amount * THREADS_BATCH);
}

void	cast_ray_threads(t_md *md)
{
	t_ray_manager	*mon;
	int				i;

	md->hud.new_floor_start = md->win_sz.y;
	mon = &md->ray_manager;
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
