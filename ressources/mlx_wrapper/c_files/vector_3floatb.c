/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_3floatb.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:16:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 14:03:30 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

t_vec3f	v3f_center(t_vec3f a, t_vec3f b)
{
	return (get_v3f(a.x / 2 - b.x / 2, a.y / 2 - b.y / 2, a.z / 2 - b.z / 2));
}

int	v3f_bounds(t_vec3f a, t_vec3f pos, t_vec3f size)
{
	return (a.x >= pos.x && \
		a.x <= pos.x + size.x && \
		a.y >= pos.y && \
		a.y <= pos.y + size.y);
}
