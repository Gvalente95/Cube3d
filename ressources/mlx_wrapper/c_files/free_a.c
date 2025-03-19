/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FREE_a.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 04:32:24 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 03:19:46 by giuliovalen      ###   ########.fr       */
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

int	free_md(t_md *md, int quit)
{
	int	free_count;

	if (is_audio_playing(md->bgrnd_au))
		stop_sound(md->bgrnd_au);
	if (is_audio_playing(md->bgrnd_mus))
		stop_sound(md->bgrnd_mus);
	if (!md->init_steps || !md->map.buffer)
		return (0);
	free_count = 0;
	free(md->map.buffer);
	free_count += free_image_data(md, md->screen);
	free_count += free_image_data(md, md->hud.sky);
	free_count += free_image_data(md, md->hud.floor);
	free_count += free_image_data(md, md->hud.base_sky);
	free_count += free_image_data(md, md->hud.base_floor);
	free_count += free_image_data(md, md->hud.lock_x_icon);
	free_count += free_image_data(md, md->hud.lock_y_icon);
	free_count += free_image_data(md, md->center);
	free_count += free_image_data(md, md->mmap.bg);
	free_count += free_image_data(md, md->mmap.img);
	free_count += free_images_data(md, md->wall_img, "wall_img");
	free_count += free_images_data(md, md->wall_img2d, "wall_img2d");
	return (free_md2(md, free_count));
}
