/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:31:39 by gvalente          #+#    #+#             */
/*   Updated: 2025/04/03 22:26:15 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../mlx_utils.h"

typedef struct s_thread_data
{
	t_md			*md;
	int				thread_id;
	int				start_x;
	int				end_x;
	pthread_t		thread;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				should_work;
	int				should_exit;
}	t_thread_data;

typedef struct s_soft_barrier
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				count;
	int				total;
}	t_soft_barrier;

typedef struct s_thread_worker
{
	t_md			*md;
	int				index;
	pthread_t		thread;
}	t_thread_worker;

typedef struct s_threads_manager
{
	int				threads_amount;
	t_dblst			*ents_to_draw;
	t_dblst			*e_distances;
	t_vec2			ray_visu_offset;
	t_thread_worker	threads[MAX_RAYS];
	t_thread_data	thrdlp[MAX_RAYS];
	t_vec3f			dir_vals[MAX_RAYS];
	t_soft_barrier	barrier;
}	t_threads_manager;

void	soft_barrier_init(t_soft_barrier *b, int total);
void	soft_barrier_wait(t_soft_barrier *b);
void	soft_barrier_destroy(t_soft_barrier *b);
void	*cast_thread_loop(void *arg);
void	init_thread_pool(t_md *md, int thread_count);
void	trigger_threads(t_md *md);
void	cleanup_thread_pool(t_md *md);

int		cast_thread_ray(t_md *md, t_ray *ray);
void	cast_ray_threads_lp(t_md *md);