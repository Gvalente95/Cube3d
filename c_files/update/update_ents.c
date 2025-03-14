/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_ents.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:57:44 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/14 08:00:44 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static t_vec3f	move_toward(t_ent *from, t_ent *to, float speed)
{
	t_vec3f	movement_towards;
	float	angle_towards;

	angle_towards = atan2f(to->pos.y - from->pos.y, to->pos.x - from->pos.x);

	movement_towards.x = cosf(angle_towards) * speed;
    movement_towards.y = sinf(angle_towards) * speed;
    movement_towards.z = 0;
	return (movement_towards);
}

static void	update_ent_movement(t_md *md, t_ent *e)
{
	return ;
	e->mov = move_toward(e, &md->plr, 10);
	//set_collisions(md, e);
	e->pos = get_v3f(e->pos.x + e->mov.x, e->pos.y + e->mov.y, 0);
	e->coord_pos = get_v3(\
		e->pos.x / md->t_len, \
		e->pos.y / md->t_len, \
		e->pos.z / md->t_len);
	update_map_index(md, e);
}

void	update_ent_frame(t_md *md, t_ent *e)
{
	e->frame_index++;
	if (!md->e_frms[e->type][e->action][e->frame_index])
		e->frame_index = 0;
	e->frame = md->e_frms[e->type][e->action][e->frame_index];
}

static int	update_ent(t_md *md, t_ent *e)
{
	update_ent_frame(md, e);
	if (e->type != nt_wall)
		update_ent_movement(md, e);
	return (1);
}

int	update_ents(t_md *md)
{
	t_ent	*e;
	t_dblst	*node;
	int		upd_render;

	upd_render = 0;
	node = md->entities;
	while (node)
	{
		e = (t_ent *)node->content;
		if (!e->is_active || !e->in_screen || e->type == nt_wall || e->type == nt_empty)
		{
			node = node->next;
			continue ;
		}
		upd_render += update_ent(md, e);
		node = node->next;
	}
	return (upd_render);
}
