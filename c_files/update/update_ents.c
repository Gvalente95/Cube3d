/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_ents.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:57:44 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/15 11:14:25 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	reset_mapped_end(t_md *md, t_ent *e)
{
	if (e->map_index > 0 && e->map_index <= md->map.len - 1 && \
		md->mapped_ents[e->map_index] == e)
		md->mapped_ents[e->map_index] = NULL;
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
	e->row_draw_index = 0;
	if (e->type == nt_wall || e->type == nt_empty)
		return (0);
	//update_ent_frame(md, e);
	if (e->type != nt_mob)
		return (1);
	if (e->hp <= 0 || !e->is_active || !e->in_screen)
		return (e->is_active = 0, 0);
	move_ent(md, e);
	update_map_index(md, e);
	return (1);
}

int	update_ents(t_md *md)
{
	t_ent	*e;
	t_dblst	*node;
	t_dblst	*next;
	int		upd_render;

	upd_render = 0;
	node = md->entities;
	while (node)
	{
		next = node->next;
		e = (t_ent *)node->content;
		upd_render += update_ent(md, e);
		if (!e->is_active)
		{
			reset_mapped_end(md, e);
			dblst_delone(node, free);
		}
		if (md->particles == node)
			md->particles = next;
		node = next;
	}
	return (upd_render);
}

int	ent_in_bounds(t_ent *ent, t_ent *bounds)
{
	return (ent->pos.x >= bounds->pos.x + bounds->mov.x && \
		ent->pos.x <= bounds->pos.x + bounds->mov.x + bounds->size.x && \
		ent->pos.y >= bounds->pos.y + bounds->mov.y && \
		ent->pos.y <= bounds->pos.y + bounds->mov.y + bounds->size.y);
}
