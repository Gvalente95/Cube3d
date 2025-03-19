/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_menu.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:33:17 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 05:30:23 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	render_slider(t_md *md, t_slider *slider, t_image *screen)
{
	t_vec2		txt_pos;

	draw_img(slider->img, screen, slider->pos, -1);
	txt_pos.x = slider->pos.x + md->txt_scale * 1.5;
	txt_pos.y = slider->pos.y + slider->img->size.y / 2 - md->txt_scale / 2;
	rnd_abs_txt(md, get_v4(txt_pos.x, txt_pos.y, -1, md->txt_scale), \
	"%-15s %.1f", slider->label, *slider->value);
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
		if (!menu->selected_slider && \
			v2_bounds(md->mouse_real, slider->pos, slider->img->size))
			menu->selected_slider = slider;
	}
}

int	update_slider(t_md *md, t_slider *sld)
{
	int		x_pos;
	t_vec2	end;
	t_vec2	sz;

	if (md->mouse_pressed != MOUSE_PRESS)
		return (md->menu.selected_slider = NULL, 0);
	sz = sld->img->size;
	x_pos = ((md->mouse_real.x + 40 - sld->pos.x) * 10) / sld->img->size.x;
	if (x_pos == sld->point)
		return (0);
	sld->point = minmax(0, 10, x_pos);
	*sld->value = ((sld->limits.z - sld->limits.x) / 10) * sld->point;
	if (md->fov <= 0)
		md->fov = 1;
	flush_img(sld->img, md->menu.slider_bgr_clr, -1, -1);
	end = get_v2((sz.x / 10) * sld->point, sz.y);
	draw_pixels(sld->img, get_v2(0, 0), end, md->menu.slider_fill_clr);
	render(md);
	return (1);
}

void	render_buttons(t_md *md, t_menu *menu, t_image *screen, int *hover)
{
	t_button	*but;
	t_vec4		txt_data;
	int			i;

	i = -1;
	while (menu->buttons[++i].active)
	{
		but = &menu->buttons[i];
		draw_img(but->img, screen, but->pos, -1);
		txt_data.r = but->pos.x + but->img->size.x / 2 - md->txt_scale * 5;
		txt_data.r -= (ft_strlen(but->label) - 3) * md->txt_scale - 5;
		txt_data.g = but->pos.y + but->img->size.y / 2 - md->txt_scale / 2;
		txt_data.b = -1;
		txt_data.a = md->txt_scale;
		rnd_abs_txt(md, txt_data, "%s %d", but->label, *but->value);
		if (v2_bounds(md->mouse_real, but->pos, but->img->size))
		{
			*hover = 1;
			if (!md->mouse_click)
				continue ;
			*but->value = !(*but->value);
			flush_img(but->img, md->rgb[*but->value > EPSILON], -1, -1);
			render(md);
		}
	}
}

int	update_menu(t_md *md, t_menu *menu)
{
	int		over_button;
	t_image	*freeze_frame;

	freeze_frame = copy_image(md, md->screen);
	menu->freeze_frame = freeze_frame;
	draw_transp_img(menu->overlay, freeze_frame, get_v2(0, 0), 0.95);
	over_button = 0;
	update_menu_input(md, menu);
	if (menu->selected_slider)
		update_slider(md, menu->selected_slider);
	render_sliders(md, menu, freeze_frame);
	render_buttons(md, menu, freeze_frame, &over_button);
	mlx_put_image_to_window(md->mlx, md->win, freeze_frame->img, 0, 0);
	free_image_data(md, freeze_frame);
	render_cursor(md, NULL, menu->selected_slider != NULL || over_button);
	reset_mlx_values(md);
	update_time(md, &md->timer);
	return (1);
}
