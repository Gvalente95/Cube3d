/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:21:44 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/24 11:56:53 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

int	r_range_seed(unsigned int *g_seed, int min, int max)
{
	if (min > max)
		return (-1);
	*g_seed = (*g_seed * 1103515245 + 12345) & 0x7FFFFFFF;
	return (min + (*g_seed % (max - min + 1)));
}

int	r_range(int min, int max)
{
	if (min > max)
		return (-1);
	return (min + rand() % (max - min + 1));
}

float	f_range(float min, float max)
{
	float	scale;

	if (min > max)
		return (-1.0f);
	scale = rand() / (float)RAND_MAX;
	return (min + scale * (max - min));
}

void	*flip_image_x(t_md *md, void *img, t_vec2 size)
{
	t_vec2	pos;
	int		prm[5];
	void	*flipped_img;
	char	*src;
	char	*dest_data;

	flipped_img = mlx_new_image(md->mlx, size.x, size.y);
	if (!flipped_img)
		return (NULL);
	src = mlx_get_data_addr(img, &prm[0], &prm[1], &prm[2]);
	dest_data = mlx_get_data_addr(flipped_img, &prm[0], &prm[1], &prm[2]);
	if (!src || !dest_data)
		return (NULL);
	pos.y = -1;
	while (++pos.y < size.y)
	{
		pos.x = -1;
		while (++pos.x < size.x)
		{
			prm[3] = pos.y * prm[1] + (size.x - pos.x - 1) * (prm[0] / 8);
			prm[4] = pos.y * prm[1] + pos.x * (prm[0] / 8);
			ft_memcpy(dest_data + prm[4], src + prm[3], prm[0] / 8);
		}
	}
	return (mlx_destroy_image(md->mlx, img), img = flipped_img, flipped_img);
}
