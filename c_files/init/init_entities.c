/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 00:11:00 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 20:01:29 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	set_type_specifics(t_md *md, t_ent *e, t_ent_type type, char c)
{
	(void)c;
	if (type == nt_wall)
	{
		e->frames = malloc(sizeof(t_image) * 5);
		e->frames[0] = copy_image(md, md->txd.wall_img[0], get_v2(-1, -1), -1);
		e->frames[1] = copy_image(md, md->txd.wall_img[1], get_v2(-1, -1), -1);
		e->frames[2] = copy_image(md, md->txd.wall_img[2], get_v2(-1, -1), -1);
		e->frames[3] = copy_image(md, md->txd.wall_img[3], get_v2(-1, -1), -1);
		e->frames[4] = NULL;
	}
	else
		e->dir = get_v3f(r_range(-1, 1), r_range(-1, 1), r_range(-1, 1));
	if (e->type == nt_mob)
		e->hp = ((int)(e->mob_type + 1) * 2) * (md->prm.difficulty);
}

void	set_base_ent_values(t_md *md, t_ent *e, char c, t_vec2 pos)
{
	e->anim = NULL;
	e->frames = NULL;
	e->overlay = NULL;
	e->overlay_dir = -1;
	e->character = c;
	e->type = get_char_index(md->txd.ents_tp_map[0], c);
	init_ent_frames(md, &md->txd, e);
	e->size = e->frame->size;
	e->pos.x = (pos.x * md->t_len) + (md->t_len - e->size.x) * 0.5f;
	e->pos.y = (pos.y * md->t_len) + (md->t_len - e->size.y) * 0.5f;
	e->pos.z = 0;
	e->target_pos = get_v3f(-999, 0, 0);
	e->start_pos = get_v3f(e->pos.x, e->pos.y, e->pos.z);
	e->coord = get_v3(pos.x, pos.y, 0);
	e->mov = get_v3f(0, 0, 0);
	e->dir = get_v3f(0, 0, 0);
	e->is_active = 1;
	e->in_screen = 0;
	e->shot = 0;
	e->was_hit = 0;
	e->shot_timer = 0;
	e->can_shoot = 1;
	e->hp = 5;
	e->level = 0;
	e->hurt_timer = 0;
}

static void	init_player(t_md *md, char c, t_vec2 pos, int map_index)
{
	char	base_c;

	base_c = c;
	c = '*';
	set_base_ent_values(md, &md->plr, c, pos);
	md->plr.map_index = map_index;
	md->mapped_ents[map_index] = &md->plr;
	md->plr_rot.x = -90;
	if (base_c == 'S')
		md->plr_rot.x = 90;
	if (base_c == 'E')
		md->plr_rot.x = 0;
	else if (base_c == 'W')
		md->plr_rot.x = 180;
	md->plr.angle = md->plr_rot.x * (M_PI / 180.0f);
	md->plr.size = get_v2(md->t_len / 2, md->t_len / 2);
	md->plr.pos.z = 0;
	md->cam_pos = md->plr.pos;
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
