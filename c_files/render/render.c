/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 23:46:39 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/07 16:16:53 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	is_in_screen(t_md *md, t_vec3 pos, t_vec2 size)
{
	return (pos.x > -size.x && pos.x < md->win_size.x + \
		size.x && pos.y > -size.y && pos.y < md->win_size.y + size.y);
}

void	render_ent(t_md *md, t_ent *e)
{
	t_vec3		pos;

	pos = get_v3(e->pos.x - md->cam_ofst.x, e->pos.y - md->cam_ofst.y, 0);
	e->in_screen = is_in_screen(md, pos, e->size);
	if (e->in_screen)
		mlx_put_image_to_window(md->mlx, md->win, e->frame, pos.x, pos.y);
}

void	render_entities(t_md *md)
{
	t_dblist	*node;
	t_ent		*e;

	node = md->entities;
	while (node)
	{
		e = (t_ent *)node->content;
		render_ent(md, e);
		node = node->next;
	}
}

void	render_minimap(t_md *md)
{
	t_vec2	plr_p;
	t_vec2	centr;
	void	*plr;

	centr = get_v2(md->win_size.x - md->mmap.size.x, 0);
	mlx_put_image_to_window(md->mlx, md->win, md->mmap.img, centr.x, centr.y);
	plr_p = get_v2(centr.x, centr.y);
	plr_p.x += (int)md->plr.pos.x / md->mmap.icon_scale;
	plr_p.y += (int)md->plr.pos.y / md->mmap.icon_scale;
	plr = mlx_new_image(md->mlx, md->mmap.icon_scale, md->mmap.icon_scale);
	set_img_color(plr, get_v2(md->mmap.icon_scale, md->mmap.icon_scale), str_to_color("0,255,0"), 1);
	mlx_put_image_to_window(md->mlx, md->win, plr, plr_p.x, plr_p.y);
	mlx_destroy_image(md->mlx, plr);
}

void	render(t_md *md)
{
	mlx_put_image_to_window(md->mlx, md->win, md->sky, 0, -md->win_size.y / 2);
	mlx_put_image_to_window(md->mlx, md->win, md->floor, 0, md->win_size.y / 2);
	render_rays(md, get_v3f(\
		md->plr.pos.x + md->plr.size.x / 2, \
		md->plr.pos.y + md->plr.size.y / 2, \
		md->plr.pos.z));
	if (!md->ray_mode)
	{
		render_entities(md);
		render_ent(md, &md->plr);
	}
	if (md->mmap.active)
		render_minimap(md);
	mlx_put_image_to_window(md->mlx, md->win, md->center, \
			md->win_size.x / 2 - 10, md->win_size.y / 2 - 10);
	render_cursor(md, 0);
	if (md->debug_mode)
		show_update_information(md);
}
