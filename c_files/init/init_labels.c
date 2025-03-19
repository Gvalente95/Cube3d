/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_labels.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:41:10 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 03:29:15 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	init_action_labels(t_md *md)
{
	md->ents_act_names[m_idle] = "IDLE";
	md->ents_act_names[m_walk] = "WALK";
	md->ents_act_names[m_atk] = "ATTACK";
	md->ents_act_names[m_death] = "DEATH";
	md->plr_act_names[plr_walk] = "WALK";
	md->plr_act_names[plr_attack] = "ATTACK";
	md->plr_act_names[plr_hurt] = "HURT";
	md->plr_act_names[plr_death] = "DEATH";
	md->plr_act_names[plr_idle] = "idle";
}

void	init_weapon_labels(t_md *md)
{
	md->weapons_names[Knife] = "Knife";
	md->weapons_names[Pistol] = "Pistol";
	md->weapons_names[Shotgun] = "Shotgun";
	md->weapons_names[Machine_Gun] = "Machine gun";
	md->weapons_names[Flame_Thrower] = "Flame Thrower";
	md->weapons_names[Rocket] = "Rocket";
}

void	init_ents_labels(t_md *md)
{
	md->ents_tp_map[0] = "10*MDP";
	md->ents_types_names[nt_wall] = "Wall";
	md->ents_types_names[nt_empty] = "Empty";
	md->ents_types_names[nt_plr] = "Player";
	md->ents_types_names[nt_mob] = "Mob";
	md->ents_types_names[nt_door] = "Door";
	md->ents_types_names[nt_pickup] = "Pickup";
	md->pickup_names[Keys] = "Keys";
	md->pickup_names[Score] = "Score";
	md->pickup_names[Health] = "Health";
	md->pickup_names[Weapon] = "Weapn";
	md->pickup_names[Ammo] = "Ammo";
	md->mob_names[Rat] = "Rat";
	md->mob_names[Guard] = "Guard";
	md->mob_names[Elite_Guard] = "Elite Guard";
	md->mob_names[Officer] = "Officer";
	md->mob_names[Death_Knight] = "Death Knight";
	md->mob_names[Mecha_Meister] = "Mecha Meister";
}

void	init_dir_labels(t_md *md)
{
	md->dir_labels[NORTH] = "north";
	md->dir_labels[SOUTH] = "south";
	md->dir_labels[WEST] = "west";
	md->dir_labels[EAST] = "east";
}

void	init_labels(t_md *md)
{
	init_weapon_labels(md);
	init_ents_labels(md);
	init_action_labels(md);
	init_dir_labels(md);
}
