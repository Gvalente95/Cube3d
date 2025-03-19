/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INPUT.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 18:33:59 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/18 16:52:57 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

int	handle_key_press(int keycode, t_md *md)
{
	if (keycode == 65505)
		keycode = SHIFT_KEY;
	if (keycode < 0 || keycode > 512)
		return (0);
	md->key_clicked = keycode;
	md->key_prs[keycode] = 1;
	return (0);
}

int	handle_key_release(int keycode, t_md *md)
{
	if (keycode == 65505)
		keycode = SHIFT_KEY;
	printf("keycode : %d\n", keycode);
	if (keycode >= 0 && keycode < 512)
		md->key_prs[keycode] = 0;
	md->key_clicked = -1;
	return (0);
}

void	reset_mlx_values(t_md *md)
{
	md->key_clicked = -1;
	md->mouse_click = 0;
	if (md->mouse_pressed == MOUSE_RELEASE)
		md->mouse_pressed = 0;
	md->mouse_delta = get_v2(0, 0);
}

int	close_window(t_md *md)
{
	free_md(md, 1);
	exit(0);
	return (0);
}
