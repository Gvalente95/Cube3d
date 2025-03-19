/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprite_column.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 04:30:37 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 04:50:18 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	paint_ent(t_md *md, t_ent *e, t_vec2 txtr_coord)
{
	int			action_index;
	int			frame_index;
	t_image		*img;

	txtr_coord.x += r_range(-30, 30);
	txtr_coord.y += r_range(-10, 10);
	action_index = -1;
	while (++action_index < ENT_ACTION_LEN)
	{
		if (action_index == m_death)
			txtr_coord.y += e->frame->size.y * .5;
		frame_index = -1;
		while (e->anim[action_index][++frame_index])
		{
			img = e->anim[action_index][frame_index];
			draw_blood(md, img, txtr_coord, md->rgb[RGB_RED]);
		}
	}
	e->was_hit = 1;
	if (e->type != nt_mob)
		return ;
	e->hp--;
	if (!e->hp)
		e->frame_index = 0;
}

int	get_prspctive_offset(t_md *md, float ray_dst, t_ent *e)
{
	float	pitch_factor;
	int		pitch_offset;
	float	vrt_offst;

	pitch_factor = tanf(md->plr.rot.y * (M_PI / 180.0f));
	vrt_offst = (md->plr.pos.z - e->pos.z) * md->win_size.y / (ray_dst + 1.0f);
	pitch_offset = (-pitch_factor * md->win_size.y / 2) - vrt_offst;
	return (pitch_offset);
}

void	draw_sprite_slice(t_md *md, t_ent *ent, t_vec2 winp, t_vec3f crd)
{
	t_vec2	y;
	t_vec2	pxl;
	t_vec2	wins;
	int		img_y;

	wins = md->win_size;
	y = (t_vec2){(wins.y / 2 - crd.y / 2) - 1, (wins.y / 2 + crd.y / 2)};
	crd.x = minmax(0, ent->frame->size.x - 1, crd.x);
	while (++y.x < y.y)
	{
		img_y = (y.x - (wins.y / 2 - crd.y / 2)) * (ent->frame->size.y / crd.y);
		if (img_y < 0 || img_y >= ent->frame->size.y)
			continue ;
		pxl.y = (img_y * (ent->frame->size_line / 4)) + (int)crd.x;
		pxl.x = *(ent->frame->src_data + pxl.y);
		if ((pxl.x >> 24) != 0x00)
			continue ;
		if (ent->was_hit == 2)
			pxl.x = md->rgb[RGB_RED];
		draw_pixel(md->screen, \
			get_v2(winp.x, y.x + winp.y - md->plr.pos.z), pxl.x, -1);
		if (md->plr.shot && winp.x == wins.x / 2 && ent->hp && \
			y.x + winp.y - md->plr.pos.z - wins.y / 2 < md->t_len)
			paint_ent(md, ent, get_v2((int)crd.x, (int)crd.x));
	}
}

void	draw_sprite_pxl(t_md *md, t_ray *ray, t_ent *sprite, t_vec3f txtr_crd)
{
	t_vec2	draw_start;
	float	vrt_offs;

	vrt_offs = get_prspctive_offset(md, ray->distance, sprite);
	draw_start = get_v2(ray->index, vrt_offs - (sprite->pos.z / ray->distance));
	draw_sprite_slice(md, sprite, draw_start, txtr_crd);
}

void	draw_sprite(t_md *md, t_ray *ray, t_hit_data hit_data)
{
	t_vec3f	texture_coord;
	t_ent	*sprite;
	float	sprt_scrn_width;
	float	nrm_dst;
	float	scale_factor;

	sprite = hit_data.hit;
	sprite->in_screen ++;
	ray->distance = maxf(0.1, hit_data.dist_at_e);
	scale_factor = sprite->frame->size.y / md->e_sizes[nt_mob].y;
	nrm_dst = (hit_data.dist_at_e / 2) * scale_factor;
	sprt_scrn_width = (md->win_size.y * sprite->frame->size.y) / nrm_dst;
	texture_coord.x = sprite->row_draw_index;
	texture_coord.y = sprt_scrn_width;
	draw_sprite_pxl(md, ray, sprite, texture_coord);
	md->texture_accumulator += sprite->size.x / sprt_scrn_width;
	while (md->texture_accumulator >= 1.0f)
	{
		sprite->row_draw_index += 1;
		md->texture_accumulator -= 1.0f;
	}
}
