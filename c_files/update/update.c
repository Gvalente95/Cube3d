/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:45:36 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/18 16:52:25 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	update_camera_offset(t_md *md)
{
	t_vec3f	dspl;
	t_vec2	win_size;
	t_vec2	plr_size;
	t_vec3f	plr_pos;

	win_size = md->win_size;
	plr_size = md->plr.size;
	plr_pos = md->plr.pos;
	dspl.x = plr_pos.x - plr_size.x / 2 - win_size.x / 2 + md->t_len / 2;
	dspl.y = plr_pos.y - plr_size.y / 2 - win_size.y / 2 + md->t_len / 2;
	dspl.z = plr_pos.z;
	md->cam_ofst = dspl;
	md->wrd_mv_offst.x += md->plr_wrd_mv.x * 20;
	md->wrd_mv_offst.y += md->plr_wrd_mv.y * 20;
}

int	update_and_render(t_md *md)
{
	if (md->menu.active)
		return (update_menu(md, &md->menu));
	update_input(md);
	update_mouse(md);
	update_player(md, &md->plr);
	update_camera_offset(md);
	update_ents(md);
	render(md);
	reset_mlx_values(md);
	update_time(md, &md->timer);
	return (0);
}
