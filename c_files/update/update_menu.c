/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_menu.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:33:17 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 06:55:54 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	update_slider(t_md *md, t_slider *slider)
{
	int		x_pos;
	t_vec2	end;
	t_vec2	siz;

	if (md->mouse_pressed != MOUSE_PRESS)
		return (md->menu.selected_slider = NULL, 0);
	siz = slider->img->size;
	x_pos = (md->mouse_real.x - md->win_size.x / 2 + siz.x / 2 + 40) / (siz.x / 10);
	if (x_pos == slider->point)
		return (0);
	slider->point = minmax(0, 10, x_pos);
	*slider->value = ((slider->limits.z - slider->limits.x) / 10) * slider->point;
	if (md->fov <= 0)
		md->fov = 1;
	flush_img(slider->img, md->menu.slider_bgr_clr, -1, -1);
	end = get_v2((siz.x / 10) * slider->point, siz.y);
	draw_pixels(slider->img, get_v2(0, 0), end, md->menu.slider_fill_clr);
	render(md);
	return (1);
}

static int	update_keys(t_md *md, t_menu *menu)
{
	if (md->key_clicked == ESC_KEY)
	{
		mlx_mouse_move(md->win, md->prev_mouse.x, md->prev_mouse.y);
		md->menu.active = 0;
		return (0);
	}
	if (menu->selected_slider)
		return (update_slider(md, menu->selected_slider));
	return (1);
}

void	render_sliders(t_md *md, t_menu *menu, t_image *screen)
{
	t_slider	*slider;
	t_vec2		txt_pos;
	int			i;

	i = -1;
	while (menu->sliders[++i].active)
	{
		slider = &menu->sliders[i];
		draw_img(slider->img, screen, slider->pos, -1);
		txt_pos.x = slider->pos.x + slider->img->size.x / 2 - md->txt_scale * 5;
		txt_pos.y = slider->pos.y + slider->img->size.y / 2 - md->txt_scale / 2;
		rnd_abs_txt(md, get_v4(txt_pos.x, txt_pos.y, -1, md->txt_scale), \
		"%s[%d] > %.1f", slider->label, slider->point, *slider->value);
		if (v2_bounds(md->mouse_real, slider->pos, slider->img->size))
			menu->selected_slider = slider;
	}
}

int	update_menu(t_md *md, t_menu *menu)
{
	update_keys(md, menu);
	flush_img(menu->overlay, menu->bgr_color, -1, 0);
	draw_transp_img(md->screen, menu->overlay, get_v2(0, 0), 0.05);
	render_sliders(md, menu, menu->overlay);
	render_cursor(md, menu->overlay, menu->selected_slider != NULL);
	mlx_put_image_to_window(md->mlx, md->win, menu->overlay->img, 0, 0);
	reset_mlx_values(md);
	update_time(md, &md->timer);
	return (1);
}
