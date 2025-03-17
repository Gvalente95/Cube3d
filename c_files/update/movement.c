/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:44:34 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/15 20:07:56 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	move_player(t_md *md, t_ent *e)
{
	t_vec3f	sign_mov;
	t_vec3f	decc_mov;

	sign_mov = get_v3f(e->mov.x * PLRSPD, e->mov.y * PLRSPD, e->mov.z * PLRSPD);
	decc_mov = get_v3f(e->mov.x * ACCSPD, e->mov.y * ACCSPD, e->mov.z * ACCSPD);
	if (e->pos.z + sign_mov.z > 0)
	{
		e->pos.z = 0;
		decc_mov.z = 0;
		sign_mov.z = 0;
		e->mov.z = 0;
	}
	e->pos = get_v3f(\
		e->pos.x + sign_mov.x, \
		e->pos.y + sign_mov.y, \
		e->pos.z + sign_mov.z);
	e->mov = get_v3f(\
		e->mov.x - decc_mov.x, \
		e->mov.y - decc_mov.y, \
		e->mov.z);
	e->coord_pos = get_v3(\
		e->pos.x / md->t_len, \
		e->pos.y / md->t_len, \
		e->pos.z / md->t_len);
	update_map_index(md, e);
	return (1);
}

t_vec3f	get_dir_towards(t_ent *from, t_ent *to, float speed)
{
	t_vec3f	movement_towards;
	float	angle_towards;

	angle_towards = atan2f(to->pos.y - from->pos.y, to->pos.x - from->pos.x);

	movement_towards.x = cosf(angle_towards) * speed;
	movement_towards.y = sinf(angle_towards) * speed;
	movement_towards.z = 0;
	return (movement_towards);
}

int	move_ent(t_md *md, t_ent *e)
{
	e->pos.x += (e->dir.x * PARTICLE_SPEED) * md->timer.delta_time;
	e->pos.y += e->dir.y * PARTICLE_SPEED * md->timer.delta_time;
	e->pos.z += e->dir.z * PARTICLE_SPEED * md->timer.delta_time;
	e->coord_pos.x = e->pos.x / md->t_len;
	e->coord_pos.y = e->pos.y / md->t_len;
	update_map_index(md, e);
	return (1);
}

int	update_map_index(t_md *md, t_ent *e)
{
	int		new_index;
	char	cur_char;

	new_index = e->coord_pos.x + ((md->map.size.x + 1) * e->coord_pos.y);
	if (new_index == e->map_index)
		return (0);
	if (e->map_index < 0 || e->map_index >= md->map.len)
		return (0);
	if (md->mapped_ents[e->map_index] == e)
		md->mapped_ents[e->map_index] = NULL;
	cur_char = md->map.buffer[e->map_index];
	if (cur_char == '0' || cur_char == e->character)
		md->map.buffer[e->map_index] = '0';
	cur_char = md->map.buffer[new_index];
	if (char_in_str(cur_char, "\n1"))
		return (0);
	md->map.buffer[new_index] = e->character;
	e->map_index = new_index;
	md->mapped_ents[e->map_index] = e;
	return (1);
}
