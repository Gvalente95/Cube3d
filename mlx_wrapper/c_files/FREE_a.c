/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FREE_a.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 04:32:24 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/12 01:20:51 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

int	free_vec2_array(t_vec2 **array)
{
	int	i;

	if (!array)
		return (printf("tried to free null v2\n"), 0);
	i = -1;
	while(array[++i])
		free(array[i]);
	free(array);
	return (i + 1);
}

int	free_vec3_array(t_vec3 **array)
{
	int	i;

	if (!array)
		return (printf("tried to free null v3\n"), 0);
	i = -1;
	while(array[++i])
		free(array[i]);
	free(array);
	return (i + 1);
}

int	free_vec4_array(t_vec4 **array)
{
	int	i;

	if (!array)
		return (printf("tried to free null v4\n"), 0);
	i = -1;
	while(array[++i])
		free(array[i]);
	free(array);
	return (i + 1);
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
		free_am += free_images(md, md->e_frms[i], "e_frms");
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
	free_count += mlx_destroy_image(md->mlx, md->bgrnd_img) + 1;
	free_count += mlx_destroy_image(md->mlx, md->mmap.bgrnd) + 1;
	free_count += mlx_destroy_image(md->mlx, md->mmap.plr_icon) + 1;
	free_count += mlx_destroy_image(md->mlx, md->center) + 1;
	free_count += mlx_destroy_image(md->mlx, md->sky) + 1;
	free_count += mlx_destroy_image(md->mlx, md->floor) + 1;
	free_count += free_e_frms(md);
	free_count += free_images(md, (void ***)&md->txtr_2d, "txtr_2d");
	free_count += free_images(md, (void ***)&md->wall_txtr, "wall_txtr");
	free_count += free_images(md, (void ***)&md->wall_txtr_2d, "wall_txtr_2d");
	if (!quit || md->init_steps == 1)
		return (free_count);
	return (free_md2(md, free_count));
}
