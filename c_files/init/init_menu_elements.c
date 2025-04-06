/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_menu_elements.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 21:25:49 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/05 20:27:18 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

//data: r = min, data.g = current data.b = max data.a = (int)steps
static void	inisld(t_md *md, char *label, t_vec4f data, float *value)
{
	t_slider	*sld;
	t_vec2		fill_end;
	t_vec3f		slider_limits;

	sld = &md->menu.sliders[md->menu.slider_index];
	sld->index = md->menu.slider_index++;
	sld->steps = (int)data.a;
	slider_limits = get_v3f(data.r, data.g, data.b);
	ft_strlcpy(sld->label, label, 50);
	sld->img = init_img(md, get_v2(md->win_sz.x / 6, 20), NULL, \
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
	t_parameters *pm, t_fx_data *fx)
{
	menu->slider_index = 0;
	inisld(md, "Move speed", get_v4f(200, 800, 4000, 100), &pm->plr_speed);
	inisld(md, "Rotation speed", get_v4f(.01, MOUSESPD, 1, 10), &pm->rot_speed);
	inisld(md, "Camera Height", get_v4f(0, 0, md->t_len, 100), &pm->height);
	inisld(md, "Camera bob", get_v4f(0, BOB_AMOUNT, 1, 100), &pm->bob_amount);
	inisld(md, "fov", get_v4f(0, 60, 600, 100), &pm->fov);
	inisld(md, "fov floor", get_v4f(.5, 1, 1.5, 100), &pm->floor_fov);
	inisld(md, "floor glide", get_v4f(0, .5, 3, 100), &pm->floor_glide);
	inisld(md, "Grass width", get_v4f(.1, .3, 2, 100), &pm->grass_w);
	inisld(md, "Grass speed", get_v4f(0, .2, 3, 100), &pm->fe_speed);
	inisld(md, "text size", get_v4f(10, pm->txt_sc, 30, 20), &pm->txt_sc);
	inisld(md, "ray depth", \
		get_v4f(0, pm->ray_depth, pm->ray_depth * 2, 100), &pm->ray_depth);
	inisld(md, "Fog", get_v4f(0, .4, 3, 100), &md->fx.fog);
	inisld(md, "noise", get_v4f(0, 0, 1, 100), &fx->noise);
	inisld(md, "scanlines fx", get_v4f(0, 0, 1, 100), &fx->scanlines);
	inisld(md, "dithering fx", get_v4f(0, 0, 1, 100), &fx->dithering);
	inisld(md, "barrel fx", get_v4f(0, 0, 5, 100), &fx->barrel_amount);
	inisld(md, "Color band", get_v4f(0, 0, 1, 100), &fx->color_band);
	inisld(md, "Glow fx", get_v4f(0, 0, 1, 100), &fx->bloom_threshold);
	inisld(md, "r", get_v4f(0, 1, 2, 100), &fx->hue.r);
	inisld(md, "g", get_v4f(0, 1, 2, 100), &fx->hue.g);
	inisld(md, "b", get_v4f(0, 1, 2, 100), &fx->hue.b);
	menu->sliders[menu->slider_index].active = 0;
}

static void	inibut(t_button *but, int *value, char *label, unsigned int map_c)
{
	char	*xtr_lbl;

	but->key_show[0] = '\0';
	but->key_trigger = map_c;
	xtr_lbl = ft_strchr(label, '_');
	if (xtr_lbl)
	{
		ft_strlcpy(but->key_show, label, \
			ft_strlen(label) - ft_strlen(xtr_lbl) + 1);
		ft_strlcpy(but->label, xtr_lbl + 1, 50);
	}
	else
		ft_strlcpy(but->label, label, 50);
	but->value = value;
	but->active = 1;
}

static void	init_buttons(t_md *md, t_menu *menu, t_parameters *prm)
{
	int	i;

	i = 0;
	inibut(&menu->buttons[i++], &prm->debug_mode, "1_debug mode", NUM_1_KEY);
	inibut(&menu->buttons[i++], &prm->view_2d, "2_Show Grid", NUM_2_KEY);
	inibut(&menu->buttons[i++], &prm->show_rays, "3_show rays", NUM_3_KEY);
	inibut(&menu->buttons[i++], &prm->use_ceiling, "4_Show sky", NUM_4_KEY);
	inibut(&menu->buttons[i++], &prm->use_floor, "B_Show floor", NUM_B_KEY);
	inibut(&menu->buttons[i++], &prm->ent_mode, "E_Show sprites", NUM_E_KEY);
	inibut(&menu->buttons[i++], &prm->use_grass, "G_Show grass", NUM_G_KEY);
	inibut(&menu->buttons[i++], &prm->fly_cam, "F_Fly cam", NUM_F_KEY);
	inibut(&menu->buttons[i++], &prm->use_thrd, "T_Use Threads", NUM_T_KEY);
	inibut(&menu->buttons[i++], &prm->au_on, "U_audio", NUM_U_KEY);
	inibut(&menu->buttons[i++], &md->mmap.cmps, "N_Show compass", NUM_N_KEY);
	inibut(&menu->buttons[i++], &md->mmap.active, "M_Show minmap", NUM_M_KEY);
	inibut(&menu->buttons[i++], &md->mouse.lock_rot.x, "X_x_mouse", NUM_X_KEY);
	inibut(&menu->buttons[i++], &md->mouse.lock_rot.y, "Y_y_mouse", NUM_Y_KEY);
	inibut(&menu->buttons[i++], &md->fx.anti_alias, "aliasing", -1);
	inibut(&menu->buttons[i++], &menu->active, "ESC_back", -1);
	menu->buttons[i].active = 0;
}

void	init_menu_elements(t_md *md, t_menu *menu)
{
	md->menu.slider_hov = -1;
	md->menu.button_hov = -1;
	menu->selected_slider = NULL;
	menu->slider_bgr_clr = _BLACK;
	menu->slider_fill_clr = _RED;
	init_sliders(md, menu, &md->prm, &md->fx);
	init_buttons(md, menu, &md->prm);
}
