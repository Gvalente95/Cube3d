/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 22:42:23 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/03 10:51:09 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

void	resume_timer(t_timer *timer)
{
	double	elapsed_pause;

	elapsed_pause = get_time_in_seconds() - timer->tm_menu;
	timer->elapsed_pause += elapsed_pause;
	timer->tm_menu = 0;
}

double	get_total_time(t_timer *timer)
{
	return (get_time_in_seconds() - timer->game_start - timer->elapsed_pause);
}

void	upd_timer(double *timer, double cur_tm, double incr, int *event)
{
	*event = 0;
	if (*timer < cur_tm)
	{
		*timer = cur_tm + incr;
		*event = 1;
	}
}

pid_t	play_index(t_md *md, const char *filepath, int index)
{
	pid_t	pid;
	char	*index_txt;
	char	*path_with_index;
	char	*full_path;

	if (!md->prm.au_on)
		return (0);
	index_txt = ft_itoa(index);
	path_with_index = ft_strjoin(index_txt, ".mp3");
	full_path = ft_strjoin(filepath, path_with_index);
	pid = play_sound(md, full_path);
	free(full_path);
	free(path_with_index);
	free(index_txt);
	return (pid);
}
