/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:44:34 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 05:31:55 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	move_player(t_md *md, t_ent *e)
{
	t_vec3f	sign_mov;
	t_vec3f	decc_mov;

	sign_mov = get_v3f(e->mov.x * PLRSPD, e->mov.y * PLRSPD, e->mov.z * PLRSPD);
	decc_mov = get_v3f(e->mov.x * ACCSPD, e->mov.y * ACCSPD, e->mov.z * ACCSPD);
	if (e->pos.z + sign_mov.z > 0)
	{
		e->pos.z = 0;
		decc_mov.z = 0;
		sign_mov.z = 0;
		e->mov.z = 0;
	}
	e->pos = get_v3f(\
		e->pos.x + sign_mov.x, e->pos.y + sign_mov.y, e->pos.z + sign_mov.z);
	e->mov = get_v3f(\
		e->mov.x - decc_mov.x, e->mov.y - decc_mov.y, e->mov.z);
	e->coord_pos = get_v3(\
		e->pos.x / md->t_len, e->pos.y / md->t_len, e->pos.z / md->t_len);
	return (1);
}

t_vec3f	get_dir_towards(t_ent *from, t_ent *to, float speed)
{
	t_vec3f	movement_towards;
	float	angle_towards;

	angle_towards = atan2f(to->pos.y - from->pos.y, to->pos.x - from->pos.x);
	movement_towards.x = cosf(angle_towards) * speed;
	movement_towards.y = sinf(angle_towards) * speed;
	movement_towards.z = 0;
	return (movement_towards);
}

int	move_ent(t_md *md, t_ent *e)
{
	int	new_index;
	int	dx;
	int	dy;

	dx = ft_sign(e->dir.x);
	dy = ft_sign(e->dir.y);
	if (dx == 0 && dy == 0)
		return (0);
	new_index = e->map_index + dx + (dy * md->map.size.x);
	if (new_index < 0 || new_index >= md->map.len)
		return (0);
	if (md->mapped_ents[new_index] && \
		md->mapped_ents[new_index]->type == nt_wall)
		return (0);
	e->pos.x += dx * md->t_len;
	e->pos.y += dy * md->t_len;
	e->coord_pos.x = e->pos.x / md->t_len;
	e->coord_pos.y = e->pos.y / md->t_len;
	md->mapped_ents[e->map_index] = NULL;
	md->mapped_ents[new_index] = e;
	e->map_index = new_index;
	print_vec3f(e->pos, "ent pos");
	return (1);
}

int	update_map_index(t_md *md, t_ent *e)
{
	int		new_index;
	char	cur_char;

	new_index = e->coord_pos.x + ((md->map.size.x + 1) * e->coord_pos.y);
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

static void	move_ent_on_grid(t_md *md, t_ent *e, t_vec3 cord)
{
	const t_vec2	r_pos[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
	t_vec3			valid_cord[4];
	t_vec3			new_cord;
	int				valid_dir[4];
	int				valid_index;
	int				i;
	int				new_index;
	int				random_index;
	int				move_dir;

	valid_index = 0;
	i = -1;
	while (++i < 4)
	{
		new_cord = (t_vec3){cord.x + r_pos[i].x, cord.y + r_pos[i].y, cord.z};
		new_index = new_cord.x + ((md->map.size.x + 1) * new_cord.y);
		if (new_index < 0 || new_index >= md->map.len || \
			md->mapped_ents[new_index])
			continue ;
		valid_cord[valid_index] = new_cord;
		valid_dir[valid_index] = i;
		valid_index++;
	}
	if (valid_index == 0)
		return ;
	random_index = r_range_seed(&md->random_seed, 0, valid_index - 1);
	move_dir = valid_dir[random_index];
	e->target_pos = (t_vec3f){e->pos.x + r_pos[move_dir].x * md->t_len, \
		e->pos.y + r_pos[move_dir].y * md->t_len, e->pos.z};
}

void	update_mob_actions(t_md *md, t_ent *e)
{
	t_vec3f	dir;
	t_vec3f	target_p;
	t_vec3	new_cord;

	if (e->in_screen >= e->size.x * .2)
	{
		e->action = m_atk;
		return ;
	}
	target_p = e->target_pos;
	if (target_p.x != -999)
	{
		e->action = m_walk;
		dir = normalize_vec3f(sub_vec3f(target_p, e->pos));
		e->pos = add_vec3f(e->pos, scale_vec3f(dir, 5));
		if (cmp_vec3f(e->pos, target_p, 1))
		{
			e->pos = get_v3f((int)target_p.x, (int)target_p.y, e->pos.z);
			new_cord = get_v3(e->pos.x / md->t_len, \
				e->pos.y / md->t_len, e->pos.z / md->t_len);
			e->coord_pos = new_cord;
			e->map_index = new_cord.x + ((md->map.size.x + 1) * new_cord.y);
			md->mapped_ents[e->map_index] = e;
			target_p.x = -999;
			e->target_pos.x = -999;
		}
	}
	else if (md->timer.time % 20 == 0)
		move_ent_on_grid(md, e, e->coord_pos);
}
