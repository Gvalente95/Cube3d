/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:55:38 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/04 00:15:14 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

double	get_time_in_seconds(void)
{
	struct timespec	ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec + (ts.tv_nsec / 1000000000.0));
}

double	check_timer(double timer)
{
	return (get_time_in_seconds() - timer);
}

void	start_timer(double *timer)
{
	*timer = get_time_in_seconds();
}

void	stop_timer(t_timer *timer)
{
	timer->tm_menu = get_time_in_seconds();
}

void	init_timer(t_md *md, t_timer *timer)
{
	md->menu.menu_time = 0;
	timer->time = 0;
	timer->fps = 0;
	timer->avrg_fps = 0;
	timer->prv_fps = 0;
	timer->delta_time = 0;
	timer->tm_level = 0;
	timer->shoot_timer = 0;
	timer->tm_menu = 0;
	timer->frm_cnt = 0;
	timer->tm_anim = 0;
	timer->tm_walk = 0;
	timer->no_portal_cooldown = 0;
	timer->prev_time = get_time_in_seconds();
	timer->fps_tm = timer->game_start;
	timer->game_start = timer->prev_time;
}
