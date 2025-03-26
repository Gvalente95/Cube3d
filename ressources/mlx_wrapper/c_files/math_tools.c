/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 11:59:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/26 16:41:26 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

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

float fast_inverse_sqrt(float number)
{
	long		i;
	float		x2;
	float		y;
	float		threehalfs;

	threehalfs = 1.5f;
	x2 = number * 0.5f;
	y = number;
	i = * ( long * ) &y;
	i = 0x5f3759df - ( i >> 1 );
	y = * ( float * ) &i;
	y = y * ( threehalfs - ( x2 * y * y ) );
	return (y);
}
