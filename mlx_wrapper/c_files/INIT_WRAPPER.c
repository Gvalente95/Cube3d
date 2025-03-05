/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INIT_WRAPPER.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 20:39:27 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 12:17:59 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

int	init_md(t_md *md)
{
	memset(md->key_prs, 0, 512);
	md->mlx = mlx_init();
	md->bgrnd = NULL;
	md->key = NULL;
	md->exit = NULL;
	md->images = NULL;
	md->win = NULL;
	md->all_images = NULL;
	md->env_images = NULL;
	md->bgrnd_img = NULL;
	md->particles = NULL;
	md->star_icon = NULL;
	md->wall_txtr = NULL;
	md->hpbar = NULL;
	md->index = 0;
	md->is_typing = 0;
	md->mouse_pressed = 0;
	md->mouse_clicked = 0;
	md->key_clicked = -1;
	md->images_len = 0;
	md->win_size = get_v2(0, 0);
	md->cam_offset = get_v3f(0, 0, 0);
	md->coins_left = 0;
	md->jump_timer = 0;
	return (md->coin_au_timer = 0, md->t_len = CELL_SIZE, 1);
}

void	init_wrapper(t_md *md, t_vec2 win_size, char *win_name, int row_amount)
{
	start_timer(&md->timer.game_start);
	init_md(md);
	md->win = mlx_new_window(md->mlx, win_size.x, win_size.y, win_name);
	md->win_size = get_v2(win_size.x, win_size.y);
	md->t_len = win_size.x / row_amount;
	mlx_hook(md->win, KEY_PRESS, 0, handle_key_press, md);
	mlx_hook(md->win, KEY_RELEASE, 0, handle_key_release, md);
	mlx_hook(md->win, 17, 0, close_window, md);
}
