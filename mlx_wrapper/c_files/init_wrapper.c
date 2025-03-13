/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INIT_WRAPPER.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 20:39:27 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/13 20:25:45 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"



int	init_screen(t_md *md, t_vec2 win_size, int row_amount, char *win_name)
{
	void	*screen_image;

	md->win = mlx_new_window(md->mlx, win_size.x, win_size.y, win_name);
	md->win_size = get_v2(win_size.x, win_size.y);
	md->t_len = win_size.x / row_amount;
	md->row_amount = row_amount;
}

int	init_md(t_md *md)
{
	ft_memset(md->key_prs, 0, 512);
	md->mlx = mlx_init();
	md->txtr_2d = NULL;
	md->win = NULL;
	md->particles = NULL;
	md->wall_txtr = NULL;
	md->mouse_pressed = 0;
	md->mouse_clicked = 0;
	md->key_clicked = -1;
	md->win_size = get_v2(0, 0);
	md->cam_ofst = get_v3f(0, 0, 0);
	md->t_len = 60;
	return (1);
}

void	init_os_params(t_md *md)
{
	md->mlx_put = mlx_put_image_to_window;
	md->mlx_make = mlx_png_file_to_image;
	md->is_linux = 0;
	ft_strlcpy(md->img_dir_path, IMG_PATH, 10);
	if (LIN)
	{
		ft_strlcpy(md->base_map_path, "maps/lnx/map.cub", 20);
		md->mlx_make = mlx_xpm_file_to_image;
		printf("OS : linux ");
		md->is_linux = 1;
	}
	else
		ft_strlcpy(md->base_map_path, "maps/mac/map.cub", 20);
	printf("img path: %s format: %s\n", IMG_PATH, IMG_FORMAT);
	printf("wrapper LIN = %d\n", LIN);
	ft_strlcpy(md->img_dir_path, IMG_PATH, 10);
	ft_strlcpy(md->img_format, IMG_FORMAT, 10);
	ft_strlcpy(md->img_format, IMG_FORMAT, 10);
}

void	init_wrapper(t_md *md, t_vec2 win_size, char *win_name, int row_amount)
{
	init_screen(md, win_size, row_amount, win_name);
	start_timer(&md->timer.game_start);
	init_os_params(md);
	init_md(md);
	if (md->is_linux)
	{
		mlx_hook(md->win, KeyPress, KeyPressMask, handle_key_press, md);
		mlx_hook(md->win, KeyRelease, KeyReleaseMask, handle_key_release, md);
		mlx_hook(md->win, DestroyNotify, StructureNotifyMask, close_window, md);
	}
	else
	{
		mlx_hook(md->win, 2, 0, handle_key_press, md);
		mlx_hook(md->win, 3, 0, handle_key_release, md);
		mlx_hook(md->win, 17, 0, close_window, md);
	}
}
