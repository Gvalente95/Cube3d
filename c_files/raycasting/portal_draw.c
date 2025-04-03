/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:09:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/03 12:11:17 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	draw_portal_layers(t_image *img, \
	t_vec2 pos, t_vec2 size, t_vec3 colors)
{
	t_vec2	glow_pos;
	t_vec2	glow_size;

	glow_pos = get_v2(pos.x - 5, pos.y - 5);
	glow_size = get_v2(size.x + 10, size.y + 10);
	if (0)
		draw_pixels(img, glow_pos, glow_size, colors.z);
	draw_pixels(img, pos, size, colors.x);
}

static void	set_portal_pos(t_ent *e, int x_pos_offset, t_vec2 *out_pos)
{
	t_wrd_dir	dir;

	dir = e->overlay_dir;
	if (dir == NORTH)
	{
		out_pos->y = e->pos.y + e->size.y;
		out_pos->x = e->pos.x + x_pos_offset;
	}
	else if (dir == SOUTH)
	{
		out_pos->y = e->pos.y;
		out_pos->x = e->pos.x + x_pos_offset;
	}
	else if (dir == EAST)
	{
		out_pos->x = e->pos.x;
		out_pos->y = e->pos.y + x_pos_offset;
	}
	else
	{
		out_pos->x = e->pos.y + e->size.y;
		out_pos->y = e->pos.y + x_pos_offset;
	}
}

static void	clear_prev_portal(t_md *md, t_ent *e, t_vec2 out_pos)
{
	t_portal	*p;
	int			index;

	index = md->portal.last_shot_index;
	p = &md->portal;
	free_image_data(md, p->ends[index].e->overlay);
	p->ends[index].e->overlay = NULL;
	p->ends[index].e = e;
	p->ends[index].dir = e->overlay_dir;
	p->ends[index].out = out_pos;
	md->portal.last_shot_index = !index;
}

static int	set_single(t_md *md, t_ent *e, t_vec2 out_pos, t_vec4 data)
{
	t_vec2	draw_pos;
	t_vec2	size;
	int		i;
	int		color;

	draw_pos = get_v2(data.r, data.g);
	size = get_v2(data.b, data.a);
	i = -1;
	while (++i < 2)
	{
		if (md->portal.ends[i].e)
			continue ;
		md->portal.ends[i].e = e;
		color = md->rgb[RGB_INDIGO];
		if (i == 1)
			color = md->rgb[RGB_ORANGE];
		draw_portal_layers(e->overlay, draw_pos, size,
			get_v3(md->rgb[RGB_BLUE], 0, color));
		md->portal.ends[i].out = out_pos;
		md->portal.ends[i].dir = e->overlay_dir;
		return (i);
	}
	return (-1);
}

void	draw_portal(t_md *md, t_ent *e, t_vec2 pos)
{
	t_vec2	draw_sz;
	t_vec2	draw_p;
	t_vec2	out_pos;
	int		index;

	if (e->overlay || e->type != nt_wall)
		return ;
	(void)pos;
	play_sound(md, AU_PORTAL_SHOOT);
	draw_sz = get_v2(md->t_len / 3, md->t_len * .75);
	draw_p = get_v2(e->size.x / 2 - draw_sz.x / 2, \
		md->t_len * .25);
	e->overlay = copy_image(md, e->frame, e->frame->size, -1);
	set_portal_pos(e, e->size.x / 2, &out_pos);
	index = set_single(md, e, out_pos, \
		get_v4(draw_p.x, draw_p.y, draw_sz.x, draw_sz.y));
	if (index != -1)
		return ;
	clear_prev_portal(md, e, out_pos);
	if (md->portal.last_shot_index == 0)
		draw_portal_layers(e->overlay, draw_p, draw_sz, \
			get_v3(md->rgb[RGB_BLUE], 0, md->rgb[RGB_ORANGE]));
	else
		draw_portal_layers(e->overlay, draw_p, draw_sz, \
			get_v3(md->rgb[RGB_BLUE], 0, md->rgb[RGB_INDIGO]));
}
