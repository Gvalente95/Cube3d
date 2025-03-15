/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector_3b.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:18:44 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/14 23:30:19 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

t_vec3	v3_center(t_vec3 a, t_vec3 b)
{
	return (get_v3(a.x / 2 - b.x / 2, a.y / 2 - b.y / 2, a.z / 2 - b.z / 2));
}

int	v3_bounds(t_vec3 a, t_vec3 pos, t_vec3 size)
{
	return (a.x >= pos.x && \
		a.x <= pos.x + size.x && \
		a.y >= pos.y && \
		a.y <= pos.y + size.y);
}
