/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inventory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:31:25 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/22 23:56:41 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	collect_item(t_md *md, t_inventory *inv, t_ent *e)
{
	remove_ent_at_cord(md, get_v2(e->coord.x, e->coord.y));
	e->is_active = 0;
	inv->items[e->pckp_type]++;
	play_sound(md, AU_GRAB);
}

int	use_item(t_md *md, t_inventory *inv, int item_index, int option)
{
	if (!inv->items[item_index])
		return (play_sound(md, AU_MENU_OFF), 1);
	if (option == 2)
		return (1);
	inv->update_img = 1;
	if (item_index == Pokeball)
	{
		inv->hold_pkbl = 1;
		set_inventory(md, inv, 0);
	}
	inv->items[item_index]--;
	play_sound(md, AU_GRAB);
	return (inv->items[item_index]);
}

void	init_inventory(t_md *md, t_inventory *inv)
{
	int				i;
	const t_vec2	sz = v2(md->prm.txt_sc * 20);
	const int		tx_sc = md->prm.txt_sc;
	const int		line_h = md->prm.txt_sc * 1.75f;
	const t_vec2	pannel_sz = (t_vec2){(tx_sc * 1.2f) * 5, line_h * 3};

	i = -1;
	while (++i < PCKP_TYPE_LEN)
		inv->items[i] = 0;
	inv->active = 0;
	inv->sel_i = -1;
	inv->hov_index = 0;
	inv->img = init_img(md, sz, NULL, -1);
	inv->sz = inv->img->size;
	inv->p = (t_vec2){md->win_sz.x / 2 - sz.x / 2, md->win_sz.y / 2 - sz.y / 2};
	inv->throwing_pokebal = 0;
	inv->opt_img = init_img(md, pannel_sz, NULL, _BLACK);
	inv->opt_sz = inv->opt_img->size;
	inv->pkbl_sz = v2(md->win_sz.x * .2);
	inv->pkball = copy_image(md, \
		md->txd.pickup_txtr[Pokeball][0], inv->pkbl_sz, -1);
	inv->hold_pkbl = 0;
	inv->line_height = line_h;
	refresh_inv_opt_bgr(md, inv);
	refresh_inv_bgr(md, inv);
}

void	set_inventory(t_md *md, t_inventory *inv, int active)
{
	if (!inv->active && active)
		inv->update_img = 1;
	if (active)
		mlx_mouse_show(md->mlx, md->win);
	else
		mlx_mouse_hide(md->mlx, md->win);
	inv->hov_index = 0;
	inv->opt_i = 0;
	inv->sel_i = -1;
	inv->active = active;
}
