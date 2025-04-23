/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_mouse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:57:28 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/23 01:13:27 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	mouse_event_handler(int button, int x, int y, void *param)
{
	t_md	*md;

	md = (t_md *)param;
	md->mouse.pressed = button;
	if (md->mouse.pressed == MOUSE_PRESS)
		md->mouse.click = MOUSE_PRESS;
	return (0);
}

int	mouse_release_handler(int button, int x, int y, void *param)
{
	t_md	*md;

	(void)x;
	(void)y;
	(void)button;
	md = (t_md *)param;
	md->mouse.pressed = MOUSE_RELEASE;
	md->mouse.click = MOUSE_RELEASE;
	return (0);
}

int	mouse_motion_handler(int x, int y, void *param)
{
	t_vec3	grid_pos;
	t_md	*md;
	t_mouse	*msd;

	md = (t_md *)param;
	msd = &md->mouse;
	msd->real = get_v2(x, y);
	msd->delta = get_v2(x - msd->prev.x, y - msd->prev.y);
	msd->prev = get_v2(x, y);
	msd->pos.x += msd->delta.x * MOUSE_SENSITIVITY;
	msd->pos.y += msd->delta.y * MOUSE_SENSITIVITY;
	grid_pos = get_grid_pos(md, get_v3(msd->pos.x, msd->pos.y, 0));
	msd->grid_pos = get_v2((grid_pos.x + md->cam.ofst.x) / md->t_len, \
		(grid_pos.y + md->cam.ofst.y) / md->t_len);
	if (msd->locked && \
		(x < 5 || x > md->win_sz.x - 5 || y < 5 || y > md->win_sz.y - 5))
		wrap_mouse(md, msd->delta.x, msd->delta.y);
	return (msd->focus = 1, 0);
}

int	update_mouse(t_md *md)
{
	t_mouse	*msd;

	msd = &md->mouse;
	msd->world = get_v2(msd->pos.x + \
		md->cam.ofst.x, msd->pos.y + md->cam.ofst.y);
	return (cmp_vec2(msd->prev, get_v2((int)msd->pos.x, (int)msd->pos.y)));
}
