/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INPUT_MOUSE.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:57:28 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/18 16:48:52 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

int	mouse_event_handler(int button, int x, int y, void *param)
{
	t_md	*md;

	md = (t_md *)param;
	md->mouse_pressed = button;
	return (0);
}

int	mouse_release_handler(int button, int x, int y, void *param)
{
	t_md	*md;

	(void)x;
	(void)y;
	(void)button;
	md = (t_md *)param;
	md->mouse_click = md->mouse_pressed;
	md->mouse_pressed = MOUSE_RELEASE;
	return (0);
}

void	wrap_mouse(t_md *md, int delta_x, int delta_y)
{
	t_vec2	block_pos;
	t_vec2	delta_block;

	block_pos = get_v2(md->win_size.x - 200, md->win_size.y - 200);
	mlx_mouse_move(md->win, block_pos.x, block_pos.y);
	delta_block = get_v2(block_pos.x + delta_x, block_pos.y + delta_y);
	md->prev_mouse = block_pos;
	md->mouse_world_pos = block_pos;
}

int	mouse_motion_handler(int x, int y, void *param)
{
	t_vec3	grid_pos;
	t_md	*md;
	t_vec2	delta;

	md = (t_md *)param;
	if (!md->mouse_focus)
	{
		mlx_mouse_move(md->win, md->win_size.x / 2, md->win_size.y / 2);
		x = md->win_size.x / 2;
		y = md->win_size.y / 2;
		md->mouse_focus = 1;
	}
	md->mouse_delta.x = (x - md->prev_mouse.x);
    md->mouse_delta.y = (y - md->prev_mouse.y);
	md->mouse_real.x = x;
	md->mouse_real.y = y;
	if (md->menu.active)
		return (0);
	md->mouse_pos.x += md->mouse_delta.x * MOUSE_SENSITIVITY;
	md->mouse_pos.y += md->mouse_delta.y * MOUSE_SENSITIVITY;
	md->prev_mouse.x = x;
	md->prev_mouse.y = y;
	grid_pos = get_grid_pos(md, get_v3(md->mouse_pos.x, md->mouse_pos.y, 0));
	md->mouse_grid_pos = get_v2((grid_pos.x + md->cam_ofst.x) / md->t_len, \
		(grid_pos.y + md->cam_ofst.y) / md->t_len);
	if (x < 0 || x > md->win_size.x || y < 0 || y > md->win_size.y)
		wrap_mouse(md, md->mouse_delta.x, md->mouse_delta.y);
	md->mouse_focus = 1;
	return (0);
}

int	update_mouse(t_md *md)
{
	md->mouse_world_pos = get_v2(md->mouse_pos.x + \
		md->cam_ofst.x, md->mouse_pos.y + md->cam_ofst.y);
	return (cmp_vec2(md->prev_mouse, get_v2((int)md->mouse_pos.x, (int)md->mouse_pos.y)));
}
