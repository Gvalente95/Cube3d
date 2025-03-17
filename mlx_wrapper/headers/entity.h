/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:29:41 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 00:04:42 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITY_H
# define ENTITY_H

# include "../mlx_utils.h"
# include "../../lists/lists.h"

# define PRT_AMOUNT		500
# define PART_BASE_SIZE	35

typedef struct s_image_data t_image;

typedef enum e_dir
{
	up,
	left,
	down,
	right,
	none,
	all,
	DIR_LEN
}	t_dir;

typedef enum e_entity_types
{
	nt_plr,
	nt_mob,
	nt_coin,
	nt_key,
	nt_door,
	nt_portal,
	nt_wall,
	nt_empty,
	nt_axe,
	nt_ground,
	nt_tile,
	nt_mover,
	nt_bush,
	nt_spike,
	nt_thrower,
	nt_trees,
	nt_star,
	nt_road,
	nt_env,
	nt_prt,
	ENT_TYPE_LEN
}	t_ent_type;

typedef enum e_entity_action
{
	ac_idl,
	ac_wlk,
	ac_run,
	ac_fall,
	ac_jmp,
	ac_hurt,
	ac_atk,
	ac_throw,
	ac_push,
	ac_slide,
	ac_death,
	ENT_ACTION_LEN
}	t_ent_action;

typedef enum e_particle_tpes
{
	proj,
	splat,
	target,
	trail,
	spark,
	PRT_TYPE_LEN,
}	t_prt_type;

typedef struct s_entity t_ent;

typedef struct s_entity
{
	t_ent			*col_hit;
	t_ent			*emitter;
	t_ent_type		type;
	t_ent_action	action;
	t_vec3f			dir;
	t_vec3f			mov;
	t_vec3f			pos;
	t_vec3f			start_pos;
	t_vec3			rot;
	t_vec3			coord_pos;
	t_vec2			size;
	t_image			*frame;
	t_image			**frames;
	t_image			***anim;
	double			shot_timer;
	float			angle;
	char			character;
	int				shot;
	int				can_shoot;
	int				map_index;
	int				is_active;
	int				level;
	int				hp;
	int				hurt_timer;
	int				audio;
	int				is_grounded;
	int				jumps;
	int				frame_index;
	int				in_screen;
	int				row_draw_index;
}	t_ent;

typedef struct s_particle
{
	t_prt_type	type;
	t_vec2		foll_ofs;
	t_vec3f		pos;
	t_vec3f		dir;
	t_ent		*hit;
	int			speed;
	int			lifetime;
}	t_prt;

#endif