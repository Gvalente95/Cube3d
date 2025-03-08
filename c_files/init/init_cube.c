/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cube.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:36:33 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/07 16:12:35 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	init_cursor(t_md *md)
{
	t_vec2	cursor_size;

	mlx_mouse_hide();
	md->cursor = mlx_png_file_to_image(md->mlx, \
		CURSOR_SPR_PATH, &cursor_size.x, &cursor_size.y);
	md->cursor = scale_img(md, md->cursor, \
		&cursor_size, get_v2(30, 30));
	md->cursor_detect = mlx_png_file_to_image(md->mlx, \
		CURSOR_DETECT_PATH, &cursor_size.x, &cursor_size.y);
	md->cursor_detect = scale_img(md, md->cursor_detect, \
		&cursor_size, get_v2(30, 30));
	md->cursor_grab = mlx_png_file_to_image(md->mlx, \
		CURSOR_GRAB_PATH, &cursor_size.x, &cursor_size.y);
	md->cursor_grab = scale_img(md, md->cursor_grab, \
		&cursor_size, get_v2(30, 30));
	md->mouse_pressed = 0;
	md->mouse_clicked = 0;
	mlx_mouse_hook(md->win, mouse_event_handler, md);
	mlx_hook(md->win, 5, 0, mouse_release_handler, md);
	mlx_hook(md->win, 6, 0, mouse_motion_handler, md);
}

static void	init_minimap(t_md *md)
{
	t_dblist	*node;
	t_ent		*e;
	t_vec2		pos;
	int			ic_len;

	md->mmap.active = 0;
	ic_len = md->win_size.x / 100;
	md->mmap.size = get_v2(md->map.size.x * ic_len, md->map.size.y * ic_len);
	md->mmap.img = mlx_new_image(md->mlx, md->mmap.size.x, md->mmap.size.y);
	set_img_color(md->mmap.img, md->mmap.size, COLOR_BLUE, 0.5);
	node = md->entities;
	while (node)
	{
		e = (t_ent *)node->content;
		pos = get_v2(e->coord_pos.x, e->coord_pos.y);
		color_img(md->mmap.img, md->mmap.size, str_to_color("255,50,50"), \
			get_v4(ic_len * e->coord_pos.x + 1, \
			ic_len * pos.y + 1, \
			ic_len * pos.x + ic_len - 1, \
			ic_len * pos.y + ic_len - 1));
		node = node->next;
	}
	md->mmap.icon_scale = ic_len;
}

static void	init_center_pointer(t_md *md)
{
	t_vec2	pos;

	md->center = add_img("mlx_png/center.png", &pos.x, &pos.y, md);
	md->center = scale_abs_img(md, md->center, &pos, get_v2(20, 20));
}

int	init_cube(t_md *md, char *file_arg, int start_debug)
{
	init_wrapper(md, get_v2(RAYS_AMOUNT, RAYS_AMOUNT), "Cube3d", 5);
	md->ray_mode = 1;
	md->debug_mode = start_debug;
	init_map(md, file_arg);
	init_entities(md);
	init_cursor(md);
	init_center_pointer(md);
	init_minimap(md);
	if (md->debug_mode)
		show_init_information(md);
	return (0);
}
