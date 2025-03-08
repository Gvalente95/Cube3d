/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:44:34 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/07 13:20:45 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	move_ent(t_ent *e)
{
	t_vec3f	sign_mov;
	t_vec3f	decc_mov;

	sign_mov = get_v3f(e->mov.x * PLRSPD, e->mov.y * PLRSPD, e->mov.z * PLRSPD);
	decc_mov = get_v3f(e->mov.x * ACCSPD, e->mov.y * ACCSPD, e->mov.z * ACCSPD);
	e->pos = get_v3f(\
		e->pos.x + sign_mov.x, \
		e->pos.y + sign_mov.y, \
		e->pos.z + sign_mov.z);
	e->mov = get_v3f(\
		e->mov.x - decc_mov.x, \
		e->mov.y - decc_mov.y, \
		e->mov.z - decc_mov.z);
	return (1);
}
