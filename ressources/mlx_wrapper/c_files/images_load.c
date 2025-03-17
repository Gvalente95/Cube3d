/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images_load.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:00:24 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/17 13:11:33 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"
#include "../../../cube.h"

char	**get_frames(t_md *md, char *dir, int *amount, int max)
{
	char	**frames;
	char	*path;
	char	*num_str;

	if (!dir || !amount)
		return (NULL);
	num_str = ft_strdup("0");
	frames = md_malloc(md, sizeof(char *) * (max + 1));
	*amount = 0;
	path = ft_megajoin(IMG_PATH, dir, num_str, ".xpm");
	if (access(path, F_OK) == -1)
		return (printf("\"%s\" not found in get_frames\n", dir), free(path), NULL);
	while (access(path, F_OK) != -1 && *amount < max)
	{
		free(path);
		frames[*amount] = ft_megajoin(dir, num_str, ".xpm", NULL);
		frames[*amount + 1] = NULL;
		setstr(&num_str, ft_itoa(*amount));
		*amount += 1;
		if (!num_str)
			return (free_void_array((void **)frames), NULL);
		path = ft_megajoin(IMG_PATH, dir, num_str, ".xpm");
		if (!path)
			return (free(num_str), free_void_array((void **)frames), NULL);
	}
	return (free(num_str), free(path), frames[*amount] = NULL, frames);
}

//	needs relative path
void	*ld_txtr(t_md *md, t_vec2 final_size, char *path, int maintain_ratios)
{
	t_vec2	txt_size;
	void	*texture;
	char	*full_path;

	full_path = md_strjoin(md, IMG_PATH, path);
	if (!full_path)
		return (printf("\"%s\" alloc failed\n", path), NULL);
	if (access(full_path, F_OK) == -1)
		return (printf("\"%s\" file not found in ld_txtr\n", full_path), NULL);
	texture = md->mlx_make(md->mlx, full_path, &txt_size.x, &txt_size.y);
	if (!texture)
		return (printf("\"%s\" can't make txtr\n", full_path), NULL);
	if (maintain_ratios)
		texture = scale_img_keep_ratio(md, texture, &txt_size, final_size);
	else
		texture = resize_img(md, texture, &txt_size, final_size);
	if (!texture)
		return (printf("\"%s\" can't scale txtr\n", full_path), NULL);
	return (texture);
}

void	**ld_txtrs(t_md *md, t_vec2 final_size, char *dirpath, int keep_ratio)
{
	t_vec2	size;
	void	**txtr;
	char	*fulldir;
	char	**frms;
	int		i;

	fulldir = md_strjoin(md, IMG_PATH, dirpath);
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
