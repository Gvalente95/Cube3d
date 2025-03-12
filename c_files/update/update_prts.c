/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_prts.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 06:11:22 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/12 06:24:37 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	update_prt_ent(t_md *md, t_ent *e)
{
	if (!e->is_active)
		return (0);
	e->mov = get_v3f(e->dir.x, e->dir.y, e->dir.z);
	set_collisions(md, e);
	if (e->col_hit)
		return (0);
	e->pos = get_v3f(e->pos.x + e->mov.x, e->pos.y + e->mov.x, e->pos.y + e->mov.z);
	e->coord_pos = get_v3(e->pos.x / md->t_len, e->pos.y / md->t_len, e->pos.z / md->t_len);
}

static t_ent	*init_prt(t_md *md, t_vec3f dir, t_vec3f start_pos)
{
	t_ent	*e;

	e = malloc(sizeof(t_ent *));
	if (!e)
		free_and_quit(md, "alloc for prt\n", NULL);
	e->frame = md->e_frms[nt_prt][0][0];
	e->frame_index = 0;
	e->dir = dir;
	e->pos = start_pos;
}


void	update_prts(t_md *md)
{
	t_dblst	*node;
	t_ent	*e;

	node = md->particles;
	while (node)
	{
		e = (t_ent *)node->content;
		if (!update_prt_ent(md, e))
			dblst_delone(md->particles, e);
		node = node->next;
	}
}

void	launch_prt(t_md *md, t_ent *emitter, t_vec3f dir)
{
	t_dblst	*new_node;
	t_ent	*new_prt;

	new_prt = init_prt(md, emitter->dir, emitter->pos);
	dblst_add_back(md->particles, new_prt);
}
