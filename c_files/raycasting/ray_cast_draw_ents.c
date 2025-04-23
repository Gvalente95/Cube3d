/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_draw_ents.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 19:44:01 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/22 22:57:16 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	get_scale_and_pos(t_md *md, t_ent *e, t_vec2 win_sz, t_vec2 *draw_p)
{
	float	scale_factor;
	float	dist;
	int		scale;

	scale_factor = e->size.y / md->txd.e_scales[e->type];
	dist = (maxf(0.1, e->hit_dist) / 2) * scale_factor;
	scale = minmaxf(5, win_sz.y * 2, (win_sz.y * e->size.y) / dist);
	draw_p->x = e->ray_hit_index - scale / 2;
	draw_p->y = (win_sz.y / 2 - (scale * 0.2f) - \
		(((md->plr.pos.z * .8) * scale_factor)) - (md->cam.rot.y * 8));
	if (e->type == nt_pokemon)
		draw_p->y -= ((float)(e->size.y - \
			(float)md->txd.feet_offsets[e->type]) / e->size.y) * scale;
	if (e->type == nt_pickup)
		draw_p->y += scale * (1.0f - (md->txd.e_scales[e->type] / md->t_len));
	return (scale);
}

void	update_pointed(t_md *md, t_vec2 draw_p, t_vec3 sz_scale, t_ent *e)
{
	const t_vec2	sz = get_v2(sz_scale.x, sz_scale.y);
	const int		scale = sz_scale.z;
	const t_vec2	pkpos = md->inv.pkbl_p;
	int				is_pointed;

	is_pointed = 0;
	if (!md->inv.hold_pkbl)
		is_pointed = v2_bounds(draw_p, sub_vec2(div_v2(md->win_sz, 2), sz), sz);
	else if (e->type == nt_pokemon)
		is_pointed = v2_touch(draw_p, div_v2(sz, 2), pkpos, md->inv.pkbl_sz);
	if (is_pointed)
	{
		md->cam.pointed = e;
		if (md->mouse.click)
			md->txd.last_pointed = e;
	}
	if (md->txd.last_pointed == e)
		md->txd.last_pointed_screen_p = sub_vec2(draw_p, v2(scale * .2));
}

void	draw_sprite_thread(t_md *md, t_ent *e, t_vec2 win_sz, float fogalpha)
{
	t_image		*img;
	t_vec2		sz;
	t_vec2		draw_p;
	const int	scale = get_scale_and_pos(md, e, win_sz, &draw_p);

	img = copy_image(md, e->frame, v2(scale), -1);
	sz = img->size;
	update_pointed(md, draw_p, get_v3(sz.x, sz.y, scale), e);
	if (e->caught)
		flush_img(img, _WHITE, 10, 1);
	else if (fogalpha < .95)
		flush_img(img, md->hud.fog_color, fogalpha, 1);
	draw_sphere(md->screen, get_v2(draw_p.x, draw_p.y + sz.y * .8), \
		get_v2(sz.x, sz.y * .25), get_v3(_BLACK, 3, 1));
	if (md->cam.pointed == e && !e->caught)
		draw_img_contour(md, img, draw_p, (t_vec2){_MAGENT, 8});
	else
		draw_img(img, md->screen, draw_p, -1);
	free_image_data(md, img);
	e->in_screen = 1;
}

static void	sort_ent_list_by_distance(t_dblst **lst)
{
	t_dblst	*node;
	int		count;
	t_ent	**ent_array;
	t_dblst	*new_list;
	int		i;

	new_list = NULL;
	count = 0;
	count = dblst_size(*lst);
	ent_array = malloc(sizeof(t_ent *) * count);
	node = dblst_first(*lst);
	i = -1;
	while (++i < count)
	{
		ent_array[i] = (t_ent *)node->content;
		node = node->next;
	}
	qsort(ent_array, count, sizeof(t_ent *), \
		(int (*)(const void *, const void *))ent_sort_cmp);
	i = -1;
	while (++i < count)
		dblst_add_back(&new_list, dblst_new(ent_array[i]));
	dblst_clear(lst, NULL);
	*lst = new_list;
	free(ent_array);
}

void	draw_found_ents(t_md *md, t_thrd_manager *mon)
{
	t_dblst					*node;
	t_ent					*sel;
	t_ent					*e;
	float					fogalpha;

	sel = md->txd.last_pointed;
	sort_ent_list_by_distance(&mon->ents_to_draw);
	node = dblst_first(mon->ents_to_draw);
	if (md->mouse.click && sel)
		md->txd.last_pointed = NULL;
	if (!sel)
		md->txd.opt_i = -1;
	while (node)
	{
		e = (t_ent *)node->content;
		fogalpha = 1 - minmaxf(0, .95, (e->hit_dist / 1000.0f) * md->fx.fog);
		draw_sprite_thread(md, e, md->win_sz, fogalpha);
		node = node->next;
	}
	if (sel && sel->in_screen && !md->inv.hold_pkbl)
		show_pointed_data(md, md->txd.last_pointed_screen_p, sel);
	dblst_clear(&mon->ents_to_draw, NULL);
}
