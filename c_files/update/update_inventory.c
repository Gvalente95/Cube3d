/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_inventory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:45:16 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/22 23:42:36 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	close_option_pannel(t_inventory *inv)
{
	inv->sel_i = -1;
	inv->opt_i = 0;
}

void	handle_inv_mouse(t_md *md, t_inventory *inv, t_image *img)
{
	const t_mouse	ms = md->mouse;
	const int		txsc = md->prm.txt_sc;
	float			relative_y;
	int				hov;
	const float		line_h = inv->line_height;

	if (ms.pressed == MOUSE_PRESS)
	{
		inv->p.x = minmax(0, md->win_sz.x - img->size.x, inv->p.x + ms.delta.x);
		inv->p.y = minmax(0, md->win_sz.y - img->size.y, inv->p.y + ms.delta.y);
	}
	if (ms.real.y <= inv->p.y + 8 + txsc + line_h)
		return ;
	relative_y = ms.real.y - (inv->p.y + 8 + txsc + line_h);
	hov = minmax(0, PCKP_TYPE_LEN - 1, (int)(relative_y / line_h));
	if (inv->hov_index != hov && (inv->sel_i < 0 || ms.click))
	{
		inv->hov_index = hov;
		inv->update_img = 1;
	}
	if (ms.click != MOUSE_PRESS || inv->sel_i == hov || inv->items[hov] <= 0)
		return ;
	inv->sel_i = hov;
	inv->update_img = 1;
	play_sound(md, AU_MOUSE_CLICK);
}

void	handle_opt_mouse(t_md *md, t_inventory *inv)
{
	const t_mouse	ms = md->mouse;
	float			relative_y;
	int				hov;
	const float		line_h = inv->line_height;

	relative_y = ms.real.y - inv->opt_p.y;
	hov = minmax(0, 2, (int)(relative_y / line_h));
	if (inv->opt_i != hov)
	{
		inv->opt_i = hov;
		inv->update_opt = 1;
	}
	if (ms.click != MOUSE_PRESS)
		return ;
	if (inv->opt_i == 2 || !use_item(md, inv, inv->sel_i, inv->opt_i == 1))
		close_option_pannel(inv);
}

int	handle_mouse_interraction(t_md *md, t_inventory *inv)
{
	const t_vec2	mp = md->mouse.real;
	int				delta_i;

	if (v2_touch(mp, v2(1), inv->p, inv->sz))
		return (handle_inv_mouse(md, inv, inv->img), 1);
	else if (v2_touch(mp, v2(1), inv->opt_p, inv->opt_sz))
		return (handle_opt_mouse(md, inv), 1);
	if (md->timer.time % 8 != 0 || !md->mouse.delta.y)
		return (0);
	delta_i = ft_sign(md->mouse.delta.y);
	if (inv->sel_i != -1)
	{
		inv->opt_i = minmax(0, 2, inv->opt_i + delta_i);
		inv->update_opt = 1;
		return (1);
	}
	inv->hov_index = minmax(0, PCKP_TYPE_LEN - 1, inv->hov_index + delta_i);
	inv->update_img = 1;
	return (1);
}

void	update_inventory(t_md *md, t_inventory *inv)
{
	const int	key = md->key_click;

	if (inv->sel_i == -1)
		inv->opt_i = 0;
	if (key == ENTER_KEY || md->mouse.click == MOUSE_PRESS)
	{
		if (inv->sel_i < 0 && inv->items[inv->hov_index] > 0)
			inv->sel_i = inv->hov_index;
		else if (inv->opt_i == 2 || !use_item(md, inv, inv->sel_i, inv->opt_i))
			close_option_pannel(inv);
	}
	if (inv->sel_i < 0 && wrap_int(&inv->hov_index, 0, PCKP_TYPE_LEN - 1, \
			(key == S_KEY) - (key == W_KEY)))
		inv->update_img = 1;
	else if (wrap_int(&inv->opt_i, 0, 2, (key == S_KEY) - (key == W_KEY)))
		inv->update_opt = 1;
	handle_mouse_interraction(md, inv);
	if (inv->update_img)
		refresh_inv_bgr(md, inv);
	if (inv->update_opt)
		refresh_inv_opt_bgr(md, inv);
	if (inv->sel_i == -1)
		return ;
	inv->opt_p.x = inv->p.x - inv->opt_sz.x;
	inv->opt_p.y = inv->p.y + 8 + inv->line_height * (inv->sel_i + 2);
}
