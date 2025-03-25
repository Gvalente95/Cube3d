/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_gen_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 21:15:25 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/23 22:28:44 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

char	*get_rectangle(t_vec2 size)
{
	char	*rect;
	t_vec2	pos;
	int		i;

	rect = malloc((size.x * size.y) + 1);
	pos = get_v2(-1, -1);
	i = -1;
	while (++pos.y < size.y)
	{
		pos.x = -1;
		while (++pos.x < size.x)
		{
			if (pos.x == 0 || pos.x == size.x || pos.y == 0 || pos.y == size.y)
				rect[i++] = '0';
			else
				rect[i++] = '1';
		}
		rect[i++] = '\n';
	}
	rect[i] = '\0';
	return (rect);
}

int	get_char_amount(char *buffer, char c)
{
	int	i;
	int	amount;

	amount = 0;
	i = -1;
	while (buffer[++i])
		if (buffer[i] == c)
			amount++;
	return (amount);
}

void	close_map(char *map, t_vec2 size, int len)
{
	int	i;

	i = -1;
	while (map[++i])
	{
		if (map[i] != '0' && map[i] != 'D')
			continue ;
		if (i > 0 && map[i - 1] == ' ')
			map[i] = '1';
		else if (i < len && map[i + 1] == ' ')
			map[i] = '1';
		else if (i - size.x > 0 && map[i - size.x] == ' ')
			map[i] = '1';
		else if (i + size.x < len && map[i + size.x] == ' ')
			map[i] = '1';
	}
}
