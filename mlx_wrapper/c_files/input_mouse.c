/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INPUT_MOUSE.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:57:28 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/14 16:11:27 by giuliovalen      ###   ########.fr       */
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
	md->mouse_clicked = md->mouse_pressed;
	md->mouse_pressed = MOUSE_RELEASE;
	return (0);
}

int	mouse_motion_handler(int x, int y, void *param)
{
	t_vec3	grid_pos;
	t_md	*md;
	t_vec2	delta;

	md = (t_md *)param;
	delta.x = x - md->prev_mouse.x;
    delta.y = y - md->prev_mouse.y;
	md->mouse_real.x = x;
	md->mouse_real.y = y;
	md->mouse_pos.x += delta.x;
	md->mouse_pos.y += delta.y;
	md->prev_mouse.x = x;
	md->prev_mouse.y = y;
	grid_pos = get_grid_pos(md, v2_to_v3(md->mouse_pos));
	md->mouse_grid_pos = get_v2((grid_pos.x + md->cam_ofst.x) / md->t_len, \
		(grid_pos.y + md->cam_ofst.y) / md->t_len);
	if (!md->mouse_focus)
		printf("mouse set succesfully\n");
	md->mouse_focus = 1;
	return (0);
}

void	wrap_mouse(t_md *md)
{
	t_vec2	block_pos;

	block_pos = get_v2(md->win_size.x - 20, md->win_size.y - 20);
	mlx_mouse_move(md->win, block_pos.x, block_pos.y);
	md->prev_mouse = block_pos;
	md->mouse_world_pos = block_pos;
}


int	update_mouse(t_md *md)
{
	if (md->lock_mouse && md->time % 2 == 0 && md->mouse_focus)
		wrap_mouse(md);
	md->mouse_world_pos = get_v2(md->mouse_pos.x + \
		md->cam_ofst.x, md->mouse_pos.y + md->cam_ofst.y);
	return (cmp_vec2(md->prev_mouse, md->mouse_pos));
}
