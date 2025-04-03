/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:48:25 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/03 20:45:25 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	set_base_mmap_bg(t_md *md, t_mmap *mmap, int ic_len)
{
	t_vec2		pos;
	int			i;
	char		c;

	i = -1;
	while (md->map.buffer[++i])
	{
		c = md->map.buffer[i];
		if (c == ' ' || c == '1')
			continue ;
		pos = get_v2(i % (md->map.size.x + 1), i / (md->map.size.x + 1));
		draw_pixels(mmap->bg, get_v2(ic_len * pos.x, ic_len * pos.y), \
			v2(ic_len), md->rgb[RGB_YELLOW]);
		if (c != '1')
			continue ;
		draw_pixels(mmap->bg, get_v2(ic_len * pos.x - 1, ic_len * pos.y - 1), \
		v2(ic_len - 1), md->rgb[RGB_BLACK]);
	}
}

void	init_minimap(t_md *md, t_mmap *mmap)
{
	int	ic_len;

	ic_len = max(4, md->win_sz.x / 125);
	mmap->ic_scl = ic_len;
	mmap->comps_scl = md->win_sz.x / 150;
	mmap->cmps = 1;
	mmap->mray_len = 0;
	mmap->revealed_cur = 0;
	mmap->active = 1;
	mmap->size = get_v2(md->map.size.x * ic_len, md->map.size.y * ic_len);
	mmap->img = init_img(md, mmap->size, NULL, md->rgb[RGB_NULL]);
	mmap->bg = init_img(md, mmap->size, NULL, md->rgb[RGB_NULL]);
}
