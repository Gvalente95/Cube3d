/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FREE_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 02:48:05 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 03:11:09 by giuliovalen      ###   ########.fr       */
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


int	free_mob_images(t_md *md, t_ent *e, char *label)
{
	int	actions;
	int	len;

	len = 0;
	actions = -1;
	while (++actions < ENT_ACTION_LEN)
		len += free_images_data(md, e->anim[actions], "label");
	free(e->anim);
	return (len);
}

int	free_ents(t_md *md)
{
	int		count;
	t_dblst	*node;
	t_ent	*e;
	int		i;

	count = 0;
	node = dblst_first(md->entities);
	while (node)
	{
		if (e->type == nt_mob)
			count += free_mob_images(md, e, "mob");
		node = node->next;
	}
	count += dblst_size(md->entities);
	dblst_clear(&md->entities, free);
	return (count);
}

int	free_md2(t_md *md, int free_count)
{
	if (!quit || md->init_steps == 1)
	{
		free_count += mlx_destroy_window(md->mlx, md->win) + 1;
		free_count += free_void(md->mlx);
		ft_printf("(PART A) Freed: %d elements\n", free_count);
		return (free_count);
	}
	free_count += free_ents(md);
	free_count += free_image_data(md, md->cursor);
	free_count += free_image_data(md, md->curs_dtc);
	free_count += free_image_data(md, md->curs_grb);
	free_count += mlx_destroy_window(md->mlx, md->win) + 1;
	free_count += free_void(md->mlx);
	ft_printf("Freed: %d elements\n", free_count);
	return (free_count);
}
