/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugger.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:03:13 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 14:06:04 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"
#include "../../../cube.h"

void	show_vec2(t_md *md, char *label, t_vec2 vec, t_vec2 pos)
{
	rnd_abs_txt(md, \
		get_v4(pos.x * (md->txt_scale * 1.5), pos.y * (md->txt_scale * 1.5), \
		COLOR_WHITE, md->txt_scale), \
		"%s x%d y%d", \
		label, vec.x, vec.y);
}

void	show_vec3(t_md *md, char *label, t_vec3 vec, t_vec2 pos)
{
	rnd_abs_txt(md, \
		get_v4(pos.x * (md->txt_scale * 1.5), pos.y * (md->txt_scale * 1.5), \
		COLOR_WHITE, md->txt_scale), \
		"%s x%d y%d z%d", \
		label, vec.x, vec.y, vec.z);
}


void	show_vec3f(t_md *md, char *label, t_vec3f vec, t_vec2 pos)
{
	rnd_abs_txt(md, \
		get_v4(pos.x * (md->txt_scale * 1.5), pos.y * (md->txt_scale * 1.5), \
		COLOR_WHITE, md->txt_scale), \
		"%s x%.2f y%.2f z%.2f", \
		label, vec.x, vec.y, vec.z);
}

void	show_int(t_md *md, char *label, int value, t_vec2 pos)
{
	rnd_abs_txt(md, \
		get_v4(pos.x * (md->txt_scale * 1.5), pos.y * (md->txt_scale * 1.5), \
		COLOR_WHITE, md->txt_scale), "%s %d", label, value);
}

void	show_float(t_md *md, char *label, float value, t_vec2 pos)
{
	rnd_abs_txt(md, \
		get_v4(pos.x * (md->txt_scale * 1.5), pos.y * (md->txt_scale * 1.5), \
		COLOR_WHITE, md->txt_scale), "%s %-3f", label, value);
}
