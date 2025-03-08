/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FREE_a.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 04:32:24 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/07 00:08:11 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

void	free_vec2_array(t_vec2 **array)
{
	int	i;

	if (!array)
		return ;
	i = -1;
	while(array[++i])
		free(array[i]);
	free(array);
}

void	free_vec3_array(t_vec3 **array)
{
	int	i;

	if (!array)
		return ;
	i = -1;
	while(array[++i])
		free(array[i]);
	free(array);
}

void	free_vec4_array(t_vec4 **array)
{
	int	i;

	if (!array)
		return ;
	i = -1;
	while(array[++i])
		free(array[i]);
	free(array);
}

int	free_images(t_md *md, void ***images)
{
	int	i;
	int	free_count;

	i = 0;
	free_count = 0;
	if (!images || !*images)
		return (0);
	while ((*images)[i])
	{
		free_count += mlx_destroy_image(md->mlx, (*images)[i]);
		(*images)[i] = NULL;
		i++;
	}
	free(*images);
	*images = NULL;
	return (free_count + 1);
}

int	free_ent(t_ent **ent)
{
	int	free_count;

	free_count = 0;
	if (!ent || !*ent)
		return (0);
	if ((*ent)->audio)
		stop_sound((*ent)->audio);
	free(*ent);
	*ent = NULL;
	return (free_count + 1);
}

int	free_ents(t_ent ***ents)
{
	int	freed_count;
	int	i;

	freed_count = 0;
	i = 0;
	if (!ents || !*ents)
		return (0);
	while ((*ents)[i])
	{
		freed_count += free_ent(&(*ents)[i]);
		(*ents)[i] = NULL;
		i++;
	}
	free(*ents);
	*ents = NULL;
	return (freed_count + i + 1);
}

int	free_md(t_md *md, int quit)
{
	int	free_count;

	if (is_audio_playing(md->bgrnd_au))
		stop_sound(md->bgrnd_au);
	if (is_audio_playing(md->bgrnd_mus))
		stop_sound(md->bgrnd_mus);
	if (!md->images || md->images_len <= 0)
		return (0);
	free_count = 0;
	free_count += mlx_destroy_image(md->mlx, md->bgrnd_img);
	free_count += free_ents(&md->images);
	if (quit)
		return (free_md2(md, free_count));
	else
		return (free_count);
}
