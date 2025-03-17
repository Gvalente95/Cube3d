/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_menu.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:33:17 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/16 21:51:02 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	update_keys(t_md *md)
{
	t_vec2	new_wrd;

	if (md->key_clicked == ESC_KEY)
	{
		new_wrd = md->mouse_world_pos;
		mlx_mouse_move(md->win, md->prev_mouse.x, md->prev_mouse.y);
		md->prev_mouse = new_wrd;
		md->menu.active = 0;
		return ;
	}
}

int	update_menu(t_md *md, t_menu *menu)
{
	update_keys(md);
	flush_img(menu->overlay, menu->bgr_color, -1, 0);
	draw_transp_img(md->screen, menu->overlay, get_v2(0, 0), 0.2);
	shift_rgb(menu->overlay, get_v4f(0.3,0.3,.3,1));
	render_cursor(md, menu->overlay, 0);
	mlx_put_image_to_window(md->mlx, md->win, md->menu.overlay->img, 0, 0);
	reset_mlx_values(md);
	update_time(md, &md->timer);
	return (1);
}
