/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_threads_Loop.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:26:11 by gvalente          #+#    #+#             */
/*   Updated: 2025/04/03 22:36:10 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	*cast_thread_loop(void *arg)
{
	t_thread_data	*data;
	int				x;
	t_ray			ray;

	data = (t_thread_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->mutex);
		while (!data->should_work && !data->should_exit)
			pthread_cond_wait(&data->cond, &data->mutex);
		if (data->should_exit)
		{
			pthread_mutex_unlock(&data->mutex);
			break;
		}
		data->should_work = 0;
		pthread_mutex_unlock(&data->mutex);
		x = data->start_x - 1;
		while (++x < data->end_x)
		{
			ray.index = x;
			cast_thread_ray(data->md, &ray);
		}
		soft_barrier_wait(&data->md->threads_manager.barrier);
	}
	return (NULL);
}

void	trigger_threads(t_md *md)
{
	t_threads_manager	*rm = &md->threads_manager;
	int				i;

	i = -1;
	while (++i < rm->threads_amount)
	{
		pthread_mutex_lock(&rm->thrdlp[i].mutex);
		rm->thrdlp[i].should_work = 1;
		pthread_cond_signal(&rm->thrdlp[i].cond);
		pthread_mutex_unlock(&rm->thrdlp[i].mutex);
	}
	soft_barrier_wait(&rm->barrier);
}

void	cast_ray_threads_lp(t_md *md)
{
	t_threads_manager *rm = &md->threads_manager;

	md->hud.new_floor_start = md->win_sz.y;
	rm->ray_visu_offset = get_2d_ray_pos(md);
	compute_ray_directions(md, rm->dir_vals, md->win_sz.x);
	trigger_threads(md);
	md->hud.floor_start = minmax(0, md->win_sz.x, md->hud.new_floor_start);
	if (rm->ents_to_draw)
		draw_found_ents(md, rm);
}

void	init_thread_pool(t_md *md, int thread_count)
{
	t_threads_manager	*rm = &md->threads_manager;
	int					width = md->win_sz.x;
	int					i;

	soft_barrier_init(&rm->barrier, thread_count + 1);
	rm->threads_amount = thread_count;
	i = -1;
	while (++i < thread_count)
	{
		rm->thrdlp[i].md = md;
		rm->thrdlp[i].thread_id = i;
		rm->thrdlp[i].start_x = (width * i) / thread_count;
		rm->thrdlp[i].end_x = (width * (i + 1)) / thread_count;
		rm->thrdlp[i].should_work = 0;
		rm->thrdlp[i].should_exit = 0;
		pthread_mutex_init(&rm->thrdlp[i].mutex, NULL);
		pthread_cond_init(&rm->thrdlp[i].cond, NULL);
		pthread_create(&rm->thrdlp[i].thread, NULL, cast_thread_loop, &rm->thrdlp[i]);
	}
}

void	cleanup_thread_pool(t_md *md)
{
	t_threads_manager	*rm = &md->threads_manager;
	int				i;

	i = -1;
	while (++i < rm->threads_amount)
	{
		pthread_mutex_lock(&rm->thrdlp[i].mutex);
		rm->thrdlp[i].should_exit = 1;
		pthread_cond_signal(&rm->thrdlp[i].cond);
		pthread_mutex_unlock(&rm->thrdlp[i].mutex);
	}
	i = -1;
	while (++i < rm->threads_amount)
		pthread_join(rm->thrdlp[i].thread, NULL);
	soft_barrier_destroy(&rm->barrier);
}
