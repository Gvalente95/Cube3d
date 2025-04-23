/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_pointed.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 19:49:34 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/22 21:31:33 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	render_options(t_md *md, const char **labels, t_txtd txtd)
{
	int	i;

	txtd.y += md->prm.txt_sc;
	i = -1;
	while (++i < 3)
	{
		txtd.y += md->prm.txt_sc;
		txtd.color = -1;
		if (md->txd.opt_i == i)
			txtd.color = _RED;
		rnd_fast_txt(md, txtd, labels[i]);
	}
}

static int	display_options(t_md *md, t_ent *e, const char **lbls, t_txtd txtd)
{
	render_options(md, lbls, txtd);
	if (md->key_click != ENTER_KEY && \
		(md->mouse.click != MOUSE_PRESS || md->txd.opt_i == -1))
		return (0);
	if (md->txd.opt_i == 2)
		return (md->txd.last_pointed = NULL, 1);
	if (e->type == nt_pickup)
		collect_item(md, &md->inv, e);
	if (md->txd.opt_i == 1)
		return (0);
	if (e->type == nt_pokemon)
		use_item(md, &md->inv, Pokeball, 0);
	else if (e->type == nt_pickup)
		use_item(md, &md->inv, e->pckp_type, 0);
	return (1);
}

void	show_pointed_data(t_md *md, t_vec2 p, t_ent *e)
{
	const t_vec2	sz = (t_vec2){md->prm.txt_sc * 10, md->prm.txt_sc * 5};
	const t_txtd	txt = (t_txtd){p.x, p.y, _BLUE, md->prm.txt_sc, md->screen};
	const char		*pokemon_options[3] = {"Capture", "Attack", "Exit"};
	const char		*pickup_options[3] = {"Use", "Take", "Exit"};

	draw_pixels(md->screen, p, sz, set_alpha(_BLACK, .4));
	wrap_int(&md->txd.opt_i, 0, 2, minmax(-1, 1, md->mouse.delta.y));
	if (e->type == nt_pokemon)
	{
		rnd_fast_txt(md, txt, md->txd.pkmn_names[e->mob_type]);
		display_options(md, e, pokemon_options, txt);
	}
	else if (e->type == nt_pickup)
	{
		rnd_fast_txt(md, txt, md->txd.pickup_names[e->pckp_type]);
		display_options(md, e, pickup_options, txt);
	}
	else if (e->type == nt_mob)
		rnd_fast_txt(md, txt, md->txd.mob_names[e->mob_type]);
	if (md->txd.opt_i == -1)
		md->txd.opt_i++;
}
