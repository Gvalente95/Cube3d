/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:10:03 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/14 00:22:49 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

void	setstr(char **str, char *new)
{
	safe_free((void *)*str);
	*str = new;
}

int	only_contains(char *str, char *to_contain)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (!char_in_str(str[i], to_contain))
			return (0);
	}
	return (1);
}

char	*ft_megajoin(const char *a, const char *b, const char *c, const char *d)
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
