/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 22:23:11 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/11 22:09:06 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int	main(int argc, char *argv[])
{
	t_md	md;
	int		res;
	t_vec2	scrn_size;

	if (argc <= 1 || argc >= 6)
		return (printf("unvalid arg size\n"), 1);
	res = RESOLUTION;
	scrn_size = get_v2(SCRN_W, SCRN_H);
	if (argc >= 3)
		res = ft_atoi(argv[2]);
	if (argc >= 4)
		scrn_size.x = ft_atoi(argv[3]);
	if (argc >= 5)
		scrn_size.y = ft_atoi(argv[4]);
	init_wrapper(&md, get_v2(scrn_size.x, scrn_size.y), "Cube3d", res);
	init_cube(&md, argv[1], argc > 2);
	mlx_loop_hook(md.mlx, (int (*)())update_and_render, &md);
	mlx_loop(md.mlx);
	return (0);
}
