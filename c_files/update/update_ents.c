/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_ents.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:57:44 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/23 20:37:40 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	reset_mapped_end(t_md *md, t_ent *e)
{
	if (e->map_index > 0 && e->map_index <= md->map.len - 1 && \
		md->mapped_ents[e->map_index] == e)
		md->mapped_ents[e->map_index] = NULL;
}

void	update_ent_frame(t_ent *e)
{
	if (e->was_hit == 1)
	{
		if (e->hp <= 0)
			e->frame_index = 0;
		e->was_hit++;
	}
	else
		e->was_hit = 0;
	if (!e->anim[e->action][e->frame_index])
		e->frame_index = 0;
	else
		e->frame_index++;
	if (!e->anim[e->action][e->frame_index])
	{
		e->frame_index = 0;
		if (e->action == m_death)
		{
			e->is_active = 0;
			return ;
		}
	}
	e->frame = e->anim[e->action][e->frame_index];
	e->size = e->frame->size;
}

static int	update_ent(t_md *md, t_ent *e)
{
	e->row_draw_index = 0;
	e->tex_accumulator = 0;
	if (e->type == nt_wall)
		return (0);
	if (md->update_frames && e->is_active && e->type == nt_mob)
		update_ent_frame(e);
	if (e->type != nt_mob || !e->is_active)
		return (1);
	if (e->hp > 0)
		update_mob_actions(md, e);
	else
		e->action = m_death;
	e->in_screen = 0;
	return (1);
}

int	update_ents(t_md *md)
{
	t_ent	*e;
	t_dblst	*node;
	t_dblst	*next;
	int		upd_render;

	if (!md->prm.ent_mode)
		return (1);
	upd_render = 0;
	node = md->entities;
	while (node)
	{
		next = node->next;
		e = (t_ent *)node->content;
		upd_render += update_ent(md, e);
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
