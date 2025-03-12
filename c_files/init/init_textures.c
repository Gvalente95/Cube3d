/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:09:38 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/11 20:21:20 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

char	**get_frames(t_md *md, char *dir, int *amount, int max)
{
	char	**frames;
	char	*path;
	char	*num_str;

	num_str = NULL;
	frames = md_malloc(md, sizeof(char *) * (max + 1));
	*amount = 0;
	if (access(dir, F_OK) == -1)
		return (printf("\"%s\" path not found\n", dir), NULL);
	path = md_strjoin(md, dir, "0.png");
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
		path = ft_megajoin(dir, num_str, ".png", NULL);
		if (!path)
			return (free(num_str), free_void_array((void **)frames), NULL);
	}
	return (free(num_str), frames[*amount] = NULL, frames);
}

void	*ld_txtr(t_md *md, t_vec2 final_size, char *path)
{
	t_vec2	txt_size;
	void	*texture;

	if (access(path, F_OK) == -1)
		return (printf("\"%s\" file not found\n", path), NULL);
	texture = mlx_png_file_to_image(md->mlx, path, &txt_size.x, &txt_size.y);
	if (!texture)
		return (printf("\"%s\" unable to make texture\n", path), NULL);
	texture = scale_img(md, texture, &txt_size, final_size);
	if (!texture)
		return (printf("\"%s\" unable to scale texture\n", path), NULL);
	return (texture);
}

void	**ld_txtrs(t_md *md, t_vec2 final_size, char *dirpath)
{
	t_vec2	size;
	void	**txtr;
	char	**frms;
	int		amount;
	int		i;

	if (!dirpath)
		return (NULL);
	frms = get_frames(md, dirpath, &amount, ENT_FRAMES_MAX);
	if (!frms)
		return (printf("\"%s\" frames returned NULL\n", dirpath), NULL);
	txtr = md_malloc(md, sizeof(void *) * (amount + 1));
	i = 0;
	while (frms[i] && i < amount)
	{
		txtr[i] = mlx_png_file_to_image(md->mlx, frms[i], &size.x, &size.y);
		txtr[i] = scale_img(md, txtr[i], &size, final_size);
		if (!txtr[i])
			free_and_quit(md, "img fail in ld_txtrs\n", frms[i]);
		free(frms[i++]);
	}
	free(frms);
	txtr[amount] = NULL;
	return (txtr);
}
