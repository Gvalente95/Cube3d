/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_num.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:16:44 by gvalente          #+#    #+#             */
/*   Updated: 2024/10/31 16:50:01 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

static int	ft_get_unsigned_len(unsigned int n)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_utoa(unsigned int n)
{
	char	*res;
	int		len;

	if (n == 0)
		return (ft_strdup("0"));
	len = ft_get_unsigned_len(n);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	res[len] = '\0';
	while (n)
	{
		res[--len] = (n % 10) + '0';
		n /= 10;
	}
	return (res);
}
