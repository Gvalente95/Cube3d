/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images_load.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:00:24 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/16 23:06:20 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

char	**get_frames(t_md *md, char *dir, int *amount, int max)
{
	char	**frames;
	char	*path;
	char	*num_str;

	if (!dir || !amount)
		return (NULL);
	num_str = NULL;
	frames = md_malloc(md, sizeof(char *) * (max + 1));
	*amount = 0;
	if (access(dir, F_OK) == -1)
		return (printf("\"%s\" path not found\n", dir), NULL);
	path = ft_megajoin(dir, "0", md->img_format, NULL);
	if (access(path, F_OK) == -1)
		return (printf("\"%s\" not found\n", dir), free(path), NULL);
	while (access(path, F_OK) != -1 && *amount < max)
	{
		frames[*amount] = path;
		frames[*amount + 1] = NULL;
		setstr(&num_str, ft_itoa(*amount));
		*amount += 1;
		if (!num_str)
			return (free_void_array((void **)frames), NULL);
		path = ft_megajoin(dir, num_str, md->img_format, NULL);
		if (!path)
			return (free(num_str), free_void_array((void **)frames), NULL);
	}
	return (free(num_str), free(path), frames[*amount] = NULL, frames);
}

void	*ld_txtr(t_md *md, t_vec2 final_size, char *path, int maintain_ratios)
{
	t_vec2	txt_size;
	void	*texture;

	if (access(path, F_OK) == -1)
		return (printf("\"%s\" file not found\n", path), free(path), NULL);
	texture = md->mlx_make(md->mlx, path, &txt_size.x, &txt_size.y);
	if (!texture)
		return (printf("\"%s\" can't make txtr\n", path), free(path), NULL);
	if (maintain_ratios)
		texture = scale_img_keep_ratio(md, texture, &txt_size, final_size);
	else
		texture = resize_img(md, texture, &txt_size, final_size);
	if (!texture)
		return (printf("\"%s\" can't scale txtr\n", path), free(path), NULL);
	return (texture);
}

void	**ld_txtrs(t_md *md, t_vec2 final_size, char *dirpath, int keep_ratio)
{
	t_vec2	size;
	void	**txtr;
	char	*fulldir;
	char	**frms;
	int		i;

	fulldir = ft_megajoin(md->image_dir, "/", dirpath, NULL);
	frms = get_frames(md, dirpath, &i, ENT_FRAMES_MAX);
	if (!frms)
		return (printf("\"%s\" frames NULL\n", fulldir), free(fulldir), NULL);
	free(fulldir);
	txtr = md_malloc(md, sizeof(void *) * (i + 1));
	i = 0;
	while (frms[i])
	{
		txtr[i] = md->mlx_make(md->mlx, frms[i], &size.x, &size.y);
		if (keep_ratio)
			txtr[i] = scale_img_keep_ratio(md, txtr[i], &size, final_size);
		else
			txtr[i] = resize_img(md, txtr[i], &size, final_size);
		free(frms[i++]);
	}
	return (free(frms), txtr[i] = NULL, txtr);
}
