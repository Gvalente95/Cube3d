/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:29:41 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 03:31:17 by giuliovalen      ###   ########.fr       */
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

typedef enum e_weapon_types
{
	Knife,
	Pistol,
	Shotgun,
	Machine_Gun,
	Flame_Thrower,
	Rocket,
	WEAPON_TYPE_LEN
}	t_weapon_types;

typedef enum e_mob_types
{
	Rat,
	Guard,
	Elite_Guard,
	Officer,
	Death_Knight,
	Mecha_Meister,
	MOB_TYPE_LEN
}	t_mob_types;

typedef enum e_pickup_types
{
	Ammo,
	Health,
	Keys,
	Score,
	Weapon,
	PICKUP_TYPE_LEN
}	t_pickup_types;

typedef enum e_entity_types
{
	nt_wall,
	nt_empty,
	nt_plr,
	nt_mob,
	nt_door,
	nt_pickup,
	ENT_TYPE_LEN
}	t_ent_type;

typedef enum e_player_action
{
	plr_idle,
	plr_walk,
	plr_hurt,
	plr_attack,
	plr_death,
	PLR_ACTION_LEN
}	t_player_action;

typedef enum e_entity_action
{
	m_idle,
	m_walk,
	m_atk,
	m_death,
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
	t_ent_type		type;
	t_mob_types		mob_type;
	t_ent_action	action;
	t_vec3f			dir;
	t_vec3f			mov;
	t_vec3f			pos;
	t_vec3f			start_pos;
	t_vec3			rot;
	t_vec3			coord_pos;
	t_vec3f			target_pos;
	t_vec2			size;
	t_image			*frame;
	t_image			**frames;
	t_image			***anim;
	double			shot_timer;
	float			angle;
	char			character;
	int				was_hit;
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