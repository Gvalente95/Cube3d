/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 18:33:59 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/25 15:50:42 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

int	handle_key_press(int keycode, t_md *md)
{
	if (keycode < 0 || keycode > 65536)
		return (0);
	md->key_clicked = keycode;
	md->key_prs[keycode] = 1;
	return (0);
}

int	handle_key_release(int keycode, t_md *md)
{
	printf("keycode : %d\n", keycode);
	if (keycode >= 0 && keycode < 65536)
		md->key_prs[keycode] = 0;
	md->key_clicked = -1;
	return (0);
}

void	reset_mlx_values(t_md *md)
{
	md->key_clicked = -1;
	md->mouse.click = 0;
	if (md->mouse.pressed == MOUSE_RELEASE)
		md->mouse.pressed = 0;
	md->mouse.delta = get_v2(0, 0);
}

int	close_window(t_md *md)
{
	free_md(md, 1);
	exit(0);
	return (0);
}
