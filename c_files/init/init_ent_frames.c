/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ent_frames.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:42:52 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/18 21:44:25 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static t_image	**copy_action_frames(t_md *md, t_image **base_images)
{
	int		j;
	int		frame_count;
	t_image	**action_images;

	if (!base_images)
		return (NULL);
	frame_count = 0;
	while (base_images[frame_count])
		frame_count++;
	action_images = malloc(sizeof(t_image *) * (frame_count + 1));
	if (!action_images)
		return (NULL);
	j = -1;
	while (++j < frame_count)
		action_images[j] = copy_image(md, base_images[j]);
	action_images[j] = NULL;
	return (action_images);
}

static int	copy_anim_frames(t_md *md, t_ent *e)
{
	t_image		***ent_images;
	t_image		***base_images;
	int			i;

	e->mob_type = r_range_seed(&md->random_seed, 0, MOB_TYPE_LEN - 1);
	base_images = md->mobs_txtrs[e->mob_type];
	ent_images = malloc(sizeof(t_image **) * (ENT_ACTION_LEN + 1));
	if (!ent_images)
		return (printf("Error alloc of image in copy anim\n"), 0);
	i = -1;
	while (++i < ENT_ACTION_LEN)
	{
		ent_images[i] = copy_action_frames(md, base_images[i]);
		if (ent_images[i])
			continue ;
		while (--i >= 0)
			free(ent_images[i]);
		free(ent_images);
		return (printf("Error alloc of image in copy anim\n"), 0);
	}
	ent_images[i] = NULL;
	e->anim = ent_images;
	e->frame = e->anim[0][0];
	return (1);
}

void	init_ent_frames(t_md *md, t_ent *e, char c)
{
	e->type = get_char_index(md->ents_tp_map[0], c);
	if (e->type == nt_wall)
		e->frame = md->wall_img[0];
	else if (e->type == nt_mob)
		copy_anim_frames(md, e);
	else
		e->frame = md->pickup_txtr[r_range_seed(\
			&md->random_seed, 0, PICKUP_TYPE_LEN - 1)][0];
	e->size = e->frame->size;
	e->action = 0;
}
