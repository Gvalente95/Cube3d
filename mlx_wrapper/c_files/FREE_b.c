/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FREE_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 02:48:05 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/12 06:09:11 by giuliovalen      ###   ########.fr       */
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

int	free_void_array(void **elements)
{
	int	i;
	int	free_count;

	free_count = 0;
	if (!elements)
		return (printf("tried to free null void **\n"), 0);
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

int	free_player_animations(t_ent *plr, t_md *md)
{
	int	i;
	int	freed_count;

	i = -1;
	freed_count = 0;
	while (++i < ENT_ACTION_LEN)
	{
		if (plr->anim[i])
			freed_count += free_images(md, &plr->anim[i], "plr anim");
	}
	return (freed_count);
}

int	free_md2(t_md *md, int free_count)
{
	dblst_clear(&md->entities, free);
	free_count += mlx_destroy_window(md->mlx, md->win) + 1;
	if (md->cursor)
	{
		free_count += mlx_destroy_image(md->mlx, md->cursor) + 1;
		free_count += mlx_destroy_image(md->mlx, md->curs_dtc) + 1;
		free_count += mlx_destroy_image(md->mlx, md->curs_grb) + 1;
	}
	if (md->map.buffer)
	{
		free(md->map.buffer);
		free_count++;
	}
	free_count += free_void(md->mlx);
	ft_printf("Freed: %d elements\n", free_count);
	return (free_count);
}
