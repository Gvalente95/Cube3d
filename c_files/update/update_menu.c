/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_menu.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:33:17 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/24 12:37:30 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	render_slider(t_md *md, t_slider *sldr, t_image *screen)
{
	t_vec4		txt_d;

	draw_img(sldr->img, screen, sldr->pos, -1);
	txt_d.r = sldr->pos.x + 5;
	txt_d.g = sldr->pos.y + sldr->img->size.y / 2 - md->prm.txt_scale / 2;
	txt_d.b = -1;
	txt_d.a = md->prm.txt_scale;
	rnd_abs_txt(md, txt_d, "%s", sldr->label);
	txt_d.r = sldr->pos.x + sldr->img->size.x - md->prm.txt_scale * 5;
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
		render_slider(md, slider, screen);
		if (md->menu.selected_slider)
			continue ;
		if (!v2_bounds(md->mouse.real, slider->pos, slider->img->size))
			continue ;
		menu->hov = 1;
		if (md->mouse.pressed == MOUSE_PRESS)
			menu->selected_slider = slider;
	}
}

int	update_slider(t_md *md, t_slider *sld)
{
	int		x_pos;
	t_vec2	end;
	t_vec2	sz;

	if (!md->mouse.pressed)
		return (md->menu.selected_slider = NULL, 0);
	sz = sld->img->size;
	if (md->key_prs[CTRL_KEY] == 1)
		x_pos = sld->base_point;
	else
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
	render(md);
	md->menu.refresh = 1;
	return (1);
}

void	render_buttons(t_md *md, t_menu *menu, t_mouse *msd, t_image *screen)
{
	t_button	*but;
	t_vec4		txt_data;
	int			i;

	txt_data = get_v4(-1, -1, -1, md->prm.txt_scale);
	i = -1;
	while (menu->buttons[++i].active)
	{
		but = &menu->buttons[i];
		draw_img(but->img, screen, but->pos, -1);
		txt_data.r = but->pos.x + but->img->size.x / 2 - md->prm.txt_scale * 5;
		txt_data.r -= (ft_strlen(but->label) - 3) * md->prm.txt_scale - 5;
		txt_data.g = but->pos.y + but->img->size.y / 2 - md->prm.txt_scale / 2;
		rnd_abs_txt(md, txt_data, "%s %d", but->label, *but->value);
		if (!v2_bounds(msd->real, but->pos, but->img->size))
			continue ;
		menu->hov = 1;
		if (!msd->click)
			continue ;
		*but->value = !(*but->value);
		flush_img(but->img, md->rgb[*but->value > EPSILON], -1, -1);
		printf("%d", *but->value);
		render(md);
		menu->refresh = 1;
	}
}

int	update_menu(t_md *md, t_menu *menu)
{
	menu->hov = 0;
	update_menu_input(md, menu);
	if (menu->refresh)
	{
		if (menu->freeze_frame)
			free_image_data(md, menu->freeze_frame);
		menu->freeze_frame = copy_image(md, md->screen, get_v2(-1, -1), -1);
		apply_vignette(menu->freeze_frame, 0.5, RGB_BLACK);
		menu->refresh = 0;
	}
	render_sliders(md, menu, menu->freeze_frame);
	if (menu->selected_slider)
		update_slider(md, menu->selected_slider);
	render_buttons(md, menu, &md->mouse, menu->freeze_frame);
	md->mlx_put(md->mlx, md->win, menu->freeze_frame->img, 0, 0);
	reset_mlx_values(md);
	return (1);
}
