/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:11:19 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/03 11:15:12 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYS_H
# define RAYS_H

# include "../mlx_utils.h"
# include <pthread.h>

# define MAX_RAY_SPRITE		10
# define MAX_RAYS 			10000

typedef struct s_md		t_md;
typedef struct s_ray	t_ray;

typedef struct s_portal_wall
{
	t_ent		*e;
	t_wrd_dir	dir;
	t_vec2		out;
}	t_portal_wall;

typedef struct s_portal
{
	t_portal_wall	ends[2];
	int				last_shot_index;
}	t_portal;

typedef struct s_hit_data
{
	t_ent	*hit;
	t_vec3f	post_at_hit;
	float	dist_at_e;
	int		vertical_hit_at_e;
	int		row_index;
}	t_hit_data;

typedef struct s_ray
{
	t_hit_data	hit_data[MAX_RAY_SPRITE];
	int			hits_len;
	t_ent		*wall_hit;
	t_ent		*check_hit;
	t_vec3f		pos;
	t_vec3f		start;
	t_vec3f		dir;
	float		distance;
	float		sprite_distance;
	float		angle;
	int			vertical_hit;
	int			corner;
	int			index;
	int			color;
	int			had_door;
	int			is_floor_worker;
	int			steps;
	int			check_steps;
	int			flr_y;
	int			teleported_once;
}	t_ray;

typedef struct s_ray_draw_d
{
	t_ray	*ray;
	t_image	*img;
	t_vec3	win_start;
	t_vec3	wp;
	int		y_start;
	int		y_end;
	t_vec3f	txd_crd;
	int		pixel;
	int		pass;
	int		has_portal;
	int		y_max;
	int		pxl_clr;
	int		pxl_i;
	int		win_y;
}	t_ray_draw_d;

typedef struct s_floor_draw_d
{
	t_ray	*ray;
	t_vec2	win;
	t_vec3f	plr;
	t_vec2f	dirl;
	t_vec2f	dirr;
	float	rwd;
	t_vec2f	stp;
	t_vec2f	flr;
	t_vec2	txp;
	int		clr;
	float	p;
	float	fogalpha;
}	t_floor_draw_d;

typedef struct s_thread_worker
{
	t_md			*md;
	int				index;
	pthread_t		thread;
}	t_thread_worker;

typedef struct s_ray_manager
{
	int				threads_amount;
	t_dblst			*ents_to_draw;
	t_dblst			*e_distances;
	t_vec2			ray_visu_offset;
	t_thread_worker	threads[MAX_RAYS];
	t_vec3f			dir_vals[MAX_RAYS];
}	t_ray_manager;

#endif