/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_a.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 04:32:24 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/14 02:20:38 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

int	free_image_data(t_md *md, t_image *img_data)
{
	int	res;

	if (!img_data)
		return (0);
	res = 0;
	if (img_data->img)
		res += mlx_destroy_image(md->mlx, img_data->img);
	free(img_data);
	return (res + 1);
}

int	free_images_data(t_md *md, t_image **images, char *label)
{
	int	i;
	int	free_count;

	if (!images)
	{
		if (label)
			printf("tried to free \"%s\" img_datas\n", label);
		return (0);
	}
	free_count = 0;
	i = -1;
	while (images[++i])
		free_count += free_image_data(md, images[i]);
	free(images);
	return (free_count + 1);
}

int	free_images(t_md *md, void ***images, char *label)
{
	int	i;
	int	free_count;

	i = 0;
	free_count = 0;
	if (!images || !*images)
	{
		if (label)
			printf("tried to free \"%s\" images\n", label);
		return (0);
	}
	while ((*images)[i])
	{
		free_count += mlx_destroy_image(md->mlx, (*images)[i]) + 1;
		(*images)[i] = NULL;
		i++;
	}
	free(*images);
	*images = NULL;
	return (free_count + 1);
}

int	free_e_frms(t_md *md)
{
	int	i;
	int	free_am;

	if (!md->e_frms)
		return (ft_printf("no e_frms to free"), 0);
	free_am = 0;
	i = -1;
	while (md->e_frms[++i])
		free_am += free_images_data(md, *md->e_frms[i], "e_frms");
	free(md->e_frms);
	return (free_am);
}

int	free_md(t_md *md, int quit)
{
	int	free_count;

	if (is_audio_playing(md->bgrnd_au))
		stop_sound(md->bgrnd_au);
	if (is_audio_playing(md->bgrnd_mus))
		stop_sound(md->bgrnd_mus);
	if (!md->init_steps)
		return (0);
	free_count = 0;
	free(md->map.buffer);
	free_count += free_image_data(md, md->screen.buffer);
	free_count += free_image_data(md, md->screen.sky);
	free_count += free_image_data(md, md->screen.floor);
	free_count += free_image_data(md, md->center);
	free_count += free_image_data(md, md->mmap.bgrnd);
	free_count += free_image_data(md, md->mmap.plr_icon);
	free_count += free_images_data(md, md->txtr_2d, "txtr_2d");
	free_count += free_images_data(md, md->wall_txtr, "wall_txtr");
	free_count += free_images_data(md, md->wall_txtr_2d, "wall_txtr_2d");
	free_count += free_e_frms(md);
	free_count += mlx_destroy_window(md->mlx, md->win) + 1;
	free_count += free_void(md->mlx);
	if (!quit || md->init_steps == 1)
		return (free_count);
	return (free_md2(md, free_count));
}
