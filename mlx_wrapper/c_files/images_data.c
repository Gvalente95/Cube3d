/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images_data.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:31:53 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/15 03:11:49 by giuliovalen      ###   ########.fr       */
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
		return (printf("\"%s\" unable to make txtr\n", path), free(path), NULL);
	if (maintain_ratios)
		texture = scale_img_keep_ratio(md, texture, &txt_size, final_size);
	else
		texture = resize_img(md, texture, &txt_size, final_size);
	if (!texture)
		return (printf("\"%s\" unable to scale txtr\n", path), free(path), NULL);
	return (texture);
}

void	**ld_txtrs(t_md *md, t_vec2 final_size, char *dirpath, int maintain_ratios)
{
	t_vec2	size;
	void	**txtr;
	char	*fulldir;
	char	**frms;
	int		amount;
	int		i;

	if (!dirpath)
		return (NULL);
	fulldir = ft_megajoin(md->image_dir, "/", dirpath, NULL);
	frms = get_frames(md, dirpath, &amount, ENT_FRAMES_MAX);
	if (!frms)
		return (printf("\"%s\" frames NULL\n", dirpath), free(fulldir), NULL);
	free(fulldir);
	txtr = md_malloc(md, sizeof(void *) * (amount + 1));
	i = 0;
	while (frms[i] && i < amount)
	{
		txtr[i] = md->mlx_make(md->mlx, frms[i], &size.x, &size.y);
		if (maintain_ratios)
			txtr[i] = scale_img_keep_ratio(md, txtr[i], &size, final_size);
		else
			txtr[i] = resize_img(md, txtr[i], &size, final_size);
		free(frms[i++]);
	}
	free(frms);
	txtr[amount] = NULL;
	return (txtr);
}

t_image	*init_img_data(t_md *md, t_vec2 img_size, char *path, int color)
{
	t_image	*img_data;

	img_data = malloc(sizeof(t_image));
	if (!img_data)
		return (printf("ERR: alloc for img_data\n"), NULL);
	if (path)
		img_data->img = ld_txtr(md, img_size, path, 0);
	else
		img_data->img = mlx_new_image(md->mlx, img_size.x, img_size.y);
	if (!img_data->img)
		return (printf("ERR: Failed to set image\n"), img_data);
	img_data->size = get_v2(img_size.x, img_size.y);
	img_data->addr = mlx_get_data_addr(img_data->img, &img_data->bpp, \
		&img_data->size_line, &img_data->endian);
	if (!img_data->addr)
		return (printf("ERR: Failed to get image data address\n"), img_data);
	img_data->src_data = (int *)img_data->addr;
	if (!img_data->src_data)
		return (printf("ERR: Failed to get src data\n"), img_data);
	img_data->pos = get_v2(0, 0);
	if (color != -1)
		flush_img(img_data, color, -1);
	return (img_data);
}

t_image	**init_imgs_data(t_md *md, t_vec2 size, char *path)
{
	t_image	**img_data;
	char	**frms;
	int		amount;
	int		i;

	if (!path)
		return (printf("no path in imgs_data\n"), NULL);
	frms = get_frames(md, path, &amount, ENT_FRAMES_MAX);
	if (!frms)
		return (printf("\"%s\" frames NULL\n", path), NULL);
	img_data = md_malloc(md, sizeof(t_image *) * (amount + 1));
	i = -1;
	while (frms[++i] && i < amount)
	{
		img_data[i] = init_img_data(md, size, frms[i], -1);
		free(frms[i]);
	}
	free(frms);
	img_data[amount] = NULL;
	return (img_data);
}
