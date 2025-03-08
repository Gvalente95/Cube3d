/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 00:11:00 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/07 12:18:31 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

t_ent	*init_player(t_md *md, char c, t_vec2 pos)
{
	t_ent	*e;

	e = &md->plr;
	e->type = player;
	e->action = IDLE;
	e->hp = 0;
	e->is_active = 1;
	e->in_screen = 1;
	e->frame = add_img("mlx_png/plr.png", &e->size.x, &e->size.y, md);
	e->frame = scale_abs_img(md, e->frame, &e->size, get_v2(md->t_len / 2, md->t_len / 2));
	e->hurt_timer = 0;
	e->pos = get_v3f((float)(pos.x * md->t_len), (float)(pos.y * md->t_len), 0);
	e->start_pos = get_v3f(e->pos.x, e->pos.y, e->pos.z);
	e->coord_pos = get_v3(pos.x, pos.y, 0);
	e->mov = get_v3f(0, 0, 0);
	e->prv_pos = get_v3f(e->pos.x, e->pos.y, e->pos.z);
	e->rot = get_v3f(0, 0, 0);
	if (c == 'S')
		e->rot.x = -180;
	else if (c == 'E')
		e->rot.x = 90;
	else if (c == 'W')
		e->rot.x = -90;
	e->jumps = 0;
	e->jump_timer = 0;
	e->level = 0;
	e->flip_x = 0;
	e->audio = 0;
	return (e);
}

t_ent	*init_ent(t_md *md, char c, t_vec2 pos)
{
	t_ent	*e;

	if (char_in_str(c, "NSEW"))
		return (init_player(md, c, pos));
	e = malloc(sizeof(t_ent));
	e->frame = md->wall_txtr[0];
	e->is_active = 1;
	e->in_screen = 1;
	e->type = wall;
	e->action = IDLE;
	e->hp = 0;
	e->hurt_timer = 0;
	e->size = get_v2(md->t_len, md->t_len);
	e->pos = get_v3f((float)(pos.x * md->t_len), (float)(pos.y * md->t_len), 0);
	e->mov = get_v3f(0, 0, 0);
	e->coord_pos = get_v3(pos.x, pos.y, 0);
	e->prv_pos = get_v3f(e->pos.x, e->pos.y, e->pos.z);
	e->rot = get_v3f(0, 0, 0);
	e->jumps = 0;
	e->jump_timer = 0;
	e->level = 0;
	e->flip_x = 0;
	e->audio = 0;
	return (e);
}

int	init_entities(t_md *md)
{
	int			i;
	int			x;
	int			y;
	t_dblist	*ents;
	t_dblist	*node;
	t_ent		*e;
	t_vec2		crd_pos;

	ents = NULL;
	x = 0;
	y = 0;
	i = -1;
	while (md->map.buffer[++i])
	{
		if (md->map.buffer[i] == '\n')
		{
			y++;
			x = 0;
			continue ;
		}
		if (!char_in_str(md->map.buffer[i], "1NSEW"))
		{
			x++;
			continue ;
		}
		crd_pos = get_v2(x, y);
		e = init_ent(md, md->map.buffer[i], crd_pos);
		if (e->type == player)
		{
			x++;
			continue ;
		}
		node = dblst_new((void *)e);
		dblst_add_back(&ents, node);
		x++;
	}
	md->entities = dblst_first(ents);
	return (1);
}
