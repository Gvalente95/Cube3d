/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:44:12 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/04 11:50:32 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	solve_collision(t_ent *a, t_ent *b, t_vec2 a_size)
{
	float	abs_x;
	float	abs_y;

	a->col_hit = b;
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

int	is_collision(t_ent *a, t_ent *b, t_vec2 a_size)
{
	return (!(a->pos.y + a->mov.y + a_size.y < b->pos.y || \
		a->pos.y + a->mov.y > b->pos.y + b->size.y || \
		a->pos.x + a->mov.x + a_size.x < b->pos.x || \
		a->pos.x + a->mov.x > b->pos.x + b->size.x));
}

static int	handle_soft_collisions(t_md *md, t_ent *b)
{
	if (b->type == nt_door && md->hud.keys)
		md->hud.keys--;
	else if (b->type == nt_pickup)
	{
		if (b->pckp_type == Ammo && md->hud.ammo < MAX_AMMO)
			md->hud.ammo = minmax(0, MAX_AMMO, md->hud.ammo + 5);
		else if (b->pckp_type == Keys && md->hud.keys < MAX_KEY)
			md->hud.keys++;
		else if (b->pckp_type == Health && md->hud.hp < 100)
			md->hud.hp += 5;
		else if (b->pckp_type == Score)
			md->score += 50;
		else
			return (1);
	}
	else if (b->type == (int)Weapon && !md->hud.unlocked_weapons[b->wpn_type])
		md->hud.unlocked_weapons[b->wpn_type] = 1;
	else
		return (0);
	reset_mapped_end(md, b);
	b->is_active = 0;
	play_sound(md, AU_GRAB);
	return (1);
}

static int	validate_collision(t_md *md, t_ent *a, t_ent *b, t_vec2 a_size)
{
	if (!a || !b)
		return (0);
	if (b->type == nt_empty)
		return (0);
	if (!a->is_active || !b->is_active)
		return (0);
	if (!md->prm.ent_mode && b->type != nt_door && b->type != nt_wall)
		return (0);
	if (a->type == nt_plr && (b->type == nt_pickup))
	{
		if (is_collision(a, b, v2(md->t_len * 2)))
			handle_soft_collisions(md, b);
		return (0);
	}
	if (b->type != nt_wall && b->type != nt_door)
		return (0);
	if (b->type == nt_door && !b->hp)
		return (0);
	if (!is_collision(a, b, a_size))
		return (0);
	if (b->type == nt_wall && b->overlay)
		return (validate_portal_collision(md, b));
	return (1);
}

int	set_collisions(t_md *md, t_ent *e, t_vec2 e_size)
{
	const t_vec2	neighbors[] = {{-1, -1}, {0, -1}, {1, -1}, \
									{-1, 0}, {0, 0}, {1, 0}, \
									{-1, 1}, {0, 1}, {1, 1}};
	t_vec2			cord;
	int				i;
	int				col_amount;
	int				map_i;

	if (md->prm.fly_cam)
		return (1);
	col_amount = 0;
	e->col_hit = NULL;
	i = -1;
	while (++i < 9)
	{
		cord = get_v2(e->coord.x + neighbors[i].x, e->coord.y + neighbors[i].y);
		map_i = cord.x + ((md->map.size.x + 1) * cord.y);
		if (map_i < 0 || map_i >= md->map.len || !md->mapped_ents[map_i])
			continue ;
		if (validate_collision(md, e, md->mapped_ents[map_i], e_size))
			col_amount += solve_collision(e, md->mapped_ents[map_i], e_size);
	}
	return (col_amount);
}
