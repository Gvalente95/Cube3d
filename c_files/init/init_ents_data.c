/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ents_data.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:57:39 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/14 01:18:30 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	store_entities_sizes(t_md *md, t_vec2 base)
{
	int		i;

	md->e_sizes[nt_plr] = get_v2(base.x * .8, base.y * .8);
	md->e_sizes[nt_mob] = get_v2(base.x / 2, base.y / 2);
	md->e_sizes[nt_coin] = get_v2(base.x / 3, base.y / 3);
	md->e_sizes[nt_key] = base;
	md->e_sizes[nt_door] = base;
	md->e_sizes[nt_portal] = base;
	md->e_sizes[nt_wall] = base;
	md->e_sizes[nt_empty] = base;
	md->e_sizes[nt_axe] = base;
	md->e_sizes[nt_ground] = base;
	md->e_sizes[nt_tile] = base;
	md->e_sizes[nt_mover] = base;
	md->e_sizes[nt_bush] = base;
	md->e_sizes[nt_spike] = base;
	md->e_sizes[nt_thrower] = base;
	md->e_sizes[nt_trees] = base;
	md->e_sizes[nt_star] = base;
	md->e_sizes[nt_road] = base;
	md->e_sizes[nt_env] = base;
	md->e_sizes[nt_prt] = base;
	i = -1;
	while (++i < ENT_TYPE_LEN)
		md->e_sizes_2d[i] = get_v2(md->size_2d, md->size_2d);
}

static void	init_entities_textures(t_md *md)
{
	char		*path;
	char		*with_format;
	int			i;

	i = -1;
	while (++i < ENT_TYPE_LEN)
	{
		path = ft_megajoin(md->img_dir_path, "/ent/", md->ents_tp_names[i], "/");
		md->e_frms[i][0] = init_imgs_data(md, md->e_sizes[i], path);
		free(path);
		path = ft_megajoin(md->img_dir_path, "/ent/", md->ents_tp_names[i], "/0");
		with_format = ft_strjoin(path, md->img_format);
		free(path);
		md->txtr_2d[i] = init_img_data(md, md->e_sizes_2d[i], with_format, -1);
		free(with_format);
	}
	md->e_frms[i] = NULL;
}

int	allocate_entities_textures(t_md *md)
{
	int		i;

	md->txtr_2d = malloc(sizeof(t_image *) * (ENT_TYPE_LEN + 1));
	md->e_frms = malloc(sizeof(t_image ***) * (ENT_TYPE_LEN + 1));
	if (!md->e_frms || !md->txtr_2d)
		return (printf("alloc failed for frames"), 0);
	md->e_frms[ENT_TYPE_LEN] = NULL;
	md->txtr_2d[ENT_TYPE_LEN] = NULL;
	i = -1;
	while (++i < ENT_TYPE_LEN)
		md->e_frms[i] = md_malloc(md, sizeof(t_image ***) * ENT_ACTION_LEN);
	return (1);
}

void	init_ents_data(t_md *md)
{
	store_entities_sizes(md, get_v2(md->t_len, md->t_len));
	allocate_entities_textures(md);
	init_entities_textures(md);
}
