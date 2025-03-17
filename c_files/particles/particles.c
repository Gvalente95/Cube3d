/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   particles.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 06:11:22 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 13:19:32 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	update_prt_ent(t_md *md, t_ent *e)
{
	e->hp--;
	if (e->hp <= 0 || !e->in_screen)
		e->is_active = 0;
	if (!e->is_active)
		return (0);
	move_ent(md, e);
	if (set_collisions(md, e))
		return (0);
	return (1);
}

void	update_particles(t_md *md)
{
	t_dblst	*node;
	t_dblst	*next;
	t_ent	*e;

	node = dblst_first(md->particles);
	while (node)
	{
		e = (t_ent *)node->content;
		next = node->next;
		if (e->is_active)
			update_prt_ent(md, e);
		else
		{
			// reset_mapped_end(md, e);
			// dblst_delone(node, free);
		}
		node = next;
	}
}

static t_ent	*init_prt(t_md *md, t_ent *emitter, t_vec3f pos, t_vec3f dir)
{
	t_ent	*e;
	t_vec2	coord_pos;
	int		map_index;

	coord_pos = get_v2(pos.x / md->t_len, pos.y / md->t_len);
	map_index = coord_pos.x + ((md->map.size.x + 1) * coord_pos.y);
	e = init_ent(md, md->ents_tp_map[0][nt_prt - 1], coord_pos, map_index);
	e->type = nt_prt;
	e->character = 'P';
	e->frame = md->e_frms[nt_prt][0][0];
	e->size = e->frame->size;
	e->emitter = emitter;
	e->pos = pos;
	e->dir = dir;
	e->hp = 100;
	return (e);
}

void	launch_prt(t_md *md, t_ent *emitter, t_vec3f start_pos, t_vec3f dir)
{
	t_dblst	*new_node;
	t_dblst	*del_node;
	t_ent	*new_prt;
	t_ent	*to_del;

	if (md->particles_alive > MAX_PARTICLES)
	{
		del_node = dblst_first(md->particles);
		to_del = (t_ent *)del_node;
		reset_mapped_end(md, to_del);
		dblst_delone(del_node, free);
	}
	new_prt = init_prt(md, emitter, start_pos, dir);
	new_node = dblst_new((void *)new_prt);
	dblst_add_back(&md->particles, new_node);
	md->particles_alive++;
	md->timer.shoot_timer = md->timer.current_time + SHOOT_REFRESH;
}
