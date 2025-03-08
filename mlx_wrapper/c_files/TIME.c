/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TIME.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:55:38 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/06 23:49:59 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

double	get_time_in_seconds(void)
{
	struct timespec	ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec + (ts.tv_nsec / 1000000000.0));
}

void	init_time_data(t_md *md, t_lvl_data *data)
{
	int	i;

	i = -1;
	while (++i < 3)
	{
		(data && (md->timer.trg[i] = data->trg_timer[i]));
		(!data && (md->timer.trg[i] = md->map.len / 4 + i * 10));
	}
	md->timer.game_start = get_time_in_seconds();
	md->timer.menu_timer = 0;
	md->timer.elapsed_pause = 0;
	md->timer.level_timer = 0;
	md->gst.menu_time = 0;
	md->time = 0;
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
