/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_menu_elements.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 21:25:49 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 04:35:27 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	init_button(t_md *md, t_button *button, int *value, char *label)
{
	ft_strlcpy(button->label, label, 50);
	if (*value)
		button->img = init_img(md, get_v2(30, 30), NULL, md->rgb[RGB_GREEN]);
	else
		button->img = init_img(md, get_v2(30, 30), NULL, md->rgb[RGB_RED]);
	button->value = value;
	button->active = 1;
}

//		value_limts.x = min, limits.y = current limits.z = max
static void	init_sldr(t_md *md, t_slider *sld, t_vec3f limt, float *value)
{
	t_vec2	fill_end;

	sld->img = init_img(md, get_v2(md->win_size.x / 3, 30), \
		NULL, md->menu.slider_bgr_clr);
	sld->point = minmax(0, 10, ((limt.y - limt.x) / (limt.z - limt.x)) * 10);
	fill_end = get_v2((sld->img->size.x / 10) * sld->point, sld->img->size.y);
	draw_pixels(sld->img, get_v2(0, 0), fill_end, md->menu.slider_fill_clr);
	*value = limt.y;
	sld->value = value;
	sld->limits = limt;
	sld->active = 1;
}

static void	init_sliders(t_md *md, t_menu *menu, int i)
{
	t_slider	*sldrs;

	sldrs = menu->sliders;
	i = 0;
	ft_strlcpy(sldrs[i].label, "red", 50);
	init_sldr(md, &sldrs[i++], get_v3f(0, 1, 2), &menu->hue.r);
	ft_strlcpy(sldrs[i].label, "green", 50);
	init_sldr(md, &sldrs[i++], get_v3f(0, 1, 2), &menu->hue.g);
	ft_strlcpy(sldrs[i].label, "blue", 50);
	init_sldr(md, &sldrs[i++], get_v3f(0, 1, 2), &menu->hue.b);
	ft_strlcpy(sldrs[i].label, "FOV", 50);
	init_sldr(md, &sldrs[i++], get_v3f(0, 60, 600), &md->fov);
	ft_strlcpy(sldrs[i].label, "Text scale", 50);
	init_sldr(md, &sldrs[i++], get_v3f(0, \
		md->txt_scale, md->txt_scale * 2), &md->txt_scale);
	ft_strlcpy(sldrs[i].label, "Ray Depth", 50);
	init_sldr(md, &sldrs[i++], \
		get_v3f(0, md->ray_depth, md->ray_depth * 2), &md->ray_depth);
	ft_strlcpy(sldrs[i].label, "RGB Dist", 50);
	init_sldr(md, &sldrs[i++], get_v3f(0, 0, 10), &md->rgb_distortion);
	ft_strlcpy(sldrs[i].label, "Scanlines", 50);
	init_sldr(md, &sldrs[i++], get_v3f(0, 1, 1), &md->scanlines);
	ft_strlcpy(sldrs[i].label, "Dithering", 50);
	init_sldr(md, &sldrs[i++], get_v3f(0, 0, 1), &md->dithering);
	sldrs[i].active = 0;
}

static void	init_buttons(t_md *md, t_menu *menu)
{
	int	i;

	i = 0;
	init_button(md, &menu->buttons[i++], &md->lock_rotation.x, "lock_x");
	init_button(md, &menu->buttons[i++], &md->lock_rotation.y, "lock_y");
	init_button(md, &menu->buttons[i++], &md->debug_mode, "debug mode");
	init_button(md, &menu->buttons[i++], &md->show_rays, "show 2d rays");
	init_button(md, &menu->buttons[i++], &md->real_mode, "real mode");
	init_button(md, &menu->buttons[i++], &md->mmap.active, "minimap");
	init_button(md, &menu->buttons[i++], &md->anti_aliasing, "anti-aliasing");
	menu->buttons[i].active = 0;
}

void	init_menu_elements(t_md *md, t_menu *menu)
{
	menu->selected_slider = NULL;
	menu->slider_bgr_clr = md->rgb[RGB_WHITE];
	menu->slider_fill_clr = md->rgb[RGB_RED];
	init_sliders(md, menu, 0);
	init_buttons(md, menu);
}
