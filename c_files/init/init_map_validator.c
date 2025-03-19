/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map_validator.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 02:32:22 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 02:44:37 by giuliovalen      ###   ########.fr       */
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

int	validate_map(t_md *md, char *map, int len)
{
	char	unvalid_char;
	char	*valid_chars;
	int		i;

	i = -1;
	while (map[++i] != '\n')
		if (map[i] != '1')
			free_and_quit(md, "Map unclosed", map + i);
	i = len;
	while (map[--i] != '\n')
		if (map[i] != '1')
			free_and_quit(md, "Map unclosed", map + i);
	valid_chars = md_strjoin(md, md->ents_tp_map[0], " 10NSEW\n");
	unvalid_char = find_unvalid_char(map, valid_chars);
	if (unvalid_char != '\0')
		return (printf("Error\nUnvalid char: %c\n", unvalid_char), 0);
	return (1);
}
