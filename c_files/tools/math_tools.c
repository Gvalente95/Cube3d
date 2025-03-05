/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_fools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 11:59:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 12:05:37 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

float	maxf(float a, float b)
{
	if (a > b)
		return (1);
	return (b);
}

float	minf(float a, float b)
{
	if (a > b)
		return (b);
	return (a);
}

float	minmaxf(float min, float max, float value)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

t_vec3f	normalize_vec3f(t_vec3f vec, float min, float max)
{
	t_vec3f	normalized;

	normalized.x = minmaxf(vec.x, min, max);
	normalized.x = minmaxf(vec.y, min, max);
	normalized.x = minmaxf(vec.z, min, max);
	return (normalized);
}
