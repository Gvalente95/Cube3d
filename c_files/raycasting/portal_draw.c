/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:09:50 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 12:56:00 by giuliovalen      ###   ########.fr       */
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
	draw_sphere(img, glow_pos, glow_size, get_v3(colors.z, 50, 1));
	draw_sphere(img, pos, size, get_v3(colors.x, 0, 1));
}

static t_vec2	rotate_overlay_offset(t_vec2 offset, t_wrd_dir dir)
{
	t_vec2	result;

	if (dir == NORTH)
		return (offset);
	else if (dir == EAST)
		result = (get_v2(offset.y, -offset.x));
	else if (dir == SOUTH)
		result = (get_v2(-offset.x, -offset.y));
	else if (dir == WEST)
		result = (get_v2(-offset.y, offset.x));
	else
		result = offset;
	return (result);
}

static void	clear_prev_portal(t_md *md, t_ent *e, \
	int is_exit, t_vec2 world_p)
{
	if (is_exit)
	{
		free_image_data(md, md->portal_gun.exit->overlay);
		md->portal_gun.exit->overlay = NULL;
		md->portal_gun.exit = e;
		md->portal_gun.exit_pos = world_p;
		md->portal_gun.last_shot_exit = 0;
	}
	else
	{
		free_image_data(md, md->portal_gun.entrance->overlay);
		md->portal_gun.entrance->overlay = NULL;
		md->portal_gun.entrance = e;
		md->portal_gun.entrance_pos = world_p;
		md->portal_gun.last_shot_exit = 1;
	}
}

static int	set_single(t_md *md, t_ent *e, t_vec2 overlay_pos, t_vec4 data)
{
	t_vec2	draw_pos;
	t_vec2	size;

	draw_pos = get_v2(data.r, data.g);
	size = get_v2(data.b, data.a);
	if (!md->portal_gun.entrance)
	{
		md->portal_gun.entrance_pos = overlay_pos;
		md->portal_gun.entrance = e;
		draw_portal_layers(e->overlay, draw_pos, size,
			get_v3(md->rgb[RGB_BLUE], 0, md->rgb[RGB_ORANGE]));
		return (1);
	}
	if (!md->portal_gun.exit)
	{
		md->portal_gun.exit = e;
		md->portal_gun.exit_pos = overlay_pos;
		md->portal_gun.last_shot_exit = 1;
		draw_portal_layers(e->overlay, draw_pos, size,
			get_v3(md->rgb[RGB_RED], 0, md->rgb[RGB_INDIGO]));
		return (1);
	}
	return (0);
}

void	draw_portal(t_md *md, t_ent *e, t_vec2 pos)
{
	t_vec2	draw_sz;
	t_vec2	draw_p;
	t_vec2	world_pos;
	t_vec2	rot_offst;

	if (e->overlay || e->type != nt_wall)
		return ;
	draw_sz = get_v2(md->t_len / 3, md->t_len * 0.75f);
	draw_p = get_v2(\
		minmax(10, e->size.x - draw_sz.x - 3, pos.x - draw_sz.x / 2), \
		minmax(10, e->size.y - draw_sz.y - 3, pos.y - draw_sz.y / 2));
	rot_offst = rotate_overlay_offset(draw_p, e->overlay_dir);
	world_pos = get_v2(e->pos.x + rot_offst.x, e->pos.y + rot_offst.y);
	e->overlay = copy_image(md, e->frame, e->frame->size, -1);
	if (set_single(md, e, world_pos, get_v4(draw_p.x, draw_p.y, draw_sz.x, draw_sz.y)))
		return ;
	clear_prev_portal(md, e, md->portal_gun.last_shot_exit, world_pos);
	if (md->portal_gun.last_shot_exit)
		draw_portal_layers(e->overlay, draw_p, draw_sz, \
			get_v3(md->rgb[RGB_BLUE], 0, md->rgb[RGB_ORANGE]));
	else
		draw_portal_layers(e->overlay, draw_p, draw_sz, \
			get_v3(md->rgb[RGB_RED], 0, md->rgb[RGB_INDIGO]));
}
