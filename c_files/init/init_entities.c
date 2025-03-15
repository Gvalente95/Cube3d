/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 00:11:00 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/15 00:12:44 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	set_type_specifics(t_md *md, t_ent *e, t_ent_type type, char c)
{
	(void)md;
	if (type == nt_plr)
	{
		if (c == 'N')
			e->rot.x = -90;
		if (c == 'S')
			e->rot.x = 90;
		else if (c == 'W')
			e->rot.x = -180;
	}
	if (e->type == nt_mob || e->type == nt_coin)
		e->pos.z = e->size.y;
	if (e->type != nt_wall)
		e->dir = get_v3f(r_range(-1, 1), r_range(-1, 1), r_range(-1, 1));
}


void	set_base_ent_values(t_md *md, t_ent *e, char c, t_vec2 pos)
{
	e->emitter = NULL;
	e->frame = NULL;
	e->type = get_char_index(md->ents_tp_map[0], c);
	e->action = ac_idl;
	e->character = c;
	e->size = md->e_sizes[e->type];
	e->pos = get_v3f((float)(pos.x * md->t_len), (float)(pos.y * md->t_len), 0);
	e->start_pos = get_v3f(e->pos.x, e->pos.y, e->pos.z);
	e->coord_pos = get_v3(pos.x, pos.y, 0);
	e->mov = get_v3f(0, 0, 0);
	e->rot = get_v3f(0, 0, 0);
	e->dir = get_v3f(0, 0, 0);
	e->is_active = 1;
	e->in_screen = 1;
	e->hp = 0;
	e->jumps = 0;
	e->level = 0;
	e->audio = 0;
	e->hurt_timer = 0;
	e->frame_index = 0;
}

static void	init_player(t_md *md, char c, t_vec2 pos, int map_index)
{
	c = '*';
	set_base_ent_values(md, &md->plr, c, pos);
	md->plr.map_index = map_index;
	md->mapped_ents[map_index] = &md->plr;
	set_type_specifics(md, &md->plr, nt_plr, c);
}

t_ent	*init_ent(t_md *md, char c, t_vec2 pos, int map_index)
{
	t_ent	*e;

	e = malloc(sizeof(t_ent));
	set_base_ent_values(md, e, c, pos);
	e->map_index = map_index;
	md->mapped_ents[map_index] = e;
	set_type_specifics(md, e, e->type, c);
	return (e);
}

void	init_entities(t_md *md, t_vec2 pos)
{
	int			i;
	t_dblst		*ents;
	t_ent		*e;

	ents = NULL;
	pos = get_v2(0, 0);
	i = -1;
	while (md->map.buffer[++i])
	{
		if (md->map.buffer[i] == '\n')
		{
			pos.y++;
			pos.x = 0;
			continue ;
		}
		if (char_in_str(md->map.buffer[i], "NSEW"))
			init_player(md, md->map.buffer[i], pos, i);
		else if (!char_in_str(md->map.buffer[i], " 0\n"))
		{
			e = init_ent(md, md->map.buffer[i], pos, i);
			dblst_add_back(&ents, dblst_new((void *)e));
		}
		pos.x++;
	}
	md->entities = dblst_first(ents);
}
