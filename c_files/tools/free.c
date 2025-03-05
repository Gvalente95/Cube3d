/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 22:14:54 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 10:22:55 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int	safe_free(void *item)
{
	if (item)
	{
		free(item);
		return (1);
	}
	return (0);
}

int	free_and_quit(t_md *d, const char *msg, const char *attribute)
{
	if (msg)
	{
		printf("Error\n%s", msg);
		if (attribute)
			printf(": %s", attribute);
		printf("\n");
	}
	if (d->wall_txtr)
		free_images(d, &d->wall_txtr);
	free_md(d, 1);
	exit(0);
}
