/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INPUT_MOUSE.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:57:28 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/07 14:48:52 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

int	mouse_event_handler(int button, int x, int y, void *param)
{
	t_md	*md;

	md = (t_md *)param;
	md->mouse_pressed = button;
	md->mouse_pos.x = x;
	md->mouse_pos.y = y;
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

	md = (t_md *)param;
	md->mouse_pos.x = x;
	md->mouse_pos.y = y;
	grid_pos = get_grid_posf(md, md->mouse_pos);
	md->mouse_grid_pos = get_v2((grid_pos.x + md->cam_ofst.x) / md->t_len, \
		(grid_pos.y + md->cam_ofst.y) / md->t_len);
	return (0);
}

int	update_mouse(t_md *md)
{
	if (md->time % 5 == 0)
		md->mouse_prv_pos = md->mouse_pos;
	md->mouse_world_pos = get_v3f(md->mouse_pos.x + \
		md->cam_ofst.x, md->mouse_pos.y + md->cam_ofst.y, 0);
	return (cmp_vec3f(md->mouse_prv_pos, md->mouse_pos, EPSILON));
}
