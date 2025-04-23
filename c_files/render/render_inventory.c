/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_inventory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:30:56 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/22 23:42:46 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	refresh_inv_opt_bgr(t_md *md, t_inventory *inv)
{
	t_image			*img;
	const char		labels[3][6] = {"USE", "THROW", "BACK"};
	t_txtd			txd;
	int				i;

	img = inv->opt_img;
	flush_img(img, _BLACK, 10, 0);
	txd = (t_txtd){0, 0, _WHITE, md->prm.txt_sc, img};
	i = -1;
	while (++i < 3)
	{
		if (i == inv->opt_i)
			draw_pixels(img, get_v2(txd.x, txd.y), \
			get_v2(img->size.x, md->prm.txt_sc), _WHITE);
		rnd_fast_txt(md, txd, labels[i]);
		txd.y += md->inv.line_height;
	}
	inv->update_opt = 0;
}

static void	draw_inv_labels(t_md *md, t_inventory *inv, t_txtd td, int brdsz)
{
	int	i;

	td.y += md->prm.txt_sc;
	i = -1;
	while (++i < PCKP_TYPE_LEN)
	{
		td.y += inv->line_height;
		td.color = _BLACK;
		if (inv->items[i] > 0)
			td.color = -1;
		if (i == inv->hov_index)
			draw_pixels(inv->img, get_v2(td.x, td.y - 2), \
		get_v2(inv->sz.x * .75f - brdsz, md->prm.txt_sc * 1.4), _WHITE);
		rnd_fast_txt(md, td, "%-15s x%d", md->txd.pickup_names[i], \
			inv->items[i]);
	}
}

void	refresh_inv_bgr(t_md *md, t_inventory *inv)
{
	const int	brdsz = 4;
	const int	brd2 = brdsz * 2;
	t_image		*img;
	t_txtd		td;

	img = inv->img;
	flush_img(img, _WHITE, 10, 0);
	draw_pixels(img, v2(brdsz), sub_vec2(inv->sz, v2(brd2)), _BLACK);
	draw_pixels(img, get_v2(inv->sz.x * .75f, td.y + 25), \
		get_v2(brdsz, inv->sz.y - (td.y + 25)), _WHITE);
	draw_pixels(img, get_v2(0, td.y + 25), get_v2(inv->sz.x, brdsz), _WHITE);
	td = (t_txtd){brd2, brd2, _MAGENT, md->prm.txt_sc, img};
	rnd_fast_txt(md, td, "INVENTORY");
	draw_inv_labels(md, inv, td, brdsz);
	inv->update_img = 0;
}

void	render_inventory(t_md *md, t_inventory *inv)
{
	draw_alpha_img(inv->img, md->screen, inv->p, .8f);
	if (inv->sel_i != -1)
		draw_alpha_img(inv->opt_img, md->screen, inv->opt_p, .9);
}
