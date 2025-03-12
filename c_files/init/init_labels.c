/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_labels.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:41:10 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/12 11:47:28 by giuliovalen      ###   ########.fr       */
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
	md->ents_tp_names[nt_plr] = "player";
	md->ents_tp_names[nt_mob] = "mob";
	md->ents_tp_names[nt_coin] = "coin";
	md->ents_tp_names[nt_key] = "key";
	md->ents_tp_names[nt_door] = "door";
	md->ents_tp_names[nt_portal] = "portal";
	md->ents_tp_names[nt_wall] = "wall";
	md->ents_tp_names[nt_empty] = "empty";
	md->ents_tp_names[nt_axe] = "axe";
	md->ents_tp_names[nt_ground] = "ground";
	md->ents_tp_names[nt_tile] = "tile";
	md->ents_tp_names[nt_mover] = "mover";
	md->ents_tp_names[nt_bush] = "laser";
	md->ents_tp_names[nt_spike] = "spike";
	md->ents_tp_names[nt_thrower] = "thrower";
	md->ents_tp_names[nt_trees] = "squash";
	md->ents_tp_names[nt_star] = "star";
	md->ents_tp_names[nt_road] = "background";
	md->ents_tp_names[nt_env] = "env";
	md->ents_tp_names[nt_prt] = "particule";
	md->ents_tp_map[0] = "AMCKDP10AGTVSRQSBEP";
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
