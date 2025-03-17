/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INIT_WRAPPER.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 20:39:27 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/15 11:39:54 by giuliovalen      ###   ########.fr       */
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
	return (1);
}

int	init_md(t_md *md)
{
	ft_memset(md->key_prs, 0, 512);
	md->win_size = get_v2(0, 0);
	md->cam_ofst = get_v3f(0, 0, 0);
	md->arrow_rot = get_v2(0, 0);
	md->input_mov = get_v3f(0, 0, 0);
	md->wrd_mv_offst = get_v3f(0, 0, 0);
	md->plr_wrd_mv = get_v3f(0, 0, 0);
	md->input_mov = get_v3f(0, 0, 0);
	md->mlx = mlx_init();
	md->txtr_2d = NULL;
	md->win = NULL;
	md->particles = NULL;
	md->wall_img = NULL;
	md->particles_alive = 0;
	md->lock_mouse = 1;
	md->mouse_pressed = 0;
	md->mouse_clicked = 0;
	md->key_clicked = -1;
	md->t_len = 60;
	md->init_steps = 0;
	md->txt_scale = 10;
	md->mouse_hide = 1;
	return (1);
}

void	init_os_params(t_md *md)
{
	md->mlx_put = mlx_put_image_to_window;
	md->mlx_make = mlx_png_file_to_image;
	md->is_linux = 0;
	ft_strlcpy(md->image_dir, IMG_PATH, 10);
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
	ft_strlcpy(md->image_dir, IMG_PATH, 10);
	ft_strlcpy(md->img_format, IMG_FORMAT, 10);
	ft_strlcpy(md->img_format, IMG_FORMAT, 10);
}

void	init_wrapper(t_md *md, t_vec2 win_size, char *win_name, int row_amount)
{
	init_md(md);
	init_os_params(md);
	start_timer(&md->timer.game_start);
	init_screen(md, win_size, row_amount, win_name);
	init_timer(md, &md->timer);
	// if (md->is_linux)
	// {
	// 	mlx_hook(md->win, KeyPress, KeyPressMask, handle_key_press, md);
	// 	mlx_hook(md->win, KeyRelease, KeyReleaseMask, handle_key_release, md);
	// 	mlx_hook(md->win, DestroyNotify, StructureNotifyMask, close_window, md);
	// }
	//else
	{
		mlx_hook(md->win, 2, 0, handle_key_press, md);
		mlx_hook(md->win, 3, 0, handle_key_release, md);
		mlx_hook(md->win, 17, 0, close_window, md);
	}
}
