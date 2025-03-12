/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:44:12 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/12 06:16:16 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	resolve_entity_collision(t_ent *a, t_ent *b)
{
	float	abs_x;
	float	abs_y;

	abs_x = fabsf((a->pos.x + a->size.x / 2 + a->mov.x) - \
					(b->pos.x + b->size.x / 2));
	abs_y = fabsf((a->pos.y + a->size.y / 2 + a->mov.y) - \
					(b->pos.y + b->size.y / 2));
	if (abs_y > abs_x)
	{
		a->mov.y = 0;
		if (a->pos.y < b->pos.y)
			a->pos.y = b->pos.y - a->size.y;
		else
			a->pos.y = b->pos.y + b->size.y;
	}
	else if (abs_x > abs_y)
	{
		a->mov.x = 0;
		if (a->pos.x < b->pos.x)
			a->pos.x = b->pos.x - a->size.x;
		else
			a->pos.x = b->pos.x + b->size.x;
	}
	return (1);
}

static int	process_collision(t_md *md, t_ent *e, t_ent *col)
{
	(void)md;
	if (e->type == nt_plr && col->type == nt_coin)
	{
		col->is_active = 0;
		return (0);
	}
	e->col_hit = col;
	return (resolve_entity_collision(e, col));
}

int	is_collision(t_ent *a, t_ent *b)
{
	return (!(a->pos.y + a->mov.y + a->size.y < b->pos.y || \
		a->pos.y + a->mov.y > b->pos.y + b->size.y || \
		a->pos.x + a->mov.x + a->size.x < b->pos.x || \
		a->pos.x + a->mov.x > b->pos.x + b->size.x));
}

static int	validate_collision(t_ent *a, t_ent *b)
{
	if (!a->mov.x && !a->mov.y && !a->mov.z && b->type == nt_wall)
		return (0);
	if (!b->in_screen || !b->is_active)
		return (0);
	if (!a->in_screen || !a->is_active)
		return (0);
	return (1);
}

void	set_collisions(t_md *md, t_ent *e)
{
	t_ent	*col;
	t_dblst	*node;

	e->col_hit = NULL;
	node = md->entities;
	while (node)
	{
		col = (t_ent *)node->content;
		if (validate_collision(e, col) && is_collision(e, col))
			process_collision(md, e, col);
		node = node->next;
	}
}
