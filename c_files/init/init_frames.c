/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_frames.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:57:39 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/18 21:38:21 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	init_weapon_frames(t_md *md)
{
	md->wpn_txtr = malloc(sizeof(t_image **) * (WEAPON_TYPE_LEN + 1));
	md->wpn_txtr_2d = malloc(sizeof(t_image **) * (WEAPON_TYPE_LEN + 1));
	if (!md->wpn_txtr || !md->wpn_txtr_2d)
		return ;
	md->wpn_txtr[Knife] = init_weapon(md, \
		&md->wpn_txtr_2d[Knife], "ent/Weapons/Knife/");
	md->wpn_txtr[Pistol] = init_weapon(md, \
		&md->wpn_txtr_2d[Pistol], "ent/Weapons/Pistol/");
	md->wpn_txtr[Shotgun] = init_weapon(md, \
		&md->wpn_txtr_2d[Shotgun], "ent/Weapons/Shotgun/");
	md->wpn_txtr[Machine_Gun] = init_weapon(md, \
		&md->wpn_txtr_2d[Machine_Gun], "ent/Weapons/Machine gun/");
	md->wpn_txtr[Flame_Thrower] = init_weapon(md, \
		&md->wpn_txtr_2d[Flame_Thrower], "ent/Weapons/Flame Thrower/");
	md->wpn_txtr[Rocket] = init_weapon(md, \
		&md->wpn_txtr_2d[Rocket], "ent/Weapons/Rocket/");
	md->wpn_txtr[WEAPON_TYPE_LEN] = NULL;
	md->wpn_txtr_2d[WEAPON_TYPE_LEN] = NULL;
}

void	init_pickup_frames(t_md *md)
{
	md->pickup_txtr = malloc(sizeof(t_image **) * (PICKUP_TYPE_LEN + 1));
	md->pickup_txtr_mini = malloc(sizeof(t_image **) * (PICKUP_TYPE_LEN + 1));
	if (!md->pickup_txtr || !md->pickup_txtr_mini)
		return ;
	md->pickup_txtr[Ammo] = init_mini(md, \
		&md->pickup_txtr_mini[Ammo], "ent/Pickup/Ammo/", (int)Ammo);
	md->pickup_txtr[Health] = init_mini(md, \
		&md->pickup_txtr_mini[Health], "ent/Pickup/Health/", (int)Health);
	md->pickup_txtr[Keys] = init_mini(md, \
		&md->pickup_txtr_mini[Keys], "ent/Pickup/Keys/", (int)Keys);
	md->pickup_txtr[Score] = init_mini(md, \
		&md->pickup_txtr_mini[Score], "ent/Pickup/Score/", (int)Score);
	md->pickup_txtr[Weapon] = init_mini(md, \
		&md->pickup_txtr_mini[Weapon], "ent/Pickup/Weapon/", (int)Weapon);
	md->pickup_txtr[PICKUP_TYPE_LEN] = NULL;
	md->pickup_txtr_mini[PICKUP_TYPE_LEN] = NULL;
}

void	store_entities_sizes(t_md *md, t_vec2 base)
{
	int		i;

	i = -1;
	while (++i < ENT_TYPE_LEN)
		md->e_sizes2d[i] = get_v2(md->size_2d, md->size_2d);
	md->e_sizes[nt_plr] = get_v2(base.x * .2, base.y * .2);
	md->e_sizes[nt_mob] = get_v2(base.x * .5, base.y * .5);
	md->e_sizes[nt_door] = base;
	md->e_sizes[nt_wall] = base;
	md->e_sizes[nt_pickup] = base;
}

void	init_ents_data(t_md *md)
{
	init_labels(md);
	store_entities_sizes(md, get_v2(md->t_len, md->t_len));
	init_weapon_frames(md);
	init_pickup_frames(md);
	init_mobs_frames(md);
}
