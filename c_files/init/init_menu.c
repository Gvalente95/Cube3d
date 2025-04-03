/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:23:31 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/03 10:39:55 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	set_menu_pos(t_md *md, t_menu *menu, \
	t_vec3 sldr_offst, t_vec3 but_ofst)
{
	int			i;
	t_vec2		pos;
	t_vec2		win_sz;
	t_slider	*slider;
	t_button	*button;

	win_sz = md->win_sz;
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
		pos.x = win_sz.x / 2 + but_ofst.x;
		pos.y = ((md->prm.txt_sc + but_ofst.z) \
			* i) + win_sz.y / 2 + but_ofst.y;
		button->pos = pos;
	}
}

void	init_menu_overlay(t_md *md, t_menu *menu)
{
	menu->bgr_color = md->rgb[RGB_BLACK];
	menu->overlay = init_img(md, md->win_sz, NULL, menu->bgr_color);
	apply_dithering(menu->overlay, 10, \
		md->fx.palette, md->fx.palette_size);
	apply_scanlines(menu->overlay, 10);
	rnd_abs_txt(md, get_v4(50, 50, -1, md->prm.txt_sc * 10), "CUBE3d");
}

void	init_menu(t_md *md, t_menu *menu)
{
	menu->active = 1;
	md->fx.hue = get_v4f(1, 1, 1, 0);
	init_menu_overlay(md, menu);
	init_menu_elements(md, menu);
	set_menu_pos(md, menu, \
			get_v3(-200, -200, 5), get_v3(200, -200, 10));
	menu->active = 0;
	menu->refresh_ui = 0;
	menu->refresh_bg = 0;
}

int	replace_window(t_md *md, int new_w, int new_h)
{
	mlx_destroy_window(md->mlx, md->win);
	md->win_sz = get_v2(new_w, new_h);
	md->win = mlx_new_window(md->mlx, new_w, new_h, "Cube3D");
	if (md->is_linux)
	{
		mlx_hook(md->win, KeyPress, KeyPressMask, handle_key_press, md);
		mlx_hook(md->win, KeyRelease, KeyReleaseMask, handle_key_release, md);
		mlx_hook(md->win, DestroyNotify, StructureNotifyMask, close_window, md);
	}
	else
	{
		mlx_hook(md->win, 2, 0, handle_key_press, md);
		mlx_hook(md->win, 3, 0, handle_key_release, md);
		mlx_hook(md->win, 17, 0, close_window, md);
	}
	mlx_mouse_hook(md->win, mouse_event_handler, md);
	mlx_hook(md->win, 5, ButtonReleaseMask, mouse_release_handler, md);
	mlx_hook(md->win, 6, PointerMotionMask, mouse_motion_handler, md);
	set_menu_pos(md, &md->menu, get_v3(-200, -200, 5), get_v3(300, -200, 5));
	render(md);
	md->menu.selected_slider = NULL;
	md->mouse.click = MOUSE_NOPRESS;
	md->mouse.pressed = MOUSE_NOPRESS;
	return (md->menu.refresh_bg = 1, md->menu.refresh_ui = 1, -1);
}
