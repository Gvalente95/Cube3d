/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_pointed.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 19:49:34 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/21 15:20:26 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	display_options(t_md *md, const char **labels, int len, t_txtd txtd)
{
	const t_texture_data	txd = md->txd;
	int						i;

	txtd.y += md->prm.txt_sc;
	i = -1;
	while (++i < len)
	{
		txtd.y += md->prm.txt_sc;
		txtd.color = -1;
		if (txd.opt_index == i)
			txtd.color = _RED;
		rnd_fast_txt(md, txtd, labels[i]);
	}
}

void	show_pointed_data(t_md *md, t_vec2 p, t_ent *e)
{
	const t_vec2	sz = (t_vec2){md->prm.txt_sc * 10, md->prm.txt_sc * 5};
	const t_txtd	txt = (t_txtd){p.x, p.y, _BLUE, md->prm.txt_sc, md->screen};
	const char		*pokemon_options[3] = {"Capture", "Attack", "Exit"};
	const char		*pickup_options[3] = {"Take", "info", "Exit"};
	const int		key = md->key_click;

	draw_pixels(md->screen, p, sz, set_alpha(_BLACK, .4));
	wrap_int(&md->txd.opt_index, 0, 2, (key == NUM_S_KEY) - (key == NUM_W_KEY));
	if (e->type == nt_pokemon)
	{
		rnd_fast_txt(md, txt, md->txd.pkmn_names[e->mob_type]);
		display_options(md, pokemon_options, 3, txt);
	}
	else if (e->type == nt_pickup)
	{
		rnd_fast_txt(md, txt, md->txd.pickup_names[e->pckp_type]);
		display_options(md, pickup_options, 2, txt);
	}
	else if (e->type == nt_mob)
		rnd_fast_txt(md, txt, md->txd.mob_names[e->mob_type]);
}
