/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:44:12 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/07 10:27:50 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	set_collisions_rules(t_ent *emitter, t_ent *receiver)
{
	t_ent_type	type;
	t_ent_type	col_type;

	return (1);
	if (!receiver->is_active || !emitter->is_active || !receiver->hp)
		return (0);
	if (!emitter->in_screen && emitter->type != particle)
		return (0);
	type = emitter->type;
	col_type = receiver->type;
	if (type == particle)
		return (col_type != portal);
	if (type == player || type == laser)
		return (col_type == wall || col_type == mover \
			|| col_type == door || col_type == mob || col_type == tile);
	if (type == coin || type == key || type == axe)
		return (0);
	if (type == mob)
		return (col_type == wall || col_type == mover || col_type == door \
			|| col_type == tile);
	if (type == portal)
		return (col_type == coin);
	if (type == mover)
		return (col_type == wall || col_type == door || \
			col_type == mover || col_type == tile);
	return (0);
}

t_vec3f	calculate_displacement(t_ent *a, t_ent *b, t_vec2 displ)
{
	t_vec3f	displacement;
	int		dlt_l;
	int		dlt_r;
	int		dlt_up;
	int		dlt_down;

	displacement = get_v3f(0, 0, 0);
	dlt_l = (a->pos.x + a->size.x) - b->pos.x;
	dlt_r = (b->pos.x + b->size.x) - a->pos.x;
	dlt_up = (a->pos.y + a->size.y) - b->pos.y;
	dlt_down = (b->pos.y + b->size.y) - a->pos.y;
	if (dlt_up < dlt_down && dlt_up < dlt_l && dlt_up < dlt_r)
		displacement.y = -dlt_up + displ.y;
	else if (dlt_down < dlt_up && dlt_down < dlt_l && dlt_down < dlt_r)
		displacement.y = dlt_down - displ.y;
	if (dlt_l < dlt_r && dlt_l < dlt_up && dlt_l < dlt_down)
		displacement.x = -dlt_l + displ.x;
	else if (dlt_r < dlt_up && dlt_r < dlt_down)
		displacement.x = dlt_r - displ.x;
	return (displacement);
}

t_vec3f	get_collision_displacement(t_ent *e1, t_ent *e2, t_vec2 displ)
{
	t_vec3f	displacement;
	int		is_same_x;
	int		is_same_y;

	displacement = get_v3f(0, 0, 0);
	is_same_x = (e1->pos.x + displ.x < (e2->pos.x + e2->size.x) && \
		(e1->pos.x + e1->size.x - displ.x > e2->pos.x));
	is_same_y = (e1->pos.y + displ.y < e2->pos.y + e2->size.y && \
		e1->pos.y + e1->size.y - displ.y > e2->pos.y);
	if (is_same_x && is_same_y)
		displacement = calculate_displacement(e1, e2, displ);
	else if (displacement.y < 0)
		e1->is_grounded = 1;
	return (displacement);
}

t_vec3f	get_collisions(t_md *md, t_ent *e, t_vec2 displ)
{
	t_vec3f		cols;
	t_vec3f		new_dspl;
	t_ent		*col;
	t_dblist	*node;

	cols = get_v3f(0, 0, 0);
	e->is_grounded = 0;
	node = md->entities;
	while (node)
	{
		col = (t_ent *)node->content;
		if (!set_collisions_rules(e, col))
			continue ;
		new_dspl = get_collision_displacement(e, col, displ);
		cols = get_v3f(cols.x + new_dspl.x, cols.y + new_dspl.y, cols.z + new_dspl.z);
		if (new_dspl.x && e->type == mover && (col->type == mover))
			col->mov.x = -col->mov.x;
		if (new_dspl.y && (col->type == mover))
			e->mov.y = col->mov.y;
		node = node->next;
	}
	return (cols);
}
