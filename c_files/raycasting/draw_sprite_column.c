/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprite_column.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 04:30:37 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/08 02:41:03 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	paint_ent(t_md *md, t_ent *e, t_vec2 txtr_coord)
{
	int			action;
	int			frame_i;
	int			dmg;

	if (e->type != nt_mob)
		return ;
	dmg = md->hud.wpn_index + 1 * (1 + (txtr_coord.y < e->frame->size.y / 2));
	txtr_coord = get_v2(\
		e->frame->size.x / 2 + r_range(-5, 5), \
		r_range(20, e->frame->size.y - 20));
	action = -1;
	while (++action < ENT_ACTION_LEN)
	{
		if (action == m_death)
			txtr_coord.y += e->frame->size.y * .5;
		frame_i = -1;
		while (e->anim[action][++frame_i])
			draw_blood(md, e->anim[action][frame_i], txtr_coord, _RED);
	}
	e->was_hit = 1;
	e->hp -= dmg;
	if (!e->hp)
		e->frame_index = 0;
}

int	get_prspctive_offset(t_md *md, float ray_dst, t_ent *e)
{
	float	pitch_factor;
	int		pitch_offset;
	float	vrt_offst;

	pitch_factor = tanf(md->cam.rot.y * (M_PI / 180.0f));
	vrt_offst = (md->cam.pos.z - e->pos.z) * md->win_sz.y / (ray_dst + 1.0f);
	pitch_offset = (-pitch_factor * md->win_sz.y / 2) - vrt_offst;
	return (pitch_offset);
}

void	draw_sprite_slice(t_md *md, t_ent *ent, t_vec2 winp, t_vec3f crd)
{
	t_vec2	y;
	t_vec2	pxl;
	t_vec2	wins;
	int		img_y;

	wins = md->win_sz;
	y = (t_vec2){(wins.y / 2 - crd.y / 2) - 1, (wins.y / 2 + crd.y / 2)};
	crd.x = minmax(0, ent->frame->size.x - 1, crd.x);
	while (++y.x < y.y)
	{
		img_y = (y.x - (wins.y / 2 - crd.y / 2)) * (ent->frame->size.y / crd.y);
		if (img_y < 0 || img_y >= ent->frame->size.y)
			continue ;
		pxl.y = (img_y * (ent->frame->size_line / 4)) + (int)crd.x;
		pxl.x = *(ent->frame->src + pxl.y);
		if (md->var && winp.x == wins.x / 2 && \
			y.x + winp.y - md->cam.pos.z == wins.y / 2)
			paint_ent(md, ent, get_v2((int)crd.x, (int)crd.x));
		if ((pxl.x >> 24) != 0x00)
			continue ;
		if (ent->was_hit == 2)
			pxl.x = _RED;
		draw_pixel(md->screen, \
			get_v2(winp.x, y.x + winp.y - md->cam.pos.z), pxl.x, -1);
	}
}

void	draw_sprite_pxl(t_md *md, t_ray *ray, \
	t_ent *sprite, float sprt_scrn_width)
{
	t_vec2	draw_start;
	float	vrt_offs;
	t_vec3f	texture_coord;

	md->var = 0;
	if (md->hud.wpn_index != Knife || ray->distance < md->t_len)
		md->var = (md->plr.shot && sprite->hp);
	texture_coord.x = sprite->row_draw_index;
	texture_coord.y = sprt_scrn_width;
	texture_coord.z = 0;
	vrt_offs = get_prspctive_offset(md, ray->distance, sprite);
	draw_start = get_v2(ray->index, vrt_offs - (sprite->pos.z / ray->distance));
	draw_sprite_slice(md, sprite, draw_start, texture_coord);
	md->var = 0;
}

void	draw_sprite(t_md *md, t_ray *ray, t_hit_data hit_data)
{
	t_ent	*sprite;
	float	sprt_scrn_width;
	float	nrm_dst;
	float	scale_factor;

	sprite = hit_data.hit;
	if (!ray->had_door)
		sprite->in_screen++;
	ray->distance = maxf(0.1, hit_data.dist_at_e);
	scale_factor = sprite->frame->size.y / md->txd.e_sizes[nt_mob].y;
	nrm_dst = (hit_data.dist_at_e / 2) * scale_factor;
	sprt_scrn_width = (md->win_sz.y * sprite->frame->size.y) / nrm_dst;
	sprt_scrn_width = maxf(1, sprt_scrn_width);
	if (hit_data.hit->type == nt_door || hit_data.hit->type == nt_wall)
		draw_wall_line(md, hit_data.dist_at_e, hit_data.hit, ray);
	else if (!ray->check_hit)
		draw_sprite_pxl(md, ray, sprite, sprt_scrn_width);
	hit_data.hit->tex_accumulator += sprite->size.x / sprt_scrn_width;
	while (hit_data.hit->tex_accumulator >= 1.0f)
	{
		hit_data.hit->row_draw_index += 1;
		hit_data.hit->tex_accumulator -= 1.0f;
	}
}
