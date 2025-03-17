/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:45:36 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 06:51:02 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	update_cam_ofst(t_md *md)
{
	t_vec3f	dspl;
	float	wrd_spd;

	dspl = get_v3f(\
		md->plr.pos.x - md->plr.size.x / 2 - md->win_size.x / 2 + md->t_len / 2, \
		md->plr.pos.y - md->plr.size.y / 2 - md->win_size.y / 2 + md->t_len / 2, \
		md->plr.pos.z);
	md->cam_ofst = dspl;
	wrd_spd = 20;
	md->wrd_mv_offst.x += md->plr_wrd_mv.x * wrd_spd;
	md->wrd_mv_offst.y += md->plr_wrd_mv.y * wrd_spd;
}

int	update_and_render(t_md *md)
{
	if (md->menu.active)
		return (update_menu(md, &md->menu));
	update_input(md);
	update_mouse(md);
	update_particles(md);
	update_player(md, &md->plr);
	update_cam_ofst(md);
	update_ents(md);
	render(md);
	reset_mlx_values(md);
	update_time(md, &md->timer);
	return (0);
}
