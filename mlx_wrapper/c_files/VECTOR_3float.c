/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VECTOR_3float.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:54:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 11:50:00 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

t_vec3	get_grid_posf(t_md *md, t_vec3f pos)
{
	return ((t_vec3){(int)(pos.x / md->t_len) * md->t_len, \
		(int)(pos.y / md->t_len) * md->t_len, 0});
}

t_vec3f	v3_to_v3f(t_vec3 vec)
{
	return (get_v3f((float)vec.x, (float)vec.y, 0));
}

t_vec3f	get_v3f(float x, float y, float z)
{
	t_vec3f	ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;
	return (ret);
}

int	cmp_vec3f(t_vec3f a, t_vec3f b)
{
	return (
		fabsf(a.x - b.x) < EPSILON && \
		fabsf(a.y - b.y) < EPSILON && \
		fabsf(a.z - b.z) < EPSILON
	);
}

int	print_vec3f(t_vec3f a, const char *label)
{
	if (label)
		printf("%s: ", label);
	printf("x%.3f y%.3f %.3fz\n", a.x, a.y, a.z);
	return (1);
}
