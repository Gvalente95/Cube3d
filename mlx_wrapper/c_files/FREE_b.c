/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FREE_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 02:48:05 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 00:03:34 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

int	free_void(void *elem)
{
	if (!elem)
		return (0);
	free(elem);
	elem = NULL;
	return (1);
}

int	free_void_array(void **elements, int i)
{
    int	free_count;

	free_count = 0;
    if (!elements)
        return (0);
	i = 0;
    while (elements[i])
    {
        free(elements[i]);
        elements[i] = NULL;
        free_count++;
        i++;
    }
    free(elements);
    return (free_count + 1);
}

int	free_particles(t_md *md, t_prt ***prts)
{
	int	freed_count;
	int	i;

	freed_count = 0;
	i = 0;
	if (!prts || !*prts)
		return (0);
	while ((*prts)[i])
	{
		if ((*prts)[i]->frame)
		{
			mlx_destroy_image(md->mlx, (*prts)[i]->frame);
			(*prts)[i]->frame = NULL;
			freed_count++;
		}
		free((*prts)[i]);
		(*prts)[i] = NULL;
		freed_count++;
		i++;
	}
	free(*prts);
	*prts = NULL;
	return (freed_count + 1);
}

int	free_player_animations(t_ent *plr, t_md *md)
{
	int	i;
	int	freed_count;

	i = -1;
	freed_count = 0;
	while (++i < ENT_ACTION_LEN)
	{
		if (plr->anim[i])
		{
			freed_count += free_images(md, &plr->anim[i]);
			freed_count += free_images(md, &plr->anim_x[i]);
		}
	}
	return (freed_count);
}

int	free_md2(t_md *md, int free_count)
{
	free_count += free_particles(md, &md->particles);
	free_count += free_images(md, &md->env_images);
	free_count += free_gst(md);
	if (md->plr.frame)
		free_count += free_player_animations(&md->plr, md);
	free_count += free_void(md->map.buffer);
	mlx_destroy_window(md->mlx, md->win);
	if (md->cursor)
		free_count += mlx_destroy_image(md->mlx, md->cursor);
	free_count += free_void(md->mlx);
	ft_printf("Freed: %d elements\n", free_count);
	return (free_count);
}
