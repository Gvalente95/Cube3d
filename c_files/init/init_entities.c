/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 00:11:00 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 03:31:04 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	set_type_specifics(t_md *md, t_ent *e, t_ent_type type, char c)
{
	(void)c;
	if (type == nt_wall)
	{
		e->frames = malloc(sizeof(t_image) * 4);
		e->frames[0] = copy_image(md, md->wall_img[0]);
		e->frames[1] = copy_image(md, md->wall_img[1]);
		e->frames[2] = copy_image(md, md->wall_img[2]);
		e->frames[3] = copy_image(md, md->wall_img[3]);
	}
	else
		e->dir = get_v3f(r_range(-1, 1), r_range(-1, 1), r_range(-1, 1));
}

void	set_base_ent_values(t_md *md, t_ent *e, char c, t_vec2 pos)
{
	e->character = c;
	e->size = get_v2(md->t_len, md->t_len);
	init_ent_frames(md, e, c);
	e->pos.x = (pos.x * md->t_len) + (md->t_len - e->size.x) * 0.5f;
	e->pos.y = (pos.y * md->t_len) + (md->t_len - e->size.y) * 0.5f;
	e->pos.z = 0;
	e->target_pos = get_v3f(-999, 0, 0);
	e->start_pos = get_v3f(e->pos.x, e->pos.y, e->pos.z);
	e->coord_pos = get_v3(pos.x, pos.y, 0);
	e->mov = get_v3f(0, 0, 0);
	e->rot = get_v3(0, 0, 0);
	e->dir = get_v3f(0, 0, 0);
	e->is_active = 1;
	e->in_screen = 0;
	e->shot = 0;
	e->was_hit = 0;
	e->shot_timer = 0;
	e->can_shoot = 1;
	e->row_draw_index = 0;
	e->hp = 5;
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
	md->mmap.limits_x = get_v2(md->plr.pos.x, md->plr.pos.x);
	md->mmap.limits_y = get_v2(md->plr.pos.y, md->plr.pos.y);
	if (c == 'N')
		md->plr.rot.x = -90;
	if (c == 'S')
		md->plr.rot.x = 90;
	else if (c == 'W')
		md->plr.rot.x = -180;
	md->plr.angle = md->plr.rot.x * (M_PI / 180.0f);
	md->plr.size = get_v2(md->t_len / 2, md->t_len / 2);
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
