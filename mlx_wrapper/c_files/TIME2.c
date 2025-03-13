/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TIME2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 22:42:23 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/12 13:18:45 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

void	stop_timer(t_timer *timer)
{
	timer->menu_timer = get_time_in_seconds();
}

void	resume_timer(t_timer *timer)
{
	double	elapsed_pause;

	elapsed_pause = get_time_in_seconds() - timer->menu_timer;
	timer->elapsed_pause += elapsed_pause;
	timer->menu_timer = 0;
}

double	get_total_time(t_timer *timer)
{
	return (get_time_in_seconds() - timer->game_start - timer->elapsed_pause);
}

void	handle_timers(t_md *md, t_timer *tm)
{
	tm->is_game_running = md->gst.state == in_game;
}
