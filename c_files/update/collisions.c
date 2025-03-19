/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:44:12 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/18 21:28:59 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	resolve_entity_collision(t_ent *a, t_ent *b, t_vec2 a_size)
{
	float	abs_x;
	float	abs_y;

	abs_x = fabsf((a->pos.x + a_size.x / 2 + a->mov.x) - \
					(b->pos.x + b->size.x / 2));
	abs_y = fabsf((a->pos.y + a_size.y / 2 + a->mov.y) - \
					(b->pos.y + b->size.y / 2));
	if (abs_y > abs_x)
	{
		a->mov.y = 0;
		if (a->pos.y < b->pos.y)
			a->pos.y = b->pos.y - a_size.y;
		else
			a->pos.y = b->pos.y + b->size.y;
	}
	else if (abs_x > abs_y)
	{
		a->mov.x = 0;
		if (a->pos.x < b->pos.x)
			a->pos.x = b->pos.x - a_size.x;
		else
			a->pos.x = b->pos.x + b->size.x;
	}
	return (1);
}

static int	process_collision(t_ent *e, t_ent *col, t_vec2 a_size)
{
	if (e->type == nt_plr && col->type == nt_pickup)
		return (col->is_active = 0, 0);
	e->col_hit = col;
	return (resolve_entity_collision(e, col, a_size));
}

int	is_collision(t_ent *a, t_ent *b, t_vec2 a_size)
{
	return (!(a->pos.y + a->mov.y + a_size.y < b->pos.y || \
		a->pos.y + a->mov.y > b->pos.y + b->size.y || \
		a->pos.x + a->mov.x + a_size.x < b->pos.x || \
		a->pos.x + a->mov.x > b->pos.x + b->size.x));
}

static int	validate_collision(t_ent *a, t_ent *b, t_vec2 a_size)
{
	if (b->type != nt_wall)
		return (0);
	if (!a->mov.x && !a->mov.y && !a->mov.z)
		return (0);
	if (!b->is_active)
		return (0);
	if (!a->is_active)
		return (0);
	if (!is_collision(a, b, a_size))
		return (0);
	return (1);
}

int	set_collisions(t_md *md, t_ent *e, t_vec2 e_size)
{
	t_ent	*col;
	t_dblst	*node;
	int		collisions;

	collisions = 0;
	e->col_hit = NULL;
	node = dblst_first(md->entities);
	while (node)
	{
		col = (t_ent *)node->content;
		if (validate_collision(e, col, e_size))
			collisions += process_collision(e, col, e_size);
		node = node->next;
	}
	return (collisions);
}
