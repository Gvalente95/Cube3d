/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 02:48:05 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/13 21:57:58 by gvalente         ###   ########.fr       */
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
	if (md->cursor)
	{
		free_count += free_image_data(md, md->cursor);
		free_count += free_image_data(md, md->curs_dtc);
		free_count += free_image_data(md, md->curs_grb);
	}
	if (md->map.buffer)
		free(md->map.buffer);
	ft_printf("Freed: %d elements\n", free_count);
	return (free_count);
}
