/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:36:33 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/05 18:24:30 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	init_cursor(t_md *md)
{
	t_mouse			*msd;
	const t_vec2	cursor_sz = v2(30);

	msd = &md->mouse;
	msd->cursor = init_img(md, cursor_sz, "utils/cursor/default.xpm", -1);
	msd->curs_dtc = init_img(md, cursor_sz, "utils/cursor/hand_open.xpm", -1);
	msd->curs_grb = init_img(md, cursor_sz, "utils/cursor/hand_closed.xpm", -1);
	msd->pos = v3f(0);
	msd->world = v2(0);
	msd->real = v2(0);
	md->cam.input_offst = v2(0);
	msd->prev = v2(0);
	msd->grid_pos = v2(0);
	msd->delta = v2(0);
	msd->focus = 0;
	msd->pressed = 0;
	msd->click = 0;
	if (!LIN)
		mlx_mouse_hide(md->mlx, md->win);
	mlx_mouse_hook(md->win, mouse_event_handler, md);
	mlx_hook(md->win, 5, ButtonReleaseMask, mouse_release_handler, md);
	mlx_hook(md->win, 6, PointerMotionMask, mouse_motion_handler, md);
}

static void	init_colors(t_md *md)
{
	md->rgb[RGB_RED] = _RED;
	md->rgb[RGB_GREEN] = _GREEN;
	md->rgb[RGB_BLUE] = _BLUE;
	md->rgb[RGB_CYAN] = _CYAN;
	md->rgb[RGB_MAGENT] = _MAGENT;
	md->rgb[RGB_GRAY] = _GRAY;
	md->rgb[RGB_BROWN] = _BROWN;
	md->rgb[RGB_TEAL] = _TEAL;
	md->rgb[RGB_LIME] = _LIME;
	md->rgb[RGB_GOLD] = _GOLD;
	md->rgb[RGB_SILVER] = _SILVER;
	md->rgb[RGB_TURQ] = _TURQ;
	md->rgb[RGB_INDIGO] = _INDIGO;
	md->rgb[RGB_VIOLET] = _VIOLET;
	md->rgb[RGB_CORAL] = _CORAL;
	md->rgb[RGB_WHITE] = _WHITE;
	md->rgb[RGB_BLACK] = _BLACK;
	md->rgb[RGB_YELLOW] = _YELLOW;
	md->rgb[RGB_ORANGE] = _ORANGE;
	md->rgb[RGB_NULL] = _NULL;
}

static void	init_game_params(t_md *md, t_parameters *prm, int start_debug)
{
	md->portal.ends[0].e = NULL;
	md->portal.ends[1].e = NULL;
	md->portal.found = NULL;
	md->portal.out_pos = v2(90);
	md->mouse.lock_rot = v2(0);
	md->cam.bob_time = 0.0f;
	md->txd.size_2d = 40;
	md->plr.was_hit = 0;
	md->portal.last_shot_index = 0;
	prm->debug_mode = start_debug;
	prm->view_2d = 0;
	prm->show_rays = prm->debug_mode;
	prm->ray_depth = md->t_len * RAY_DEPTH;
	prm->resolution = RESOLUTION;
	prm->fly_cam = 0;
	prm->zoom = md->t_len / 2;
	prm->ent_mode = 0;
	prm->use_thrd = 1;
	prm->use_grass = 0;
	prm->use_floor = 1;
	prm->use_ceiling = 1;
	prm->au_on = !md->is_linux;
}

int	init_cube(t_md *md, char *file_arg, int start_debug)
{
	init_colors(md);
	init_game_params(md, &md->prm, start_debug);
	init_fonts(md);
	init_ents_data(md, &md->txd);
	init_map(md, file_arg);
	md->init_steps++;
	init_hud(md, &md->hud);
	init_entities(md, get_v2(0, 0));
	init_cursor(md);
	init_minimap(md, &md->mmap);
	init_menu(md, &md->menu);
	init_env(md);
	init_thread_pool(md, THREADS_BATCH);
	md->timer.game_start = get_time_in_seconds();
	md->timer.elapsed_pause = md->timer.game_start;
	md->init_steps++;
	show_init_information(md);
	return (0);
}
