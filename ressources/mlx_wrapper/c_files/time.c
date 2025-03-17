/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TIME.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:55:38 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/15 13:18:03 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

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
	timer->menu_timer = get_time_in_seconds();
}

void	init_timer(t_md *md, t_timer *timer)
{
	md->menu.menu_time = 0;
	timer->time = 0;
	timer->fps = 0;
	timer->prv_fps = 0;
	timer->delta_time = 0;
	timer->level_timer = 0;
	timer->shoot_timer = 0;
	timer->menu_timer = 0;
	timer->prev_time = get_time_in_seconds();
	timer->game_start = timer->prev_time;
}

double update_time(t_md *md, t_timer *timer)
{
	timer->current_time = get_time_in_seconds();
	md->timer.fps++;
	if (timer->current_time - md->timer.elapsed_pause >= 1)
	{
		md->timer.elapsed_pause = timer->current_time;
		md->timer.prv_fps = md->timer.fps;
		md->timer.fps = 0;
	}
    timer->delta_time = (timer->current_time - timer->prev_time);
    timer->prev_time = timer->current_time;
	md->timer.time++;
    return (timer->delta_time);
}
