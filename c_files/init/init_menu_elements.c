/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_menu_elements.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 21:25:49 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 16:09:55 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	init_button(t_md *md, t_button *button, int *value, char *label)
{
	t_vec2	size;

	size = get_v2(20, 20);
	ft_strlcpy(button->label, label, 50);
	if (*value)
		button->img = init_img(md, size, NULL, md->rgb[RGB_GREEN]);
	else
		button->img = init_img(md, size, NULL, md->rgb[RGB_RED]);
	button->value = value;
	button->active = 1;
}

//data: r = min, data.g = current data.b = max data.a = (int)steps
static void	init_sldr(t_md *md, char *label, t_vec4f data, float *value)
{
	t_slider	*sld;
	t_vec2		fill_end;
	t_vec3f		slider_limits;

	sld = &md->menu.sliders[md->menu.slider_index++];
	sld->steps = (int)data.a;
	slider_limits = get_v3f(data.r, data.g, data.b);
	ft_strlcpy(sld->label, label, 50);
	sld->img = init_img(md, get_v2(md->win_size.x / 3, 20), NULL, \
	md->menu.slider_bgr_clr);
	sld->point = minmax(0, sld->steps - 1, \
	((data.g - data.r) / (data.b - data.r)) * (sld->steps - 1));
	sld->base_point = sld->point;
	fill_end = get_v2((sld->img->size.x * sld->point) / (sld->steps - 1), \
		sld->img->size.y);
	draw_pixels(sld->img, v2(0), fill_end, md->menu.slider_fill_clr);
	*value = data.g;
	sld->value = value;
	sld->limits = slider_limits;
	sld->active = 1;
}

static void	init_sliders(t_md *md, t_menu *menu, \
	t_parameters *pm, t_post_fx_data *fx)
{
	menu->slider_index = 0;
	init_sldr(md, "Speed", \
		get_v4f(PLRSPD / 2, PLRSPD, PLRSPD * 2, 100), &pm->plr_speed);
	init_sldr(md, "Rot Speed", get_v4f(.01, MOUSESPD, 1, 10), &pm->rot_speed);
	init_sldr(md, "Zoom", get_v4f(1, md->prm.zoom, 500, 100), &pm->zoom);
	init_sldr(md, "Field Of View", get_v4f(0, 60, 600, 100), &pm->fov);
	init_sldr(md, "Height", get_v4f(0, HEIGHT, md->t_len, 100), &pm->height);
	init_sldr(md, "Difficulty", get_v4f(1, 1, 10, 10), &pm->difficulty);
	init_sldr(md, "Fog", get_v4f(0, md->fx.fog, 2, 100), &md->fx.fog);
	init_sldr(md, "noise", get_v4f(0, 0, 1, 100), &fx->noise);
	init_sldr(md, "txt scale", \
		get_v4f(10, pm->txt_scale, 30, 20), &pm->txt_scale);
	init_sldr(md, "ray depth", \
		get_v4f(0, pm->ray_depth, pm->ray_depth * 2, 100), &pm->ray_depth);
	init_sldr(md, "red", get_v4f(0, 1, 2, 100), &fx->hue.r);
	init_sldr(md, "green", get_v4f(0, 1, 2, 100), &fx->hue.g);
	init_sldr(md, "blue", get_v4f(0, 1, 2, 100), &fx->hue.b);
	init_sldr(md, "rgb dist", get_v4f(0, 0, 10, 100), &fx->rgb_distortion);
	init_sldr(md, "scanlines", get_v4f(0, 0, 1, 100), &fx->scanlines);
	init_sldr(md, "dithering", get_v4f(0, 0, 1, 100), &fx->dithering);
	init_sldr(md, "Chrom", get_v4f(0, 0, 5, 100), &fx->chrom_amount);
	init_sldr(md, "rgb glitch", get_v4f(0, 0, 10, 100), &fx->glitch_intensity);
	init_sldr(md, "barrel", get_v4f(0, 0, 5, 100), &fx->barrel_amount);
	init_sldr(md, "color banding", get_v4f(0, 0, 1, 100), &fx->color_band);
	init_sldr(md, "glow", get_v4f(0, 0, 1, 100), &fx->bloom_threshold);
	menu->sliders[menu->slider_index].active = 0;
}

static void	init_buttons(t_md *md, t_menu *menu)
{
	int	i;

	i = 0;
	init_button(md, &menu->buttons[i++], &md->prm.real_mode, "real mode");
	init_button(md, &menu->buttons[i++], &md->prm.ent_mode, "ent mode");
	init_button(md, &menu->buttons[i++], &md->prm.debug_mode, "debug mode");
	init_button(md, &menu->buttons[i++], &md->prm.show_rays, "show 2d rays");
	init_button(md, &menu->buttons[i++], &md->mmap.active, "minimap");
	init_button(md, &menu->buttons[i++], &md->mouse.lock_rotation.x, "lock_x");
	init_button(md, &menu->buttons[i++], &md->mouse.lock_rotation.y, "lock_y");
	init_button(md, &menu->buttons[i++], &md->fx.anti_alias, "aliasing");
	menu->buttons[i].active = 0;
}

void	init_menu_elements(t_md *md, t_menu *menu)
{
	menu->selected_slider = NULL;
	menu->slider_bgr_clr = md->rgb[RGB_WHITE];
	menu->slider_fill_clr = md->rgb[RGB_RED];
	init_sliders(md, menu, &md->prm, &md->fx);
	init_buttons(md, menu);
}
