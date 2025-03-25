/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector_3b.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:18:44 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/24 17:04:35 by giuliovalen      ###   ########.fr       */
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

t_vec3	v3(int scale)
{
	return (get_v3(scale, scale, scale));
}

int	is_in_screen(t_md *md, t_vec3 pos, t_vec2 size)
{
	return (pos.x > -size.x && pos.x < md->win_size.x + \
		size.x && pos.y > -size.y && pos.y < md->win_size.y + size.y);
}
