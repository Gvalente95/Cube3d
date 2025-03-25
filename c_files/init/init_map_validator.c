/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map_validator.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 02:32:22 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/24 12:45:27 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	find_unvalid_char(char *str, char *must_contain)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (!char_in_str(str[i], must_contain))
			return (str[i]);
	}
	return ('\0');
}

int	flood_fill(char *str, int index, int map_width, int len)
{
	if (index < 0 || index >= len)
		return (1);
	if (char_in_str(str[index], "!1D*\n\0"))
		return (1);
	if (str[index] == ' ' || str[index] == '!' || str[index] == '\n')
		return (str[index] = '!', 0);
	str[index] = '*';
	if (!flood_fill(str, index - 1, map_width, len))
		return (0);
	if (!flood_fill(str, index + 1, map_width, len))
		return (0);
	if (!flood_fill(str, index - map_width, map_width, len))
		return (0);
	if (!flood_fill(str, index + map_width, map_width, len))
		return (0);
	return (1);
}

void	print_unvalid_flood(char *flooded_map)
{
	int	i;

	i = -1;
	while (flooded_map[++i])
	{
		if (flooded_map[i] == '!')
			printf("%s!%s", COLOR_TXT_RED, COLOR_TXT_RESET);
		else if (flooded_map[i] == '*')
			printf("%s*%s", COLOR_TXT_GREEN, COLOR_TXT_RESET);
		else
			printf("%c", flooded_map[i]);
	}
	printf("\n");
}

int	find_breach(char *buffer, int width, int len, int plr_index)
{
	int		space_index;
	char	*flooded_map;
	int		found_breach;

	flooded_map = ft_strdup(buffer);
	flood_fill(flooded_map, plr_index, width, len);
	if (contains(flooded_map, '!'))
		return (print_unvalid_flood(flooded_map), free(flooded_map), 1);
	space_index = get_char_index(flooded_map, '0');
	while (space_index != -1)
	{
		flood_fill(flooded_map, space_index, width, len);
		space_index = get_char_index(flooded_map, '0');
	}
	found_breach = contains(flooded_map, '!');
	if (found_breach)
		return (print_unvalid_flood(flooded_map), free(flooded_map), 1);
	free(flooded_map);
	return (0);
}

int	validate_map(t_md *md, char *map)
{
	char	unvalid_char;
	char	*valid_chars;

	valid_chars = md_strjoin(md, md->txd.ents_tp_map[0], " 10NSEW\n");
	unvalid_char = find_unvalid_char(map, valid_chars);
	if (unvalid_char != '\0')
		return (printf("Error\nUnvalid char: %c\n", unvalid_char), 0);
	return (1);
}
