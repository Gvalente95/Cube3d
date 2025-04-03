/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_draw_ents.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 19:44:01 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/03 21:33:48 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	get_ent_screen_x(t_md *md, t_ent *e)
{
	t_vec2f	to_ent;
	float	dist;
	t_vec2f	plane;
	float	fov_scale;
	t_vec3f	cam;

	dist = sqrtf(e->pos.x * e->pos.x + e->pos.y * e->pos.y);
	to_ent = get_v2f(e->pos.x - md->plr.pos.x, e->pos.y - md->plr.pos.y);
	to_ent.x /= dist;
	to_ent.y /= dist;
	plane = get_v2f(-md->plr.dir.y, md->plr.dir.x);
	fov_scale = tanf((md->prm.fov * 0.8f) * (M_PI / 180.0f));
	plane.x *= fov_scale;
	plane.y *= fov_scale;
	cam.x = to_ent.x * plane.x + to_ent.y * plane.y;
	cam.z = to_ent.x * md->plr.dir.x + to_ent.y * md->plr.dir.y;
	if (cam.z <= 0.01f)
		return (-1);
	cam.y = 0.5f + cam.x / cam.z;
	cam.y = minmaxf(0.0f, 1.0f, cam.y);
	return ((int)(cam.y * md->win_sz.x));
}

void	draw_sprite_thread(t_md *md, t_ent *e, float fogalpha)
{
	t_image	*img;
	int		scale;
	t_vec2	draw_pos;
	float	dist;
	float	scale_factor;

	scale_factor = e->frame->size.y / md->txd.e_sizes[nt_mob].y;
	dist = (maxf(0.1, e->hit_dist) / 2) * scale_factor;
	scale = (md->win_sz.y * e->frame->size.y) / dist;
	draw_pos.x = e->ray_hit_index - scale / 2;
	draw_pos.y = md->win_sz.y / 2 \
		- (md->cam_pos.z * 5) \
		- md->plr_rot.y * 8 \
		- scale / 2 + 10;
	img = copy_image(md, e->frame, v2(scale), -1);
	flush_img(img, md->rgb[RGB_BLACK], fogalpha, 1);
	draw_img(img, md->screen, draw_pos, -1);
	free_image_data(md, img);
	e->in_screen = 1;
}

static int	ent_sort_cmp(void *a, void *b)
{
	const t_ent	*ea = *(t_ent **)a;
	const t_ent	*eb = *(t_ent **)b;

	if (ea->hit_dist < eb->hit_dist)
		return (1);
	if (ea->hit_dist > eb->hit_dist)
		return (-1);
	return (0);
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

void	draw_found_ents(t_md *md, t_threads_manager *mon)
{
	t_dblst	*node;
	t_ent	*e;
	float	fogalpha;

	sort_ent_list_by_distance(&mon->ents_to_draw);
	node = dblst_first(mon->ents_to_draw);
	while (node)
	{
		e = (t_ent *)node->content;
		fogalpha = 1 - minmaxf(0, .95, (e->hit_dist / 1000.0f) * md->fx.fog);
		draw_sprite_thread(md, e, fogalpha);
		node = node->next;
	}
	dblst_clear(&mon->ents_to_draw, NULL);
}
