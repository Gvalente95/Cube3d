/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images_data.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:31:53 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/13 21:28:27 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

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
	return (free(num_str), frames[*amount] = NULL, frames);
}

void	*ld_txtr(t_md *md, t_vec2 final_size, char *path)
{
	t_vec2	txt_size;
	void	*texture;

	path = ft_megajoin(md->img_dir_path, "/", path, md->img_format);
	if (access(path, F_OK) == -1)
		return (printf("\"%s\" file not found\n", path), free(path), NULL);
	texture = md->mlx_make(md->mlx, path, &txt_size.x, &txt_size.y);
	if (!texture)
		return (printf("\"%s\" unable to make txtr\n", path), free(path), NULL);
	texture = scale_abs_img(md, texture, &txt_size, final_size);
	if (!texture)
		return (printf("\"%s\" unable to scale txtr\n", path), free(path), NULL);
	free(path);
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
	dirpath = ft_megajoin(md->img_dir_path, "/", dirpath, NULL);
	frms = get_frames(md, dirpath, &amount, ENT_FRAMES_MAX);
	if (!frms)
		return (printf("\"%s\" frames NULL\n", dirpath), free(dirpath), NULL);
	free(dirpath);
	txtr = md_malloc(md, sizeof(void *) * (amount + 1));
	i = 0;
	while (frms[i] && i < amount)
	{
		txtr[i] = md->mlx_make(md->mlx, frms[i], &size.x, &size.y);
		txtr[i] = scale_img(md, txtr[i], &size, final_size);
		free(frms[i++]);
	}
	free(frms);
	txtr[amount] = NULL;
	return (txtr);
}

void	*init_img_data(t_md *md, t_image *img_data, t_vec2 img_size, char *path)
{
	void	*img;

	if (path)
		img = ld_txtr(md, img_size, path);
	else
		img = mlx_new_image(md->mlx, img_size.x, img_size.y);
	img_data->img = img;
	img_data->size = img_size;
	if (!img_data->img)
	{
		printf("error while making image\n");
		return (NULL);
	}
	img_data->addr = mlx_get_data_addr(img_data->img, &img_data->bpp, \
		&img_data->size_line, &img_data->endian);
	img_data->src_data = (int *)img_data->addr;
	img_data->pos = get_v2(0, 0);
	return (img_data->img);
}

void	init_imgs_data(t_md *md, t_vec2 size, t_image **data, char *path)
{
	char	**frms;
	int		amount;
	int		i;

	if (!path)
		return ;
	path = ft_megajoin(md->img_dir_path, "/", path, NULL);
	frms = get_frames(md, path, &amount, ENT_FRAMES_MAX);
	if (!frms)
	{
		printf("\"%s\" frames NULL\n", path);
		return ;
	}
	free(path);
	data = md_malloc(md, sizeof(t_image *) * (amount + 1));
	i = 0;
	while (frms[i] && i < amount)
	{
		init_img_data(md, data[i], size, frms[i]);
		free(frms[i++]);
	}
	free(frms);
	data[amount] = NULL;
}
