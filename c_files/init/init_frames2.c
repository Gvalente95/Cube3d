/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_frames2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:57:39 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/21 15:43:58 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	get_feet_offset(t_image *img)
{
	t_vec2		pos;
	uint32_t	pixel;

	pos = v2(-1);
	while (++pos.y < img->size.y - 1)
	{
		pos.x = -1;
		while (++pos.x < img->size.x)
		{
			pixel = img->src[pos.y * img->size.x + pos.x];
			if ((pixel >> 24) & 0xFF)
				return (img->size.y - 1 - pos.y);
		}
	}
	return (0);
}

void	init_pokemon_frames(t_md *md, t_texture_data *txd)
{
	char			*path;
	int				i;
	t_vec2			size;

	txd->pkmn = md_malloc(md, sizeof(t_image **) * (PKMN_TYPE_LEN + 1));
	txd->pkmns_mini = md_malloc(md, sizeof(t_image *) * (PKMN_TYPE_LEN + 1));
	i = -1;
	while (++i < PKMN_TYPE_LEN)
	{
		size = v2(-1);
		if (i == Dugtrio || i == Taurus)
			size = v2(64);
		path = ft_megajoin("pokemons/", txd->pkmn_names[i], "/", NULL);
		txd->pkmn[i] = init_images(md, size, path);
		txd->pkmns_mini[i] = copy_image(md, txd->pkmn[i][0], txd->e_sizes2d[0], -1);
		txd->feet_offsets[i] = get_feet_offset(txd->pkmn[i][0]);
		free(path);
	}
	txd->pkmns_mini[PKMN_TYPE_LEN] = NULL;
	txd->pkmn[PKMN_TYPE_LEN] = NULL;
}

void	handle_mobs_frames(t_md *md, t_image ****frames, \
	t_image ****mini, t_mob_types type)
{
	t_texture_data	*td;
	int				action;
	char			*base_path;
	char			*path;

	td = &md->txd;
	*frames = malloc(sizeof(t_image **) * (ENT_ACTION_LEN + 1));
	*mini = malloc(sizeof(t_image **) * (ENT_ACTION_LEN + 1));
	if (!*frames || !*mini)
		return ;
	base_path = ft_megajoin("ent/Mobs/", td->mob_names[type], "/", NULL);
	action = -1;
	while (++action < ENT_ACTION_LEN)
	{
		path = ft_megajoin(base_path, td->ents_act_names[action], "/", NULL);
		(*frames)[action] = init_images(md, v2(-1), path);
		(*mini)[action] = init_images(md, md->txd.e_sizes2d[nt_mob], path);
		free(path);
	}
	(*frames)[action] = NULL;
	(*mini)[action] = NULL;
	free(base_path);
}

void	init_mobs_frames(t_md *md)
{
	int		type;

	md->txd.mobs_txtrs = malloc(sizeof(t_image ***) * (MOB_TYPE_LEN + 1));
	md->txd.mobs_txtrs_mini = malloc(sizeof(t_image ***) * (MOB_TYPE_LEN + 1));
	if (!md->txd.mobs_txtrs || !md->txd.mobs_txtrs_mini)
		return ;
	type = -1;
	while (++type < MOB_TYPE_LEN)
		handle_mobs_frames(md, &md->txd.mobs_txtrs[type], \
			&md->txd.mobs_txtrs_mini[type], type);
	md->txd.mobs_txtrs[type] = NULL;
	md->txd.mobs_txtrs_mini[type] = NULL;
}

t_image	**init_mini(t_md *md, t_image ***mini, char *path)
{
	*mini = init_images(md, md->txd.e_sizes2d[0], path);
	return (init_images(md, v2(md->t_len), path));
}

t_image	**init_weapon(t_md *md, t_image ***mini, char *path)
{
	t_vec2	hud_weap_size;

	hud_weap_size = get_v2(md->win_sz.x / 2, md->win_sz.y / 2);
	*mini = init_images(md, md->txd.e_sizes2d[0], path);
	return (init_images(md, hud_weap_size, path));
}
