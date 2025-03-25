/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:44:34 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 15:36:14 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	update_map_index(t_md *md, t_ent *e)
{
	int		new_index;
	char	cur_char;

	new_index = e->coord.x + ((md->map.size.x + 1) * e->coord.y);
	if (new_index == e->map_index)
		return (0);
	if (e->map_index < 0 || e->map_index >= md->map.len)
		return (0);
	md->mapped_ents[e->map_index] = NULL;
	if (md->mapped_ents[e->map_index] == e)
		md->mapped_ents[e->map_index] = NULL;
	else if (md->mapped_ents[e->map_index])
		return (0);
	cur_char = md->map.buffer[e->map_index];
	if (cur_char == '0' || cur_char == e->character)
		md->map.buffer[e->map_index] = '0';
	cur_char = md->map.buffer[new_index];
	if (char_in_str(cur_char, "\n1"))
		return (0);
	md->map.buffer[new_index] = e->character;
	e->map_index = new_index;
	md->mapped_ents[e->map_index] = e;
	return (1);
}

static int	get_valid_moves(t_md *md, t_vec3 cord, \
	const t_vec2 *moves, int **val_mvs)
{
	int				i;
	int				nw_indx;
	t_vec3			new_cord;
	int				valid_indexes;

	valid_indexes = 0;
	i = -1;
	while (++i < 4)
	{
		new_cord = (t_vec3){cord.x + moves[i].x, cord.y + moves[i].y, cord.z};
		nw_indx = new_cord.x + ((md->map.size.x + 1) * new_cord.y);
		if (nw_indx < 0 || nw_indx >= md->map.len || md->mapped_ents[nw_indx])
			continue ;
		(*val_mvs)[valid_indexes++] = i;
	}
	return (valid_indexes);
}

static void	set_ent_target_pos(t_md *md, t_ent *e)
{
	const t_vec2	moves[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
	int				*valid_moves;
	int				valid_amount;
	int				move_dir;

	valid_moves = malloc(sizeof(int) * 4);
	valid_amount = get_valid_moves(md, e->coord, moves, &valid_moves);
	if (!valid_amount)
		return ;
	move_dir = valid_moves[r_range_seed(&md->random_seed, 0, valid_amount - 1)];
	e->target_pos = (t_vec3f){e->pos.x + moves[move_dir].x * md->t_len, \
		e->pos.y + moves[move_dir].y * md->t_len, e->pos.z};
}

void	move_ent_to_target(t_md *md, t_ent *e, t_vec3f target_p)
{
	t_vec3f			dir;
	t_vec3			new_cord;

	e->action = m_walk;
	dir = normalize_vec3f(sub_vec3f(target_p, e->pos));
	e->pos = add_vec3f(e->pos, scale_vec3f(dir, 5));
	if (cmp_vec3f(e->pos, target_p, 1))
	{
		e->pos = get_v3f((int)target_p.x, (int)target_p.y, e->pos.z);
		new_cord = get_v3(e->pos.x / md->t_len, \
			e->pos.y / md->t_len, e->pos.z / md->t_len);
		e->coord = new_cord;
		md->mapped_ents[e->map_index] = NULL;
		e->map_index = new_cord.x + ((md->map.size.x + 1) * new_cord.y);
		md->mapped_ents[e->map_index] = e;
		e->target_pos.x = -999;
	}
}

void	update_mob_actions(t_md *md, t_ent *e)
{
	t_ent_action	prev_action;

	prev_action = e->action;
	if (e->in_screen >= e->size.x * .7)
	{
		e->action = m_atk;
		if (!md->update_frames || e->frame_index > 1 || md->plr.was_hit)
			return ;
		if (e->type != (int)Rat)
		{
			md->hud.hp -= e->type;
			md->plr.was_hit = 10;
			return ;
		}
		else
			e->target_pos = md->plr.pos;
	}
	if (e->target_pos.x != -999)
		move_ent_to_target(md, e, e->target_pos);
	else if (md->timer.time % 20 == 0)
		set_ent_target_pos(md, e);
	else
		e->action = m_idle;
	if (e->action != prev_action)
		e->frame_index = 0;
}
