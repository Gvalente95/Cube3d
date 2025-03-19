/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 11:59:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 14:28:04 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

float	maxf(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

float	minf(float a, float b)
{
	if (a < b)
		return (a);
	return (b);
}

float	minmaxf(float min, float max, float value)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

int	minmax(int min, int max, int value)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

int	ft_sign(float a)
{
	if (a > 0)
		return (1);
	if (a < 0)
		return (-1);
	return (0);
}
