/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cube.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:36:33 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 13:46:17 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	init_cursor(t_md *md)
{
	t_vec2	cursor_sz;

	cursor_sz = get_v2(30, 30);
	md->center = init_img(md, get_v2(10, 10), "utils/center.xpm", md->rgb[RGB_RED]);
	md->cursor = init_img(md, cursor_sz, "utils/cursor/default.xpm", -1);
	md->curs_dtc = init_img(md, cursor_sz, "utils/cursor/hand_open.xpm", -1);
	md->curs_grb = init_img(md, cursor_sz, "utils/cursor/hand_closed.xpm", -1);
	md->mouse_pressed = 0;
	md->mouse_clicked = 0;
	md->mouse_pos = get_v3f(0, 0, 0);
	md->mouse_world_pos = get_v2(0, 0);
	md->mouse_real = get_v2(0, 0);
	md->input_offst = get_v2(0, 0);
	md->prev_mouse = get_v2(0, 0);
	md->mouse_grid_pos = get_v2(0, 0);
	md->mouse_delta = get_v2(0, 0);
	md->mouse_focus = 0;
	//if (!md->is_linux)
	mlx_mouse_hide();
	mlx_mouse_hook(md->win, mouse_event_handler, md);
	mlx_hook(md->win, 5, ButtonReleaseMask, mouse_release_handler, md);
	mlx_hook(md->win, 6, PointerMotionMask, mouse_motion_handler, md);
}

static void	init_minimap(t_md *md, t_mmap *mmap, int ic_len)
{
	t_vec2		pos;
	int			i;
	int			color_index;

	mmap->mray_len = 0;
	mmap->active = 0;
	mmap->size = get_v2(md->map.size.x * ic_len, md->map.size.y * ic_len);
	mmap->ic_scl = ic_len;
	mmap->img = init_img(md, mmap->size, NULL, -1);
	mmap->bg = init_img(md, mmap->size, NULL, md->rgb[RGB_INDIGO]);
	mmap->limits_x = get_v2(0, 0);
	mmap->limits_x = get_v2(0, 0);
	i = -1;
	while (md->map.buffer[++i])
	{
		if (md->map.buffer[i] != '1')
			continue ;
		color_index = get_char_index(md->ents_tp_map[0], md->map.buffer[i]);
		if (color_index == -1)
			continue ;
		pos = get_v2(i % (md->map.size.x + 1), i / (md->map.size.x + 1));
		draw_pixels(mmap->bg, get_v2(ic_len * pos.x + 2, ic_len * pos.y + 2), \
			get_v2(ic_len - 2, ic_len - 2), \
			md->rgb[color_index]);
	}
}

static void	init_colors(t_md *md)
{
	md->rgb[RGB_RED] = vec4_to_color(255, 0, 0, 0);
	md->rgb[RGB_GREEN] = vec4_to_color(0, 255, 0, 0);
	md->rgb[RGB_BLUE] = vec4_to_color(0, 0, 255, 0);
	md->rgb[RGB_CYAN] = vec4_to_color(0, 255, 255, 0);
	md->rgb[RGB_MAGENT] = vec4_to_color(255, 0, 255, 0);
	md->rgb[RGB_GRAY] = vec4_to_color(169, 169, 169, 0);
	md->rgb[RGB_BROWN] = vec4_to_color(139, 69, 19, 0);
	md->rgb[RGB_TEAL] = vec4_to_color(0, 128, 128, 0);
	md->rgb[RGB_LIME] = vec4_to_color(0, 255, 0, 0);
	md->rgb[RGB_GOLD] = vec4_to_color(255, 215, 0, 0);
	md->rgb[RGB_SILVER] = vec4_to_color(192, 192, 192, 0);
	md->rgb[RGB_TURQ] = vec4_to_color(64, 224, 208, 0);
	md->rgb[RGB_INDIGO] = vec4_to_color(75, 0, 130, 0);
	md->rgb[RGB_VIOLET] = vec4_to_color(238, 130, 238, 0);
	md->rgb[RGB_CORAL] = vec4_to_color(255, 128, 80, 0);
	md->rgb[RGB_WHITE] = vec4_to_color(255, 255, 255, 0);
	md->rgb[RGB_BLACK] = vec4_to_color(0, 0, 0, 0);
	md->rgb[RGB_YELLOW] = vec4_to_color(255, 255, 0, 0);
	md->rgb[RGB_ORANGE] = vec4_to_color(255, 165, 0, 0);
}

static void	init_game_params(t_md *md, int start_debug)
{
	init_colors(md);
	md->debug_mode = start_debug;
	md->ray_mode = !md->debug_mode;
	md->show_rays = md->debug_mode;
	md->ray_depth = md->t_len * md->win_size.x;
	md->mmap.ic_scl = md->win_size.x / 100;
	md->lock_rotation = get_v2(1, 0);
	md->size_2d = 40;
}

int	init_cube(t_md *md, char *file_arg, int start_debug)
{
	init_game_params(md, start_debug);
	init_labels(md);
	init_ents_data(md);
	init_map(md, file_arg);
	md->init_steps++;
	init_hud(md);
	md->mapped_ents = ft_calloc(md->map.len + 1, sizeof(t_ent *));
	init_entities(md, get_v2(0, 0));
	init_cursor(md);
	init_minimap(md, &md->mmap, md->mmap.ic_scl);
	init_menu(md, &md->menu);
	md->timer.game_start = get_time_in_seconds();
	md->timer.elapsed_pause = md->timer.game_start;
	md->init_steps++;
	if (md->debug_mode)
		show_init_information(md);
	return (0);
}
