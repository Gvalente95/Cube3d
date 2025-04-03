/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_threads_tools.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:30:23 by gvalente          #+#    #+#             */
/*   Updated: 2025/04/03 21:31:27 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	soft_barrier_init(t_soft_barrier *b, int total)
{
	pthread_mutex_init(&b->mutex, NULL);
	pthread_cond_init(&b->cond, NULL);
	b->count = 0;
	b->total = total;
}

void	soft_barrier_wait(t_soft_barrier *b)
{
	pthread_mutex_lock(&b->mutex);
	b->count++;
	if (b->count == b->total)
	{
		b->count = 0;
		pthread_cond_broadcast(&b->cond);
	}
	else
		pthread_cond_wait(&b->cond, &b->mutex);
	pthread_mutex_unlock(&b->mutex);
}

void	soft_barrier_destroy(t_soft_barrier *b)
{
	pthread_mutex_destroy(&b->mutex);
	pthread_cond_destroy(&b->cond);
}
