/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_tools2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:10:13 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/09 12:27:10 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

char	*ft_megajoin(char *a, char *b, char *c, char *d)
{
	char	*ab;
	char	*abc;
	char	*abcd;

	if (!a || !b)
		return (NULL);
	ab = ft_strjoin(a, b);
	if (!ab)
		return (NULL);
	if (!c)
		return (ab);
	abc = ft_strjoin(ab, c);
	free(ab);
	if (!abc)
		return (NULL);
	if (!d)
		return (abc);
	abcd = ft_strjoin(abc, d);
	free(abc);
	if (!abcd)
		return (NULL);
	return (abcd);
}

int	get_char_index(const char *str, char to_check)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == to_check)
			return (i);
	return (-1);
}
