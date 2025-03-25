/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:23:31 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/23 21:19:36 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	set_elements_positions(t_md *md, t_menu *menu, \
	t_vec3 sldr_offst, t_vec3 but_ofst)
{
	int			i;
	t_vec2		pos;
	t_vec2		win_sz;
	t_slider	*slider;
	t_button	*button;

	win_sz = md->win_size;
	i = -1;
	while (menu->sliders[++i].active)
	{
		slider = &menu->sliders[i];
		pos.x = win_sz.x / 2 - slider->img->size.x / 2 + sldr_offst.x;
		pos.y = (slider->img->size.y + sldr_offst.z) \
			* i + win_sz.y / 2 + sldr_offst.y;
		slider->pos = pos;
	}
	i = -1;
	while (menu->buttons[++i].active)
	{
		button = &menu->buttons[i];
		pos.x = win_sz.x / 2 - button->img->size.x / 2 + but_ofst.x;
		pos.y = (button->img->size.y + sldr_offst.z) \
			* i + win_sz.y / 2 + but_ofst.y;
		button->pos = pos;
	}
}

void	init_menu_overlay(t_md *md, t_menu *menu)
{
	menu->bgr_color = md->rgb[RGB_BLACK];
	menu->overlay = init_img(md, md->win_size, NULL, menu->bgr_color);
	apply_dithering(menu->overlay, 10, \
		md->fx.palette, md->fx.palette_size);
	apply_scanlines(menu->overlay, 10);
	rnd_abs_txt(md, get_v4(50, 50, -1, md->prm.txt_scale * 10), "CUBE3d");
}

void	init_menu(t_md *md, t_menu *menu)
{
	menu->hov = 0;
	menu->active = 1;
	md->fx.hue = get_v4f(1, 1, 1, 0);
	init_menu_overlay(md, menu);
	init_menu_elements(md, menu);
	set_elements_positions(md, menu, \
			get_v3(-200, -200, 5), get_v3(300, -200, 5));
	menu->active = 0;
	menu->refresh = 0;
}
