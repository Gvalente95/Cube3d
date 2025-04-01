/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_math_f.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:55:28 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/31 15:12:19 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	v4_to_color(int r, int g, int b, int a)
{
	t_vec4		rgba;

	a = 255 - a;
	rgba = get_v4(r, g, b, a);
	return ((rgba.a << 24) | (rgba.r << 16) | (rgba.g << 8) | rgba.b);
}

t_vec4	color_to_v4(int color)
{
	t_vec4	clr;

	clr.a = (color >> 24) & 0xFF;
	clr.r = (color >> 16) & 0xFF;
	clr.g = (color >> 8) & 0xFF;
	clr.b = color & 0xFF;
	return (clr);
}

