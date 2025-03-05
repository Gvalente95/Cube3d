/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 22:23:11 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 12:27:28 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int	main(int argc, char *argv[])
{
	t_md	md;
	t_vec2	win_size;

	if (argc <= 1 || argc >= 4)
		return (printf("unvalid arg size\n"), 1);
	win_size = get_v2(1000, 1000);
	init_wrapper(&md, win_size, "Cube3d", 10);
	init_cube(&md, argv[1], argc > 2);
	mlx_loop_hook(md.mlx, (int (*)())update_and_render, &md);
	mlx_loop(md.mlx);
	return (0);
}
