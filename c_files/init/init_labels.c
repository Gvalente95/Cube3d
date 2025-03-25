/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_labels.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:41:10 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/23 15:06:10 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	init_action_labels(t_texture_data *td)
{
	td->ents_act_names[m_idle] = "IDLE";
	td->ents_act_names[m_walk] = "WALK";
	td->ents_act_names[m_atk] = "ATTACK";
	td->ents_act_names[m_death] = "DEATH";
	td->plr_act_names[plr_walk] = "WALK";
	td->plr_act_names[plr_attack] = "ATTACK";
	td->plr_act_names[plr_hurt] = "HURT";
	td->plr_act_names[plr_death] = "DEATH";
	td->plr_act_names[plr_idle] = "idle";
}

void	init_weapon_labels(t_texture_data *td)
{
	td->weapons_names[Knife] = "Knife";
	td->weapons_names[Pistol] = "Pistol";
	td->weapons_names[Shotgun] = "Shotgun";
	td->weapons_names[Machine_Gun] = "Machine gun";
	td->weapons_names[Flame_Thrower] = "Flame Thrower";
	td->weapons_names[Rocket] = "Rocket";
}

void	init_ents_labels(t_texture_data *td)
{
	td->ents_tp_map[0] = "10*MDP";
	td->ents_types_names[nt_wall] = "Wall";
	td->ents_types_names[nt_empty] = "Empty";
	td->ents_types_names[nt_plr] = "Player";
	td->ents_types_names[nt_mob] = "Mob";
	td->ents_types_names[nt_door] = "Door";
	td->ents_types_names[nt_pickup] = "Pickup";
	td->pickup_names[Keys] = "Keys";
	td->pickup_names[Score] = "Score";
	td->pickup_names[Health] = "Health";
	td->pickup_names[Weapon] = "Weapn";
	td->pickup_names[Ammo] = "Ammo";
	td->mob_names[Rat] = "Rat";
	td->mob_names[Guard] = "Guard";
	td->mob_names[Elite_Guard] = "Elite Guard";
	td->mob_names[Officer] = "Officer";
	td->mob_names[Death_Knight] = "Death Knight";
	td->mob_names[Mecha_Meister] = "Mecha Meister";
}

void	init_dir_labels(t_md *md)
{
	md->dir_labels[NORTH] = "north";
	md->dir_labels[SOUTH] = "south";
	md->dir_labels[WEST] = "west";
	md->dir_labels[EAST] = "east";
}

void	init_labels(t_md *md, t_texture_data *txd)
{
	init_weapon_labels(txd);
	init_ents_labels(txd);
	init_action_labels(txd);
	init_dir_labels(md);
}
