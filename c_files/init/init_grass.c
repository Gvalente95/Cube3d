/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_fes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 15:30:20 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/31 23:35:03 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	get_grass_color(void)
{
	t_vec4	rgb;

	rgb.r = r_range(0, 20);
	rgb.g = r_range(120, 200);
	rgb.b = r_range(0, 40);
	return ((rgb.r << 16) | (rgb.g << 8) | rgb.b);
}

t_fe_type	get_random_fe_type(void)
{
	const int	distr[4] = {[fe_grass] = 400, [fe_tree] = 2, [fe_bush] = 1};
	int			total;
	int			r;
	int			i;
	int			acc;

	total = 0;
	i = -1;
	while (++i < fe_type_len)
		total += distr[i];
	r = r_range(0, total - 1);
	acc = 0;
	i = -1;
	while (++i < fe_type_len)
	{
		acc += distr[i];
		if (r < acc)
			return (i);
	}
	return (fe_grass);
}

void	init_fe(t_md *md, t_fe *fe)
{
	fe->active = r_range(0, FE_PER_TILE) == 0;
	if (!fe->active)
		return ;
	fe->type = get_random_fe_type();
	fe->cut_len = 0;
	fe->base_color = get_grass_color();
	fe->growth_factor = f_range(0.95, 1.05);
	fe->size = v2(r_range(14, 15), 1);
	fe->height_max = md->win_sz.y * .25;
	if (fe->type != fe_grass)
	{
		fe->base_color = _TURQ;
		fe->height_max = md->win_sz.y * .1;
		fe->growth_factor = f_range(.3, .6);
		fe->size = v2(10, 5);
	}
	fe->age = 0;
	fe->height = fe->size.y;
}

void	init_fes(t_md *md, t_env_manager *env, int tlen)
{
	const t_vec2	mapsz = md->map.size;
	t_vec2			map;
	t_vec2			cord;

	env->stored_blades = NULL;
	env->grass = md_malloc(md, sizeof(t_fe ***) * mapsz.y);
	map.y = -1;
	while (++map.y < mapsz.y)
	{
		env->grass[map.y] = md_malloc(md, sizeof(t_fe **) * mapsz.x);
		map.x = -1;
		while (++map.x < mapsz.x)
		{
			cord.y = -1;
			env->grass[map.y][map.x] = md_malloc(md, sizeof(t_fe *) * tlen);
			while (++cord.y < tlen)
			{
				env->grass[map.y][map.x][cord.y] = \
					md_malloc(md, sizeof(t_fe) * tlen);
				cord.x = -1;
				while (++cord.x < tlen)
					init_fe(md, &env->grass[map.y][map.x][cord.y][cord.x]);
			}
		}
	}
}
