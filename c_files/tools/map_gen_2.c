/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_gen_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 05:17:00 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 05:21:25 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static void	genmaze_2(char *map, t_vec2 sz, t_vec2 *stack, int i)
{
	const int	dx[] = {0, 0, -2, 2};
	const int	dy[] = {-2, 2, 0, 0};
	t_vec3		pos;
	t_vec2		cell;
	t_vec3		nn;

	pos = get_v3(r_range(1, sz.x - 2), r_range(1, sz.y - 2), 0);
	map[pos.y * (sz.x + 1) + pos.x] = '0';
	stack[pos.z++] = (t_vec2){pos.x, pos.y};
	while (pos.z > 0)
	{
		cell = stack[--pos.z];
		i = -1;
		while (++i < 4)
		{
			nn.z = r_range(0, 3);
			nn = get_v3(cell.x + dx[nn.z], cell.y + dy[nn.z], nn.z);
			if (!(nn.x > 0 && nn.x < sz.x - 1 && nn.y > 0 && nn.y < sz.y - 1 && \
				map[nn.y * (sz.x + 1) + nn.x] == '1'))
				continue ;
			map[(cell.y + dy[nn.z] / 2) * \
				(sz.x + 1) + (cell.x + dx[nn.z] / 2)] = '0';
			map[nn.y * (sz.x + 1) + nn.x] = '0';
			stack[pos.z++] = (t_vec2){nn.x, nn.y};
		}
	}
}

void	generate_maze(char *map, t_vec2 size)
{
	t_vec2		*stack;
	t_vec2		pos;
	int			i;

	pos.y = -1;
	while (++pos.y < size.y - 1)
	{
		pos.x = -1;
		while (++pos.x < size.x - 1)
		{
			if (pos.x % 2 == 1 && pos.y % 2 == 1)
				map[pos.y * (size.x + 1) + pos.x] = '0';
			else
				map[pos.y * (size.x + 1) + pos.x] = '1';
		}
	}
	stack = malloc(sizeof(t_vec2) * (size.x * size.y));
	genmaze_2(map, size, stack, 0);
	free(stack);
	i = -1;
	while (++i < (size.x * size.y) / 10)
		map[r_range(1, size.y - 2) * (size.x + 1) + \
			r_range(1, size.x - 2)] = '0';
}

static int	set_character(char c, int amount, char *buffer, int len)
{
	int	*free_indexes;
	int	free_amount;
	int	random_index;
	int	set_amount;
	int	i;

	free_amount = 0;
	free_indexes = malloc(sizeof(int) * len);
	i = -1;
	while (buffer[++i])
		if (buffer[i] == '0')
			free_indexes[free_amount++] = i;
	if (free_amount < 5)
		return (0);
	if (amount > free_amount)
		amount = free_amount;
	set_amount = amount;
	i = -1;
	while (amount--)
	{
		random_index = r_range(0, free_amount - 1);
		if (buffer[free_indexes[random_index]] == '0')
			buffer[free_indexes[random_index]] = c;
	}
	return (free_amount - set_amount);
}

void	set_characters(char *map, int difficulty)
{
	int	mobs_amount;
	int	pickup_amount;
	int	space;
	int	len;

	len = ft_strlen(map);
	space = get_char_amount(map, '0');
	mobs_amount = (space / (20 - difficulty));
	pickup_amount = (space / (20 + difficulty));
	set_character('N', 1, map, len);
	set_character('M', mobs_amount, map, len);
	set_character('P', pickup_amount, map, len);
}
