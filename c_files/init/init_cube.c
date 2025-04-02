/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cube.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:36:33 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/02 14:16:01 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	init_cursor(t_md *md)
{
	t_mouse	*msd;
	t_vec2	cursor_sz;

	msd = &md->mouse;
	cursor_sz = get_v2(30, 30);
	msd->cursor = init_img(md, cursor_sz, "utils/cursor/default.xpm", -1);
	msd->curs_dtc = init_img(md, cursor_sz, "utils/cursor/hand_open.xpm", -1);
	msd->curs_grb = init_img(md, cursor_sz, "utils/cursor/hand_closed.xpm", -1);
	msd->pressed = 0;
	msd->click = 0;
	msd->pos = get_v3f(0, 0, 0);
	msd->world = get_v2(0, 0);
	msd->real = get_v2(0, 0);
	md->input_offst = get_v2(0, 0);
	msd->prev = get_v2(0, 0);
	msd->grid_pos = get_v2(0, 0);
	msd->delta = get_v2(0, 0);
	msd->focus = 0;
	if (!LIN)
		mlx_mouse_hide(md->mlx, md->win);
	mlx_mouse_hook(md->win, mouse_event_handler, md);
	mlx_hook(md->win, 5, ButtonReleaseMask, mouse_release_handler, md);
	mlx_hook(md->win, 6, PointerMotionMask, mouse_motion_handler, md);
}

static void	init_minimap(t_md *md, t_mmap *mmap, int ic_len)
{
	t_vec2		pos;
	int			i;
	char		c;

	mmap->cmps = 1;
	mmap->mray_len = 0;
	mmap->active = 1;
	mmap->size = get_v2(md->map.size.x * ic_len, md->map.size.y * ic_len);
	mmap->img = init_img(md, mmap->size, NULL, md->rgb[RGB_NULL]);
	mmap->bg = init_img(md, mmap->size, NULL, md->rgb[RGB_NULL]);
	i = -1;
	while (md->map.buffer[++i])
	{
		c = md->map.buffer[i];
		if (c == ' ')
			continue ;
		pos = get_v2(i % (md->map.size.x + 1), i / (md->map.size.x + 1));
		draw_pixels(mmap->bg, get_v2(ic_len * pos.x, ic_len * pos.y), \
			v2(ic_len), md->rgb[RGB_YELLOW]);
		if (c != '1')
			continue ;
		draw_pixels(mmap->bg, get_v2(ic_len * pos.x - 1, ic_len * pos.y - 1), \
		v2(ic_len - 1), md->rgb[RGB_BLACK]);
	}
}

static void	init_colors(t_md *md)
{
	md->rgb[RGB_RED] = v4_to_color(255, 0, 0, 255);
	md->rgb[RGB_GREEN] = v4_to_color(0, 255, 0, 255);
	md->rgb[RGB_BLUE] = v4_to_color(0, 0, 255, 255);
	md->rgb[RGB_CYAN] = v4_to_color(0, 255, 255, 255);
	md->rgb[RGB_MAGENT] = v4_to_color(255, 0, 255, 255);
	md->rgb[RGB_GRAY] = v4_to_color(169, 169, 169, 255);
	md->rgb[RGB_BROWN] = v4_to_color(139, 69, 19, 255);
	md->rgb[RGB_TEAL] = v4_to_color(0, 128, 128, 255);
	md->rgb[RGB_LIME] = v4_to_color(0, 255, 0, 255);
	md->rgb[RGB_GOLD] = v4_to_color(255, 215, 0, 255);
	md->rgb[RGB_SILVER] = v4_to_color(192, 192, 192, 255);
	md->rgb[RGB_TURQ] = v4_to_color(64, 224, 208, 255);
	md->rgb[RGB_INDIGO] = v4_to_color(75, 0, 130, 255);
	md->rgb[RGB_VIOLET] = v4_to_color(238, 130, 238, 255);
	md->rgb[RGB_CORAL] = v4_to_color(255, 128, 80, 255);
	md->rgb[RGB_WHITE] = v4_to_color(255, 255, 255, 255);
	md->rgb[RGB_BLACK] = v4_to_color(0, 0, 0, 255);
	md->rgb[RGB_YELLOW] = v4_to_color(255, 255, 0, 255);
	md->rgb[RGB_ORANGE] = v4_to_color(255, 165, 0, 255);
	md->rgb[RGB_NULL] = v4_to_color(0, 0, 0, 0);
}

static void	init_game_params(t_md *md, t_parameters *prm, int start_debug)
{
	md->mouse.lock_rot = get_v2(0, 0);
	md->score = 0;
	md->txd.size_2d = 40;
	md->plr.was_hit = 0;
	md->bob_time = 0.0f;
	md->portal.ends[0].e = NULL;
	md->portal.ends[1].e = NULL;
	md->portal.last_shot_index = 0;
	md->portal.last_passage = NULL;
	prm->debug_mode = start_debug;
	prm->ray_mode = !prm->debug_mode;
	prm->show_rays = prm->debug_mode;
	prm->ray_depth = md->t_len * RAY_DEPTH;
	prm->free_cam = 0;
	prm->height = HEIGHT;
	prm->plr_speed = PLRSPD;
	prm->rot_speed = MOUSESPD;
	prm->resolution = RESOLUTION;
	prm->zoom = md->t_len / 2;
	prm->ent_mode = 0;
	prm->use_thrd = 1;
	prm->use_grass = 1;
	prm->au_amb_on = 1;
	prm->use_sky = 1;
	prm->au_on = 1;
}

int	init_cube(t_md *md, char *file_arg, int start_debug)
{
	init_colors(md);
	init_game_params(md, &md->prm, start_debug);
	init_ents_data(md, &md->txd);
	init_map(md, file_arg);
	md->init_steps++;
	init_hud(md, &md->hud);
	md->mapped_ents = ft_calloc(md->map.len + 1, sizeof(t_ent *));
	init_entities(md, get_v2(0, 0));
	init_cursor(md);
	//md->mmap.ic_scl = md->win_sz.x / 125;
	md->mmap.ic_scl = 5;
	md->mmap.collaps_scl = md->win_sz.x / 150;
	init_minimap(md, &md->mmap, md->mmap.ic_scl);
	init_menu(md, &md->menu);
	init_env(md);
	md->timer.game_start = get_time_in_seconds();
	md->timer.elapsed_pause = md->timer.game_start;
	md->init_steps++;
	show_init_information(md);
	init_ray_threads(md);
	return (0);
}
