/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cube.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:36:33 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 14:18:30 by giuliovalen      ###   ########.fr       */
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

void	init_player(t_md *md)
{
	md->cam_offset = get_v3f(0, 0, 0);
	md->plr.base_pos = get_v3f(0, 0, 0);
	md->plr.start_pos = get_v3f(0, 0, 0);
	md->plr.pos = get_v3f(0, 0, 0);
	md->plr.rot = get_v3f(0, 0, 0);
	md->plr.mov = get_v3f(0, 0, 0);
}

void	init_minimap(t_md *md)
{
	int		i;
	t_vec2	pos;
	t_vec2	mm_size;
	int		icon_len;

	icon_len = md->win_size.x / 100;
	mm_size = get_v2(md->map.size.x * icon_len, md->map.size.y * icon_len);
	md->minimap = mlx_new_image(md->mlx, mm_size.x, mm_size.y);
	pos = get_v2(0, 0);
	i = -1;
	while (md->map.buffer[++i])
	{
		if (md->map.buffer[i] == '1')
			color_img(md->minimap, mm_size, convert_line_to_color("255,50,50"),
			get_v4(icon_len * pos.x + 1, \
			icon_len * pos.y + 1, \
			icon_len * pos.x + icon_len - 1, \
			icon_len * pos.y + icon_len - 1));
		if (md->map.buffer[i] == '\n')
		{
			pos.x = 0;
			pos.y++;
		}
		else
			pos.x++;
	}
	md->mmap_size = get_v2(mm_size.x, mm_size.y);
	md->mmap_ic_size = icon_len;
}

int	init_cube(t_md *md, char *file_arg, int start_debug)
{
	md->debug_mode = start_debug;
	init_map(md, file_arg);
	init_map_data(md);
	init_player(md);
	init_cursor(md);
	md->show_minimap = 0;
	init_minimap(md);
	if (md->debug_mode)
		show_init_information(md);
	return (0);
}
