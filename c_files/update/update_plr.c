/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_plr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:43:58 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/08 13:06:10 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	update_player_mov(t_md *md)
{
	t_vec3f	start_mov;
	float	spd;

	spd = ACCSPD * (1 + md->key_prs[SHIFT_KEY]);
	start_mov = get_v3f(md->plr.mov.x, md->plr.mov.y, md->plr.mov.z);
	md->plr.mov.y -= (md->key_prs[W_KEY] || md->key_prs[UP_KEY]) * spd;
	md->plr.mov.y += (md->key_prs[S_KEY] || md->key_prs[DOWN_KEY]) * spd;
	md->plr.mov.x += (md->key_prs[D_KEY] || md->key_prs[RIGHT_KEY]) * spd;
	md->plr.mov.x -= (md->key_prs[A_KEY] || md->key_prs[LEFT_KEY]) * spd;
	return (!cmp_vec3f(start_mov, md->plr.mov, EPSILON));
}

int	update_player(t_md *md)
{
	int		has_moved;

	md->plr.rot.x = md->mouse_pos.x - md->win_size.x / 2;
	//md->plr.rot.y = md->mouse_pos.y - md->win_size.y / 2;
	has_moved = update_player_mov(md);
	set_collisions(md, &md->plr);
	move_ent(&md->plr);
	md->plr.coord_pos = get_v3(md->plr.pos.x / md->t_len, md->plr.pos.y / md->t_len, md->plr.pos.z / md->t_len);
	return (has_moved);
	return (0);
}
