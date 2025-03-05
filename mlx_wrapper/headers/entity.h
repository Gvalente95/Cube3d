/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:29:41 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 11:40:46 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITY_H
# define ENTITY_H

# include "../mlx_utils.h"

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
	player,
	mob,
	coin,
	key,
	door,
	portal,
	wall,
	axe,
	ground,
	tile,
	mover,
	laser,
	spike,
	thrower,
	squasher,
	star,
	bgrnd,
	env,
	particle,
	ENT_TYPE_LEN
}	t_ent_type;

typedef enum e_entity_action
{
	IDLE,
	WALK,
	RUN,
	FALL,
	JUMP,
	DEATH,
	HURT,
	THROW,
	PUSH,
	SLIDE,
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

typedef struct s_particle
{
	t_prt_type	type;
	t_vec2		foll_ofs;
	t_vec2		pos;
	t_vec2		size;
	t_vec2		movement;
	t_vec3		*dest;
	t_vec3		target;
	t_vec2		speed;
	t_dir		dir;
	void		*frame;
	int			is_grounded;
	int			lifetime;
	int			is_active;
}	t_prt;

typedef struct s_entity
{
	t_ent_type		type;
	t_ent_action	action;
	t_dir			dir;
	t_vec3f			rot;
	t_vec3f			start_pos;
	t_vec3f			prv_pos;
	t_vec3f			base_pos;
	t_vec3f			pos;
	t_vec2			size;
	t_vec2			foll_ofs;
	t_vec3f			mov;
	t_vec3f			col_dspl;
	t_prt_type		prt_type;
	void			*frame;
	void			*frame_x;
	void			**anim_frames;
	void			***anim;
	void			***anim_x;
	int				audio;
	int				level;
	int				is_throwing;
	int				hurt_timer;
	int				is_active;
	int				foll_spd;
	int				frames_amount;
	int				hp;
	int				is_grounded;
	int				jumps;
	int				jump_timer;
	int				flip_x;
	int				jet_sky_timer;
	int				frame_index;
	int				in_screen;
}	t_ent;

#endif