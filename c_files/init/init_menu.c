/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:23:31 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/16 21:51:02 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	init_menu(t_md *md, t_menu *menu)
{
	menu->active = 0;
	menu->bgr_color = md->rgb[RGB_MAGENT];
	menu->overlay = init_img_data(md, md->win_size, NULL, menu->bgr_color);
}
