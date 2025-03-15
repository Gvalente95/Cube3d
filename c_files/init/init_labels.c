/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_labels.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:41:10 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/15 00:03:44 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	init_action_labels(t_md *md)
{
	md->ents_act_names[ac_idl] = "idle";
	md->ents_act_names[ac_wlk] = "walk";
	md->ents_act_names[ac_run] = "run";
	md->ents_act_names[ac_fall] = "fall";
	md->ents_act_names[ac_jmp] = "jump";
	md->ents_act_names[ac_hurt] = "hurt";
	md->ents_act_names[ac_atk] = "attack";
	md->ents_act_names[ac_throw] = "throw";
	md->ents_act_names[ac_push] = "push";
	md->ents_act_names[ac_slide] = "slide";
	md->ents_act_names[ac_death] = "death";
}

void	init_ents_labels(t_md *md)
{
	md->e_typ_names[nt_plr] = "player";
	md->e_typ_names[nt_mob] = "mob";
	md->e_typ_names[nt_coin] = "coin";
	md->e_typ_names[nt_key] = "key";
	md->e_typ_names[nt_door] = "door";
	md->e_typ_names[nt_portal] = "portal";
	md->e_typ_names[nt_wall] = "wall";
	md->e_typ_names[nt_empty] = "empty";
	md->e_typ_names[nt_axe] = "axe";
	md->e_typ_names[nt_ground] = "ground";
	md->e_typ_names[nt_tile] = "tile";
	md->e_typ_names[nt_mover] = "mover";
	md->e_typ_names[nt_bush] = "laser";
	md->e_typ_names[nt_spike] = "spike";
	md->e_typ_names[nt_thrower] = "thrower";
	md->e_typ_names[nt_trees] = "squash";
	md->e_typ_names[nt_star] = "star";
	md->e_typ_names[nt_road] = "background";
	md->e_typ_names[nt_env] = "env";
	md->e_typ_names[nt_prt] = "particule";
	md->ents_tp_map[0] = "*MCKDP10AGTVSRQSBEP";
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
	init_ents_labels(md);
	init_action_labels(md);
	init_dir_labels(md);
}
