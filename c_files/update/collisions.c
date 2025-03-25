/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:44:12 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 16:13:18 by gvalente         ###   ########.fr       */
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
	if (b->type == nt_door)
		return (play_sound(md, AU_OPEN), 1);
	play_sound(md, AU_GRAB);
	return (1);
}

int	angle_diff(int a, int b)
{
	int	diff;

	diff = (a - b) % 360;
	if (diff < -180)
		diff += 360;
	if (diff > 180)
		diff -= 360;
	return (diff);
}

static int	pass_through_portal(t_md *md, t_ent *portal, \
	t_ent *plr, t_vec2 portal_pos)
{
	t_wrd_dir	rot;
	int			new_rot;
	t_vec2		new_pos;

	if (v2_bounds(get_v2((int)plr->pos.x, (int)plr->pos.y), \
	portal_pos, portal->overlay->size))
		return (1);
	rot = portal->overlay_dir;
	new_pos = portal_pos;
	if (rot == NORTH)
	{
		new_pos.y += md->t_len * 1.3;
		new_rot = 90;
	}
	else if (rot == SOUTH)
	{
		new_pos.y -= md->t_len * 1.3;
		new_rot = -90;
	}
	else if (rot == EAST)
	{
		new_rot = 180;
		new_pos.x -= md->t_len * 1.3;
	}
	else
	{
		new_pos.x += md->t_len * 1.3;
		new_rot = 0;
	}
	md->plr_rot.x = new_rot;
	plr->angle = (md->plr_rot.x) * (M_PI / 180.0f);
	plr->dir.x = cosf(plr->angle);
	plr->dir.y = sinf(plr->angle);
	plr->pos.x = new_pos.x;
	plr->pos.y = new_pos.y;
	if (md->input_mov.y < 0)
	{
		md->plr_rot.x = -md->plr_rot.x;
		plr->angle = (md->plr_rot.x) * (M_PI / 180.0f);
		plr->dir.x = cosf(plr->angle);
		plr->dir.y = sinf(plr->angle);
	}
	return (0);
}

static int	validate_collision(t_md *md, t_ent *a, t_ent *b, t_vec2 a_size)
{
	int	solved;

	if (!a || !b)
		return (0);
	if (!a->is_active || !b->is_active)
		return (0);
	if (md->portal_gun.last_passage == b)
		return (1);
	if (!md->prm.ent_mode && b->type != nt_door && b->type != nt_wall)
		return (0);
	if (a->type == nt_plr && (b->type == nt_pickup || b->type == nt_door))
	{
		if (is_collision(a, b, v2(md->t_len * 2)))
			solved = handle_soft_collisions(md, b);
		if (b->type != nt_door || solved)
			return (0);
	}
	if (!is_collision(a, b, a_size))
		return (0);
	if (b->type == nt_wall && b->overlay && md->portal_gun.entrance == b)
		return (pass_through_portal(md, md->portal_gun.exit, &md->plr, md->portal_gun.exit_pos));
	if (b->type == nt_wall && b->overlay && md->portal_gun.exit == b)
		return (pass_through_portal(md, md->portal_gun.entrance, &md->plr, md->portal_gun.entrance_pos));
	if (b->mob_type == Rat && b->action == m_atk)
		md->hud.hp--;
	if (b->type != nt_wall && b->type != nt_door)
		return (0);
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
