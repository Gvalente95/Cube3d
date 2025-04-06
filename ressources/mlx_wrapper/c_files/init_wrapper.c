/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_wrapper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 20:39:27 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/04 11:37:56 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

unsigned int	get_r_seed(void)
{
	int				fd;
	unsigned int	seed;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0 || read(fd, &seed, sizeof(seed)) != sizeof(seed))
		seed = (unsigned int)time(NULL);
	close(fd);
	return (seed);
}

int	init_screen(t_md *md, t_vec2 win_sz, int resolution, char *win_name)
{
	void	*screen_image;

	md->win = mlx_new_window(md->mlx, win_sz.x, win_sz.y, win_name);
	md->win_sz = get_v2(win_sz.x, win_sz.y);
	md->t_len = win_sz.x / resolution;
	md->prm.resolution = resolution;
	return (1);
}

int	init_md(t_md *md)
{
	int	i;

	i = -1;
	while (++i < 65536)
		md->key_prs[i] = 0;
	md->win_sz = v2(0);
	md->cam.ofst = v3f(0);
	md->cam.input_mov = v3f(0);
	md->cam.wrd_mv_offst = v3f(0);
	md->cam.plr_wrd_mv = v3f(0);
	md->cam.input_mov = v3f(0);
	md->mlx = mlx_init();
	md->win = NULL;
	md->txd.wall_img = NULL;
	md->mouse.locked = 1;
	md->mouse.pressed = 0;
	md->mouse.click = 0;
	md->key_clicked = -1;
	md->t_len = 60;
	md->init_steps = 0;
	md->prm.txt_sc = 14;
	md->mouse.hide = 1;
	return (1);
}

void	init_os_params(t_md *md)
{
	md->mlx_put = mlx_put_image_to_window;
	md->mlx_make = mlx_xpm_file_to_image;
	md->is_linux = 0;
	if (LIN)
		md->is_linux = 1;
	ft_strlcpy(md->base_map_path, "Randomap.cub", 20);
	printf("wrapper LIN = %d\n", LIN);
}

void	init_wrapper(t_md *md, t_vec2 win_sz, char *win_name, int resolution)
{
	init_md(md);
	init_os_params(md);
	start_timer(&md->timer.game_start);
	init_screen(md, win_sz, resolution, win_name);
	init_timer(md, &md->timer);
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
	md->r_seed = get_r_seed();
}
