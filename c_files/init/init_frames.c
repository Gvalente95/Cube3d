/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_frames.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:57:39 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 19:31:07 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	init_weapon_frames(t_md *md, t_texture_data *td)
{
	td->wpn_txtr = malloc(sizeof(t_image **) * (WEAPON_TYPE_LEN + 1));
	td->wpn_txtr_2d = malloc(sizeof(t_image **) * (WEAPON_TYPE_LEN + 1));
	if (!td->wpn_txtr || !td->wpn_txtr_2d)
		return ;
	td->wpn_txtr[Knife] = init_weapon(md, \
		&td->wpn_txtr_2d[Knife], "ent/Weapons/Knife/");
	td->wpn_txtr[Pistol] = init_weapon(md, \
		&td->wpn_txtr_2d[Pistol], "ent/Weapons/Pistol/");
	td->wpn_txtr[Shotgun] = init_weapon(md, \
		&td->wpn_txtr_2d[Shotgun], "ent/Weapons/Shotgun/");
	td->wpn_txtr[Machine_Gun] = init_weapon(md, \
		&td->wpn_txtr_2d[Machine_Gun], "ent/Weapons/Machine gun/");
	td->wpn_txtr[Flame_Thrower] = init_weapon(md, \
		&td->wpn_txtr_2d[Flame_Thrower], "ent/Weapons/Flame Thrower/");
	td->wpn_txtr[Rocket] = init_weapon(md, \
		&td->wpn_txtr_2d[Rocket], "ent/Weapons/Rocket/");
	td->wpn_txtr[WEAPON_TYPE_LEN] = NULL;
	td->wpn_txtr_2d[WEAPON_TYPE_LEN] = NULL;
}

void	init_pickup_frames(t_md *md, t_texture_data *td)
{
	td->pickup_txtr = malloc(sizeof(t_image **) * (PCKP_TYPE_LEN + 1));
	td->pickup_txtr_mini = malloc(sizeof(t_image **) * (PCKP_TYPE_LEN + 1));
	if (!td->pickup_txtr || !td->pickup_txtr_mini)
		return ;
	td->pickup_txtr[Ammo] = init_mini(md, \
		&td->pickup_txtr_mini[Ammo], "ent/Pickup/Ammo/");
	td->pickup_txtr[Health] = init_mini(md, \
		&td->pickup_txtr_mini[Health], "ent/Pickup/Health/");
	td->pickup_txtr[Keys] = init_mini(md, \
		&td->pickup_txtr_mini[Keys], "ent/Pickup/Keys/");
	td->pickup_txtr[Score] = init_mini(md, \
		&td->pickup_txtr_mini[Score], "ent/Pickup/Score/");
	td->pickup_txtr[Weapon] = init_mini(md, \
		&td->pickup_txtr_mini[Weapon], "ent/Pickup/Weapons/");
	td->pickup_txtr[PCKP_TYPE_LEN] = NULL;
	td->pickup_txtr_mini[PCKP_TYPE_LEN] = NULL;
}

void	store_entities_sizes(t_texture_data *td, t_vec2 base)
{
	int				i;

	i = -1;
	while (++i < ENT_TYPE_LEN)
		td->e_sizes2d[i] = v2(td->size_2d);
	td->e_sizes[nt_plr] = v2(base.x * .2);
	td->e_sizes[nt_mob] = v2(base.x * .5);
	td->e_sizes[nt_pickup] = v2(base.x * .5);
	td->e_sizes[nt_door] = base;
	td->e_sizes[nt_wall] = base;
}

void	init_ents_data(t_md *md, t_texture_data *txd)
{
	init_labels(md, txd);
	store_entities_sizes(txd, v2(md->t_len));
	init_weapon_frames(md, txd);
	init_pickup_frames(md, txd);
	init_mobs_frames(md);
	txd->door_txtr = init_img(md, txd->e_sizes[nt_door], "ent/door/0.xpm", -1);
	txd->door_txtr_mini = \
		init_img(md, get_v2(txd->size_2d, txd->size_2d), "ent/door/0.xpm", -1);
	if (!txd->door_txtr || !txd->door_txtr->img)
		exit(0);
}
