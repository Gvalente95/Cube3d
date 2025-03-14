/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cube.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:36:33 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/14 03:17:22 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	init_cursor(t_md *md)
{
	md->center = init_img_data(md, get_v2(CROSS_SCALE, CROSS_SCALE), "xpm/utils/center.xpm", -1);
	md->cursor = init_img_data(md, get_v2(30, 30), "xpm/utils/cursor/default.xpm", -1);
	md->curs_dtc = init_img_data(md, get_v2(30, 30), "xpm/utils/cursor/hand_open.xpm", -1);
	md->curs_grb = init_img_data(md, get_v2(30, 30), "xpm/utils/cursor/hand_closed.xpm", -1);
	md->mouse_pressed = 0;
	md->mouse_clicked = 0;
	md->mouse_pos = get_v3f(0, 0, 0);
	md->mouse_world_pos = get_v3f(0, 0, 0);
	md->mouse_prv_pos = get_v3f(0, 0, 0);
	md->mouse_grid_pos = get_v2(0, 0);
	md->mouse_delta = get_v3f(0, 0, 0);
	md->mouse_focus = 0;
	mlx_mouse_hook(md->win, mouse_event_handler, md);
	if (md->is_linux)
	{
		mlx_hook(md->win, 5, ButtonReleaseMask, mouse_release_handler, md);
		mlx_hook(md->win, 6, PointerMotionMask, mouse_motion_handler, md);
		return ;
	}
	mlx_hook(md->win, 5, 0, mouse_release_handler, md);
	mlx_hook(md->win, 6, 0, mouse_motion_handler, md);
}

static void	init_minimap(t_md *md, t_mmap *mmap, int ic_len)
{
	t_vec2		pos;
	int			i;
	int			color_index;
	int			background_color;

	mmap->mray_len = 0;
	mmap->active = 0;
	mmap->size = get_v2(md->map.size.x * ic_len, md->map.size.y * ic_len);
	background_color = md->rgb[get_char_index(md->ents_tp_map[0], '0')];
	mmap->bgrnd = init_img_data(md, get_v2(mmap->size.x, mmap->size.y), NULL, background_color - 10000);
	mmap->plr_icon = init_img_data(md, get_v2(ic_len, ic_len), NULL, md->rgb[RGB_GOLD]);
	i = -1;
	while (md->map.buffer[++i])
	{
		color_index = get_char_index(md->ents_tp_map[0], md->map.buffer[i]);
		if (color_index == -1)
			continue ;
		pos = get_v2(i % (md->map.size.x + 1), i / (md->map.size.x + 1));
		draw_pixels(\
			mmap->bgrnd, \
			get_v2(ic_len * pos.x + 1, ic_len * pos.y + 1), \
			get_v2(ic_len * pos.x + ic_len - 1, ic_len * pos.y + ic_len - 1), \
			md->rgb[color_index]);
	}
}

static void	init_background(t_md *md)
{
	t_vec2	r_pos;
	int		i;
	char	*ground_path;
	int		pow;

	ground_path = ft_strjoin(md->img_dir_path, "/ground.xpm");
	md->screen.floor = init_img_data(md, md->win_size, ground_path, md->floor_color);
	free(ground_path);
	md->screen.sky = init_img_data(md, md->win_size, NULL, md->sky_color);
	i = -1;
	while (++i < STARS_AMOUNT)
	{
		pow = r_range(50, 255);
		r_pos = get_v2(r_range(0, md->win_size.x), r_range(0, md->win_size.y));
		draw_pixels(md->screen.sky, \
			get_v2(r_pos.x, r_pos.y), \
			get_v2(r_pos.x + 1, r_pos.y + 1), \
			vec4_to_color(pow, pow, pow, pow));
	}
}

static void	init_game_params(t_md *md, int start_debug)
{
	md->rgb[RGB_RED] = str_to_color("255,0,0");
	md->rgb[RGB_GREEN] = str_to_color("0,255,0");
	md->rgb[RGB_BLUE] = str_to_color("0,0,255");
	md->rgb[RGB_CYAN] = str_to_color("0,255,255");
	md->rgb[RGB_MAGENT] = str_to_color("255,0,255");
	md->rgb[RGB_GRAY] = str_to_color("169,169,169");
	md->rgb[RGB_BROWN] = str_to_color("139,69,19");
	md->rgb[RGB_TEAL] = str_to_color("0,128,128");
	md->rgb[RGB_LIME] = str_to_color("0,255,0");
	md->rgb[RGB_GOLD] = str_to_color("255,215,0");
	md->rgb[RGB_SILVER] = str_to_color("192,192,192");
	md->rgb[RGB_TURQ] = str_to_color("64,224,208");
	md->rgb[RGB_INDIGO] = str_to_color("75,0,130");
	md->rgb[RGB_VIOLET] = str_to_color("238,130,238");
	md->rgb[RGB_CORAL] = str_to_color("255,128,80");
	md->rgb[RGB_WHITE] = str_to_color("255,255,255");
	md->rgb[RGB_BLACK] = str_to_color("0,0,0");
	md->init_steps = 0;
	md->txt_scale = 10;
	md->input_mov = get_v3f(0, 0, 0);
	md->wrd_mv_offst = get_v3f(0, 0, 0);
	md->debug_mode = start_debug;
	md->ray_mode = !md->debug_mode;
	md->show_rays = md->debug_mode;
	md->mmap.ic_scl = md->win_size.x / 100;
	md->mouse_hide = 1;
	md->plr_wrd_mv = get_v3f(0, 0, 0);
	md->input_mov = get_v3f(0, 0, 0);
	md->wrd_mv_offst = get_v3f(0, 0, 0);
	md->size_2d = 40;
}

int	init_cube(t_md *md, char *file_arg, int start_debug)
{
	md->screen.buffer = init_img_data(md, md->win_size, NULL, -1);
	init_game_params(md, start_debug);
	init_labels(md);
	init_ents_data(md);
	init_map(md, file_arg);
	md->init_steps++;
	init_background(md);
	init_entities(md, get_v2(0, 0));
	init_cursor(md);
	init_minimap(md, &md->mmap, md->mmap.ic_scl);
	md->timer.game_start = get_time_in_seconds();
	md->timer.elapsed_pause = md->timer.game_start;
	if (md->debug_mode)
		show_init_information(md);
	md->init_steps++;
	return (0);
}
