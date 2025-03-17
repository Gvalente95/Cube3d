/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_projectile.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:56:16 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 00:36:32 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	plr_shoot(t_md *md)
{
	md->plr.shot_timer = get_time_in_seconds();
	md->plr.shot = 1;
	md->plr.can_shoot = 0;
}
