/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_elements.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 04:32:24 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/29 19:27:08 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	free_hud(t_md *md, t_hud *hud)
{
	int	fa;

	fa = 0;
	fa += free_image_data(md, hud->overlay);
	fa += free_image_data(md, hud->rgun);
	fa += free_image_data(md, hud->base_sky);
	fa += free_image_data(md, hud->sky);
	fa += free_image_data(md, hud->sky_flipy);
	fa += free_image_data(md, hud->floor);
	fa += free_image_data(md, hud->lock_x_icon);
	fa += free_image_data(md, hud->lock_y_icon);
	fa += free_image_data(md, hud->amm_icon);
	fa += free_image_data(md, hud->amm2_icon);
	fa += free_image_data(md, hud->key_icon);
	fa += free_image_data(md, hud->key2_icon);
	fa += free_image_data(md, hud->hp_icon);
	printf("hud freed %d\n", fa);
	return (fa);
}

int	free_txd(t_md *md, t_texture_data *txd)
{
	int	fa;
	int	i;

	fa = 0;
	fa += free_image_data(md, txd->door_txtr);
	fa += free_image_data(md, txd->door_txtr_mini);
	fa += free_images_data(md, txd->wall_img, "wall_img");
	fa += free_images_data(md, txd->wall_img2d, "wall_img2d");
	fa += free_images_array(md, txd->item_txtr, "item tex");
	fa += free_images_array(md, txd->item_txtr_mini, "minipick tex");
	fa += free_images_array(md, txd->wpn_txtr, "wpn tex");
	fa += free_images_array(md, txd->wpn_txtr_2d, "wpn2d tex");
	fa += free_images_array(md, txd->pkmn, "pokemon tex");
	fa += free_images_data(md, txd->pkmns_mini, "pokemon_mini tex");
	i = -1;
	while (txd->mobs_txtrs && txd->mobs_txtrs[++i])
		fa += free_images_array(md, txd->mobs_txtrs[i], "mob tex");
	i = -1;
	while (txd->mobs_txtrs_mini && txd->mobs_txtrs_mini[++i])
		fa += free_images_array(md, txd->mobs_txtrs_mini[i], "minimob tex");
	fa += safe_free(txd->mobs_txtrs);
	fa += safe_free(txd->mobs_txtrs_mini);
	printf("txd freed %d\n", fa);
	return (fa);
}

int	free_var(t_md *md, t_mmap *mmap, t_fx_data *fx, t_mouse *mouse)
{
	int	fa;
	int	i;

	fa = 0;
	fa += free_image_data(md, md->screen);
	fa += free_image_data(md, md->hud.center);
	fa += safe_free(md->map.buffer);
	fa += free_image_data(md, mmap->bg);
	fa += free_image_data(md, mmap->img);
	fa += free_image_data(md, fx->vignette);
	fa += free_image_data(md, mouse->cursor);
	fa += free_image_data(md, mouse->curs_dtc);
	fa += free_image_data(md, mouse->curs_grb);
	i = -1;
	while (++i < 2)
		if (md->portal.ends[i].e && md->portal.ends[i].e->overlay)
			fa += free_image_data(md, md->portal.ends[i].e->overlay);
	printf("var freed %d\n", fa);
	return (fa);
}

int	free_menu(t_md *md, t_menu *menu)
{
	int	i;
	int	fa;

	fa = 0;
	fa += free_image_data(md, menu->overlay);
	fa += free_image_data(md, menu->freeze_frame);
	i = -1;
	while (++i < 3)
		fa += free_image_data(md, menu->clrp[i].img);
	printf("menu freed %d\n", fa);
	return (fa);
}

int	free_ents(t_md *md)
{
	int		fa;
	t_dblst	*node;
	t_ent	*e;
	int		i;

	fa = 0;
	node = dblst_first(md->entities);
	if (!node)
		return (fa);
	while (node)
	{
		e = (t_ent *)node->content;
		if (e->type == nt_mob)
			fa += free_mob_images(md, e, "mob");
		if (e->frames)
			fa += free_images_data(md, e->frames, "e frames");
		node = node->next;
	}
	fa += dblst_size(md->entities);
	dblst_clear(&md->entities, free);
	printf("ent freed %d\n", fa + 1);
	return (fa);
}
