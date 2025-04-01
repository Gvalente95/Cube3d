/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:21:44 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/31 15:02:25 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	r_range_seed(unsigned int *g_seed, int min, int max)
{
	if (min > max)
		return (-1);
	*g_seed = (*g_seed * 1103515245 + 12345) & 0x7FFFFFFF;
	return (min + (*g_seed % (max - min + 1)));
}

int	r_range(int min, int max)
{
	if (min > max)
		return (-1);
	return (min + rand() % (max - min + 1));
}

float	f_range(float min, float max)
{
	float	scale;

	if (min > max)
		return (-1.0f);
	scale = rand() / (float)RAND_MAX;
	return (min + scale * (max - min));
}

int	ent_in_bounds(t_ent *ent, t_ent *bounds)
{
	return (ent->pos.x >= bounds->pos.x + bounds->mov.x && \
		ent->pos.x <= bounds->pos.x + bounds->mov.x + bounds->size.x && \
		ent->pos.y >= bounds->pos.y + bounds->mov.y && \
		ent->pos.y <= bounds->pos.y + bounds->mov.y + bounds->size.y);
}
