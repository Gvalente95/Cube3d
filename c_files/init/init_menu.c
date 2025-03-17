/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:23:31 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 13:23:36 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

//		value_limts.x = min, limits.y = current limits.z = max
static void	init_slider(t_md *md, t_slider *slider, t_vec3f limits, float *value)
{
	t_vec2	fill_end;

	slider->img = init_img(md, get_v2(md->win_size.x / 3, 30), NULL, md->menu.slider_bgr_clr);
	fill_end = get_v2(slider->img->size.x / 2, slider->img->size.y);
	draw_pixels(slider->img, get_v2(0, 0), fill_end, md->menu.slider_fill_clr);
	*value = limits.y;
	slider->value = value;
	slider->limits = limits;
	slider->active = 1;
}

static void	init_sliders(t_md *md, t_menu *menu)
{
	int			i;
	t_vec2		pos;
	t_vec2		win_sz;
	t_slider	*slider;

	menu->slider_bgr_clr = md->rgb[RGB_WHITE];
	menu->slider_fill_clr = md->rgb[RGB_RED];
	ft_strlcpy(menu->sliders[0].label, "red", 50);
	ft_strlcpy(menu->sliders[1].label, "green", 50);
	ft_strlcpy(menu->sliders[2].label, "blue", 50);
	ft_strlcpy(menu->sliders[3].label, "FOV", 50);
	init_slider(md, &menu->sliders[0], get_v3f(0, 1, 2), &menu->hue.r);
	init_slider(md, &menu->sliders[1], get_v3f(0, 1, 2), &menu->hue.g);
	init_slider(md, &menu->sliders[2], get_v3f(0, 1, 2), &menu->hue.b);
	init_slider(md, &menu->sliders[3], get_v3f(0, 60, 600), &md->fov);
	menu->sliders[4].active = 0;
	menu->selected_slider = NULL;
	win_sz = md->win_size;
	i = -1;
	while (menu->sliders[++i].active)
	{
		slider = &menu->sliders[i];
		pos.x = win_sz.x / 2 - slider->img->size.x / 2;
		pos.y = (slider->img->size.y * 2) * i + win_sz.y / 2;
		slider->pos = pos;
	}
}

void	init_menu(t_md *md, t_menu *menu)
{
	menu->active = 0;
	menu->bgr_color = md->rgb[RGB_MAGENT];
	menu->overlay = init_img(md, md->win_size, NULL, menu->bgr_color);
	menu->hue = get_v4f(1, 1, 1, 0);
	init_sliders(md, menu);
}
