/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_2b.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:13:52 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/16 08:59:47 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

t_vec2	v2_center(t_vec2 a, t_vec2 b)
{
	return (get_v2(a.x / 2 - b.x / 2, a.y / 2 - b.y / 2));
}

t_vec2	center_pos(t_vec2 a, t_vec2 a_size, t_vec2 b, t_vec2 b_size)
{
	t_vec2	a_center;
	t_vec2	b_center;

	a_center = v2_center(a, a_size);
	b_center = v2_center(b, b_size);
	return (get_v2(a_center.x / 2 - b_center.x / 2, a_center.y / 2 - b_center.y / 2));
}

int	v2_bounds(t_vec3f a, t_vec3f bnd_pos, t_vec3f bnd_size)
{
	return (a.x >= bnd_pos.x && \
		a.x <= bnd_pos.x + bnd_size.x && \
		a.y >= bnd_pos.y && \
		a.y <= bnd_pos.y + bnd_size.y);
}
