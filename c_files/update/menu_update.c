/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_update.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:33:17 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/05 18:53:01 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	update_end(t_menu *menu, int *hov, int cur_hov)
{
	if (*hov == -1 || cur_hov != -1)
		return ;
	*hov = -1;
	menu->refresh_ui = 1;
	menu->refresh_bg = 1;
}

int	update_slider(t_md *md, t_slider *sld)
{
	int		x_pos;
	t_vec2	end;
	t_vec2	sz;

	sz = sld->img->size;
	x_pos = sld->base_point;
	if (!md->key_prs[CTRL_KEY])
		x_pos = ((md->mouse.real.x - sld->pos.x) * (sld->steps - 1)) / sz.x;
	if (x_pos == sld->point)
		return (0);
	sld->point = minmax(0, sld->steps - 1, x_pos);
	*sld->value = sld->limits.x + ((sld->limits.z - sld->limits.x) * \
		((float)sld->point / (sld->steps - 1)));
	if (md->prm.fov <= 0)
		md->prm.fov = 1;
	flush_img(sld->img, md->menu.slider_bgr_clr, -1, -1);
	end = get_v2((sz.x * sld->point) / (sld->steps - 1), sz.y);
	draw_pixels(sld->img, v2(0), end, md->menu.slider_fill_clr);
	md->plr.pos.z = -md->prm.height;
	md->cam.pos.z = md->plr.pos.z - md->prm.height;
	if (!ft_strncmp(sld->label, "win", 3))
		return (replace_window(md, md->prm.win_x, md->prm.win_y));
	render(md);
	return (1);
}

void	update_sliders(t_md *md, t_menu *menu, t_vec2 sz)
{
	t_slider		*sld;
	int				i;
	int				cur_hov;

	cur_hov = -1;
	i = -1;
	while (menu->sliders[++i].active)
	{
		sld = &menu->sliders[i];
		if (!v2_bounds(md->mouse.real, sld->pos, get_v2(sz.x, sz.y * 1.3)))
			continue ;
		cur_hov = i;
		if (menu->slider_hov != cur_hov)
		{
			menu->slider_hov = i;
			menu->refresh_ui = 1;
			menu->refresh_bg = 1;
		}
		if (md->mouse.pressed == MOUSE_PRESS)
			menu->selected_slider = sld;
		return ;
	}
	update_end(menu, &menu->slider_hov, cur_hov);
}

void	update_buttons(t_md *md, t_menu *menu, t_vec2 size)
{
	t_button			*but;
	int					cur_hov;
	int					i;

	cur_hov = -1;
	i = -1;
	while (menu->buttons[++i].active)
	{
		but = &menu->buttons[i];
		if (!v2_bounds(md->mouse.real, but->pos, size))
			continue ;
		cur_hov = i;
		if (cur_hov != menu->button_hov)
		{
			menu->button_hov = cur_hov;
			menu->refresh_ui = 1;
		}
		if (md->mouse.click != MOUSE_PRESS)
			continue ;
		*but->value = !(*but->value);
		render(md);
		menu->refresh_bg = 1;
		menu->refresh_ui = 1;
	}
	update_end(menu, &menu->button_hov, cur_hov);
}

int	update_menu(t_md *md, t_menu *menu)
{
	if (md->mouse.click == MOUSE_PRESS)
		play_sound(md, AU_MOUSE_CLICK);
	if (md->mouse.click == MOUSE_RELEASE)
		play_sound(md, AU_MOUSE_RELEASE);
	update_input(md);
	if (!menu->selected_slider)
		update_sliders(md, menu, menu->sliders[0].img->size);
	if (menu->selected_slider)
	{
		if (!md->mouse.pressed)
		{
			menu->refresh_ui = 1;
			menu->selected_slider = NULL;
		}
		else
			menu->refresh_bg = update_slider(md, menu->selected_slider);
	}
	else
		update_buttons(md, menu, \
			get_v2(md->prm.txt_sc * 15, md->prm.txt_sc + 10));
	render_menu(md, menu);
	reset_mlx_values(md);
	return (1);
}
