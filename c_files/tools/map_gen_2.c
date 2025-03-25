/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_gen_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 05:17:00 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 01:00:32 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

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

void	set_doors(char *map, int doors_amount)
{
	int	i;

	i = 0;
	while (map[++i] && doors_amount)
	{
		if (map[i] != '0')
			continue ;
		if (map[i - 1] == '1' && map[i + 1] == '1' && r_range(0, 50))
		{
			map[i] = 'D';
			doors_amount--;
			continue ;
		}
	}
}

void	set_characters(char *map, int difficulty)
{
	int	mobs_amount;
	int	pickup_amount;
	int	door_amount;
	int	space;
	int	len;

	len = ft_strlen(map);
	if (!contains(map, 'N'))
		set_character('N', 1, map, len);
	space = get_char_amount(map, '0');
	mobs_amount = minmax(1, 30, (space / (10 - difficulty)));
	set_character('M', mobs_amount, map, len);
	space = get_char_amount(map, '0');
	pickup_amount = minmax(1, 30, (space / (10 + difficulty)));
	set_character('P', pickup_amount, map, len);
	door_amount = len / 20;
	set_doors(map, door_amount);
}

char	*set_map_with_base(char *map)
{
	const char	data_info[6][50] = {
		"NO ressources/xpm/ent/wall/NORTH.xpm\n", \
		"SO ressources/xpm/ent/wall/SOUTH.xpm\n", \
		"WE ressources/xpm/ent/wall/WEST.xpm\n", \
		"EA ressources/xpm/ent/wall/EAST.xpm\n", \
		"F 96,64,32\n", "C 0,0,0\n"
	};
	int			i;
	char		*full_data;
	char		*tmp;

	full_data = ft_strdup("");
	i = -1;
	while (++i < 6)
	{
		tmp = full_data;
		full_data = ft_strjoin(tmp, data_info[i]);
		free(tmp);
		if (!full_data)
			return (printf("Error\nAlloc of texture\n"), NULL);
	}
	tmp = full_data;
	full_data = ft_strjoin(tmp, map);
	return (free(tmp), full_data);
}
