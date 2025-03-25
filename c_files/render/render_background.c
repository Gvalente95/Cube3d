/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_background.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:45:20 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 14:14:42 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	draw_sky(t_md *md, t_image *bufr, t_vec3 *sky_pos)
{
	int		i;
	t_vec2	win_sz;
	t_vec3	trimm;
	t_vec2	pos;
	t_image	*to_draw;

	win_sz = md->win_size;
	trimm = get_v3(md->win_size.x, md->hud.floor_start, md->hud.bgr_color);
	i = -1;
	while (++i < 6)
	{
		pos = get_v2(sky_pos[i].x, sky_pos[i].y);
		if (pos.y < -win_sz.y || pos.y > md->hud.floor_start || \
			pos.y < -win_sz.x || pos.x > win_sz.x)
			continue ;
		to_draw = md->hud.sky;
		if (sky_pos[i].z)
			to_draw = md->hud.sky_flipy;
		draw_trimm_img(to_draw, bufr, pos, trimm);
	}
}

static int	render_sky(t_md *md, t_image *bufr)
{
	t_vec3	sky_pos[6];
	t_vec3f	speed;
	t_vec3f	offst;
	t_vec2	scrl;
	t_vec2	win_sz;

	speed = get_v3f(3, 1, 0);
	win_sz = md->win_size;
	offst.x = win_sz.x * speed.x;
	offst.y = win_sz.y * speed.y;
	scrl.x = fmod((((md->plr_rot.x + 180.0)) / 360.0) * offst.x, win_sz.x);
	scrl.y = fmod((((md->plr_rot.y + 90.0)) / 180.0) * offst.y, win_sz.y);
	sky_pos[0] = get_v3(-scrl.x, -scrl.y, 0);
	sky_pos[1] = get_v3(-scrl.x, win_sz.y - scrl.y, 1);
	sky_pos[2] = get_v3(win_sz.x - scrl.x, -scrl.y, 0);
	sky_pos[3] = get_v3(win_sz.x - scrl.x, -win_sz.y - scrl.y, 0);
	sky_pos[4] = get_v3(-scrl.x, -scrl.y + win_sz.y, 1);
	sky_pos[5] = get_v3(win_sz.x - scrl.x, -scrl.y + win_sz.y, 1);
	draw_sky(md, bufr, sky_pos);
	return (1);
}

void	draw_floor_texture(t_md *md, t_image *tex, t_vec2 win_sz)
{
	t_vec2		pos;
	t_vec3f		dir;
	t_vec3f		plane;
	t_vec3f		ray_dir_0;
	t_vec3f		ray_dir_1;
	t_vec3f		floor, step;
	t_vec2		tex_pos;
	float		fov_scale;
	float		row_dist;
	float		p;
	float		angle_rad;
	int			color;

	// 1. Direction du joueur à partir de rotation.x
	angle_rad = md->plr_rot.x * (M_PI / 180.0f);
	dir = (t_vec3f){cosf(angle_rad), sinf(angle_rad), 0};
	fov_scale = tanf(md->prm.fov * 0.5f * M_PI / 180.0f);
	plane = (t_vec3f){dir.y * fov_scale, dir.x * fov_scale, 0};
	ray_dir_0 = (t_vec3f){dir.x - plane.x, dir.y - plane.y, 0};
	ray_dir_1 = (t_vec3f){dir.x + plane.x, dir.y + plane.y, 0};
	// 4. Parcours vertical (ligne par ligne)
	for (pos.y = md->hud.floor_start; pos.y < win_sz.y; pos.y++)
	{
		p = pos.y - md->hud.floor_start;
		if (fabsf(p) < 1e-4f)
			p = 1e-4f;
		row_dist = (0.5f * win_sz.y) / p;
		// Calcul du pas horizontal (entre chaque colonne)
		step.x = row_dist * (ray_dir_1.x - ray_dir_0.x) / win_sz.x;
		step.y = row_dist * (ray_dir_1.y - ray_dir_0.y) / win_sz.x;
		// Position sol initiale pour cette ligne
		floor.x = md->plr.pos.x + row_dist * ray_dir_0.x;
		floor.y = md->plr.pos.y + row_dist * ray_dir_0.y;
		// 5. Parcours horizontal (colonne par colonne)
		for (pos.x = 0; pos.x < win_sz.x; pos.x++)
		{
			int index = pos.y * win_sz.x + pos.x;
			if (md->screen->src[index] != md->hud.bgr_color)
				continue ;
			// Coordonnées dans la texture
			tex_pos.x = (int)(floor.x * tex->size.x + 0.5f) % tex->size.x;
			tex_pos.y = (int)(floor.y * tex->size.y + 0.5f) % tex->size.y;
			if (tex_pos.x < 0)
				tex_pos.x += tex->size.x;
			if (tex_pos.y < 0)
				tex_pos.y += tex->size.y;
			// Lecture couleur et dessin
			color = tex->src[tex_pos.y * tex->size.x + tex_pos.x];
			draw_pixel(md->screen, pos, color, 1.0f);
			// Avancer sur l’axe horizontal
			floor = add_vec3f(floor, step);
		}
	}
}

void	render_2d_floor(t_md *md)
{
	t_vec2	pos[9];
	t_vec2	centr;
	t_vec2	win_sz;
	t_vec3f	cam;
	int		i;

	win_sz = get_v2(md->win_size.x * .5, md->win_size.y * .5);
	cam = get_v3f(md->cam_ofst.x / md->t_len, md->cam_ofst.y / md->t_len, 0);
	centr.x = fmod(win_sz.x - md->txd.size_2d - (cam.x * md->txd.size_2d), \
		md->win_size.x);
	centr.y = fmod(win_sz.y - md->txd.size_2d - (cam.y * md->txd.size_2d), \
		md->win_size.y);
	pos[0] = get_v2(centr.x, centr.y + md->win_size.y);
	pos[1] = get_v2(centr.x, centr.y - md->win_size.y);
	pos[2] = get_v2(centr.x + md->win_size.x, centr.y - md->win_size.y);
	pos[3] = get_v2(centr.x + md->win_size.x, centr.y + md->win_size.y);
	pos[4] = get_v2(centr.x - md->win_size.x, centr.y - md->win_size.y);
	pos[5] = get_v2(centr.x - md->win_size.x, centr.y + md->win_size.y);
	pos[6] = get_v2(centr.x + md->win_size.x, centr.y);
	pos[7] = get_v2(centr.x - md->win_size.x, centr.y);
	pos[8] = centr;
	i = -1;
	while (++i < 9)
		if (v2_touch(pos[i], md->win_size, v2(0), md->win_size))
			draw_img(md->hud.floor, md->screen, pos[i], md->hud.bgr_color);
}

void	render_background(t_md *md)
{
	if (!md->prm.real_mode)
		render_2d_floor(md);
	else if (!md->hud.active_background)
	{
		draw_trimm_img(md->hud.base_sky, md->screen, get_v2(0, 0), \
		get_v3(md->win_size.x, md->hud.floor_start, md->hud.bgr_color));
		draw_img(md->hud.base_floor, md->screen, \
			get_v2(0, md->hud.floor_start), md->hud.bgr_color);
	}
	else
	{
		render_sky(md, md->screen);
		//draw_floor_texture(md, md->hud.floor, md->win_size);
		draw_img(md->hud.base_floor, md->screen, \
			get_v2(0, md->hud.floor_start), md->hud.bgr_color);
	}
}
