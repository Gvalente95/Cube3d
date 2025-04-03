/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:30:53 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/03 13:25:36 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	render_slider(t_md *md, t_slider *sldr, t_image *screen, float alpha)
{
	t_vec4		txt_d;

	draw_alpha_img(sldr->img, screen, sldr->pos, alpha);
	txt_d.a = md->prm.txt_sc;
	txt_d.b = md->rgb[RGB_BLACK];
	if (alpha == 1)
		txt_d.b = md->rgb[RGB_WHITE];
	txt_d.r = sldr->pos.x - (txt_d.a) * ft_strlen(sldr->label) - 30;
	txt_d.g = sldr->pos.y + sldr->img->size.y / 2 - md->prm.txt_sc / 2;
	rnd_abs_txt(md, txt_d, "%s", sldr->label);
	if (!md->prm.debug_mode && md->menu.selected_slider != sldr)
		return ;
	txt_d.r = sldr->pos.x + sldr->img->size.x - md->prm.txt_sc * 5;
	rnd_abs_txt(md, txt_d, "%.1f", *sldr->value);
}

void	render_sliders(t_md *md, t_menu *menu, t_image *screen)
{
	t_slider	*slider;
	int			i;

	i = -1;
	while (menu->sliders[++i].active)
	{
		slider = &menu->sliders[i];
		if (menu->slider_hov == i)
			render_slider(md, slider, screen, 1);
		else
			render_slider(md, slider, screen, .5);
	}
}

void	render_buttons(t_md *md, t_menu *menu)
{
	t_button	*but;
	t_vec4		txt_data;
	int			i;

	i = -1;
	while (menu->buttons[++i].active)
	{
		but = &menu->buttons[i];
		txt_data.a = md->prm.txt_sc;
		txt_data.r = but->pos.x;
		txt_data.g = but->pos.y;
		txt_data.b = md->rgb[RGB_RED + (*but->value == 1)];
		if (menu->button_hov == i)
			txt_data.b += 500;
		rnd_abs_txt(md, txt_data, "%s", but->label);
		txt_data.r -= 15;
		txt_data.b = md->rgb[RGB_BLUE];
		txt_data.a *= .75;
		if (but->key_show[0])
			rnd_abs_txt(md, txt_data, "%s", but->key_show);
	}
}

void	center_txt(t_md *md, t_vec2 pos_ofst, int scale, char *name)
{
	const t_vec2	win_cntr = (t_vec2){md->win_sz.x / 2, md->win_sz.y / 2};
	t_vec2			pos;
	const int		title_color = md->rgb[RGB_WHITE];
	t_vec4			txt_data;

	pos.x = win_cntr.x - scale * (ft_strlen(name) / 2) + pos_ofst.x;
	pos.y = win_cntr.y - scale / 2 + pos_ofst.y;
	txt_data = get_v4(pos.x, pos.y, title_color, scale);
	rnd_abs_txt(md, txt_data, name);
}

void	render_menu(t_md *md, t_menu *menu)
{
	const t_vec2	win_sz = md->win_sz;

	if (menu->refresh_bg)
	{
		if (menu->freeze_frame)
			free_image_data(md, menu->freeze_frame);
		menu->freeze_frame = copy_image(md, md->screen, get_v2(-1, -1), -1);
		apply_vignette(menu->freeze_frame, 0.5, md->rgb[RGB_BLACK]);
		menu->refresh_bg = 0;
	}
	if (menu->refresh_ui)
	{
		render_sliders(md, menu, menu->freeze_frame);
		render_buttons(md, menu);
		center_txt(md, get_v2(0, -win_sz.y * .4), win_sz.x / 20, "GRASS");
		center_txt(md, get_v2(0, -win_sz.y * .3), win_sz.x / 60, "cube3D");
		menu->refresh_ui = 0;
	}
	else if (menu->selected_slider)
		render_slider(md, menu->selected_slider, menu->freeze_frame, 1);
	md->mlx_put(md->mlx, md->win, menu->freeze_frame->img, 0, 0);
}
