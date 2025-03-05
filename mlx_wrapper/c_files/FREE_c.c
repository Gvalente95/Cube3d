/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FREE_c.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 16:26:22 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/01/16 04:33:42 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

int	free_button(t_md *md, t_but *but)
{
	int	res;

	if (!but)
		return (0);
	res = 0;
	if (but->clk_bgr)
		res += mlx_destroy_image(md->mlx, but->clk_bgr);
	if (but->act_bgr)
		res += mlx_destroy_image(md->mlx, but->act_bgr);
	if (but->unact_bgr)
		res += mlx_destroy_image(md->mlx, but->unact_bgr);
	if (but->hov_bgr)
		res += mlx_destroy_image(md->mlx, but->hov_bgr);
	free(but);
	but = NULL;
	res++;
	return (res);
}

int	free_buttons(t_md *md)
{
	t_vec2	pos;
	int		count;

	if (!md->gst.buttons)
		return (0);
	pos = get_v2(0, 0);
	count = 0;
	while (md->gst.buttons[++pos.x])
	{
		pos.y = -1;
		while (md->gst.buttons[pos.x][++pos.y])
			count += free_button(md, md->gst.buttons[pos.x][pos.y]);
		free(md->gst.buttons[pos.x]);
		md->gst.buttons[pos.x] = NULL;
		count++;
	}
	free(md->gst.buttons);
	md->gst.buttons = NULL;
	count++;
	return (count);
}

int	free_gst(t_md *md)
{
	int	count;
	int	i;

	if (!md->gst.buttons)
		return (0);
	count = 0;
	//count += free_buttons(md);
	i = -1;
	while (++i < gamestate_count - 1)
	{
		free(md->gst.menu_titles[i]);
		count++;
	}
	count += free_ents(&md->star_icon);
	free(md->cwd);
	return (count);
}
