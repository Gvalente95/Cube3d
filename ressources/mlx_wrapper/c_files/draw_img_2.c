/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_img_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 22:04:37 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/26 16:41:54 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

void	draw_trimm_img(t_image *src, t_image *dst, t_vec2 pos, t_vec3 draw_end)
{
	t_draw_data	draw_d;

	if (!src || !dst || !src->src || !dst->src)
		return ;
	draw_d.src = src;
	draw_d.dst = dst;
	draw_d.pos = get_v2(-1, -1);
	while (++draw_d.pos.y < src->size.y)
	{
		draw_d.src_pos.y = draw_d.pos.y;
		draw_d.dst_pos.y = pos.y + draw_d.pos.y;
		if (draw_d.dst_pos.y >= draw_end.y)
			break ;
		if (draw_d.dst_pos.y < 0 || draw_d.dst_pos.y >= dst->size.y)
			continue ;
		draw_d.pos.x = -1;
		while (++draw_d.pos.x < src->size.x)
		{
			draw_d.src_pos.x = draw_d.pos.x;
			draw_d.dst_pos.x = pos.x + draw_d.pos.x;
			put_pixel_if_visible(&draw_d, draw_end.z, 0, 0);
		}
	}
}

// data = x = color | y = opacity | z = perfectly round toggle (bool)
void	draw_sphere(t_image *dst, t_vec2 pos, t_vec2 draw_size, t_vec3 data)
{
	t_draw_data	dd;
	t_vec2		end_coord;
	t_vec3f		center;
	t_vec3f		rdp;
	t_vec3f		r;

	if (!dst || !dst->src || pos.x < 0 || pos.y < 0)
		return ;
	r = get_v3f(draw_size.x / 2.0f, draw_size.y / 2.0f, 0);
	center = get_v3f(pos.x + r.x, pos.y + r.y, 0);
	end_coord = get_v2(pos.x + draw_size.x, pos.y + draw_size.y);
	dd.src = NULL;
	dd.dst = dst;
	dd.src_pos.y = 0;
	dd.pos.y = pos.y - 1;
	while (++dd.pos.y <= end_coord.y)
	{
		dd.pos.x = pos.x - 1;
		while (++dd.pos.x <= end_coord.x)
		{
			rdp.x = (dd.pos.x - center.x) / r.x;
			rdp.y = (dd.pos.y - center.y) / r.y;
			r.z = sqrt(rdp.x * rdp.x + rdp.y * rdp.y);
			rdp.z = 1.0f - r.z;
			if (!(rdp.z > 0 && (data.z || r_range(0, 20) < rdp.z * 50)))
				continue ;
			dd.dst_pos = dd.pos;
			dd.src_pos.x = data.x;
			put_pixel_if_visible(&dd, -1, 1, rdp.z * (data.y / 10.0f));
		}
	}
}

void	draw_rotated_pixels(t_image *nw, t_image *sr, \
	t_image *onto, t_vec4f ctx)
{
	t_vec4		cn;
	t_draw_data	d;
	t_vec4		p;
	t_vec2		final_pos;

	cn = get_v4(sr->size.x / 2, sr->size.y / 2, nw->size.x / 2, nw->size.y / 2);
	d.dst = nw;
	d.src = sr;
	p.y = -1;
	while (++p.y < nw->size.y)
	{
		p.x = -1;
		while (++p.x < nw->size.x)
		{
			p.z = (p.x - cn.b) * ctx.g - (p.y - cn.a) * ctx.b + cn.r;
			p.w = (p.x - cn.b) * ctx.b + (p.y - cn.a) * ctx.g + cn.g;
			d.dst_pos = get_v2(p.x, p.y);
			d.src_pos = get_v2((int)p.z, (int)p.w);
			if (p.z < 0 || p.z >= sr->size.x || p.w < 0 || p.w >= sr->size.y)
				continue ;
			put_pixel_if_visible(&d, -1, 0, 0);
		}
	}
	final_pos = get_v2(nw->pos.x - (cn.b - cn.r), nw->pos.y - (cn.a - cn.g));
	draw_img(nw, onto, final_pos, -1);
}

//		pos.z = Angle
void	draw_rotated_img(t_md *md, t_image *from, t_image *onto, t_vec3f pos)
{
	t_image	*rotated;
	t_vec2	new_size;
	t_vec4f	ctx;

	if (!md || !from || !onto || !from->src || !onto->src)
		return ;
	ctx.r = pos.z * (M_PI / 180.0f);
	ctx.g = cosf(ctx.r);
	ctx.b = sinf(ctx.r);
	ctx.a = 0;
	new_size.x = fabsf(from->size.x * ctx.g) + fabsf(from->size.y * ctx.b);
	new_size.y = fabsf(from->size.x * ctx.b) + fabsf(from->size.y * ctx.g);
	rotated = init_img(md, new_size, NULL, 0xFF000000);
	if (!rotated)
	{
		printf("rotated img alloc failed\n");
		return ;
	}
	rotated->pos = get_v2(pos.x, pos.y);
	draw_rotated_pixels(rotated, from, onto, ctx);
	free_image_data(md, rotated);
}

void	flush_img(t_image *src, int color, \
	float transp, int ignore_alpha)
{
	int		*img_data;
	int		total_pixels;
	int		i;

	if (!src || !src->src)
		return ;
	if (color == -1)
		color = 0x000000;
	img_data = src->src;
	total_pixels = src->size.x * src->size.y;
	if (transp > 0)
		transp = 1 - transp;
	i = -1;
	while (++i < total_pixels)
	{
		if (ignore_alpha > 0 && (img_data[i] & 0xFF000000) != 0x00000000)
			continue ;
		if (transp < 0)
			img_data[i] = color;
		else
			img_data[i] = blend_color(img_data[i], color, transp);
	}
}
