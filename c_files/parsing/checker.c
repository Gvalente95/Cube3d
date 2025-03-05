/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:10:03 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 10:13:56 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	setstr(char **str, char *new)
{
	safe_free(*str);
	*str = new;
}

char	*extract_line(const char *str)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	if (!str || !str[0])
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	j = -1;
	while (++j < i)
		line[j] = str[j];
	line[i] = '\0';
	return (line);
}
