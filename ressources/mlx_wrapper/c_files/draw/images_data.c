/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images_data.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:31:53 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/29 12:39:35 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

t_image	*init_abs_img(t_md *md, t_vec2 img_size, char *path)
{
	t_image	*img_data;

	if (!path)
		return (printf("Called abs init with no path\n"), NULL);
	img_data = malloc(sizeof(t_image));
	if (!img_data)
		return (printf("ERR: alloc for img_data\n"), NULL);
	img_data->img = ld_txtr(md, img_size, path, get_v2(0, 0));
	if (!img_data->img)
		return (printf("ERR: Failed to set image\n"), img_data);
	img_data->size = get_v2(img_size.x, img_size.y);
	img_data->addr = mlx_get_data_addr(img_data->img, &img_data->bpp, \
		&img_data->size_line, &img_data->endian);
	if (!img_data->addr)
		return (printf("ERR: Failed to get image data address\n"), img_data);
	img_data->src = (int *)img_data->addr;
	if (!img_data->src)
		return (printf("ERR: Failed to get src data\n"), img_data);
	img_data->pos = get_v2(0, 0);
	ft_strlcpy(img_data->path, path, 20);
	return (img_data);
}

t_image	*init_img(t_md *md, t_vec2 img_size, char *path, int color)
{
	t_image	*img_data;

	img_data = md_malloc(md, sizeof(t_image));
	if (path)
		img_data->img = ld_txtr(md, img_size, path, get_v2(0, 1));
	else
		img_data->img = mlx_new_image(md->mlx, img_size.x, img_size.y);
	if (!img_data->img)
		return (printf("ERR: Failed to set image\n"), img_data);
	img_data->size = get_v2(img_size.x, img_size.y);
	img_data->addr = mlx_get_data_addr(img_data->img, &img_data->bpp, \
		&img_data->size_line, &img_data->endian);
	if (!img_data->addr)
		return (printf("ERR: Failed to get image data address\n"), img_data);
	img_data->src = (int *)img_data->addr;
	if (!img_data->src)
		return (printf("ERR: Failed to get src data\n"), img_data);
	if (color != -1)
		flush_img(img_data, color, -1, path != NULL);
	if (path)
		ft_strlcpy(img_data->path, path, 20);
	else
		ft_strlcpy(img_data->path, "mlx_image", 20);
	return (img_data->pos = get_v2(0, 0), img_data);
}

t_image	**init_images(t_md *md, t_vec2 size, char *path)
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
	while (frms[++i])
	{
		img_data[i] = init_img(md, size, frms[i], -1);
		free(frms[i]);
	}
	free(frms);
	img_data[amount] = NULL;
	return (img_data);
}

t_image	*scale_imgd(t_md *md, t_image *imgd, t_vec2 new_size, int keep_ratio)
{
	if (keep_ratio)
		imgd->img = scale_img_keep_ratio(md, imgd->img, &imgd->size, new_size);
	else
		imgd->img = resize_img(md, imgd->img, &imgd->size, new_size);
	imgd->addr = mlx_get_data_addr(imgd->img, &imgd->bpp, \
		&imgd->size_line, &imgd->endian);
	if (!imgd->addr)
		return (printf("ERR: Failed to get image data address\n"), imgd);
	imgd->src = (int *)imgd->addr;
	if (!imgd->src)
		return (printf("ERR: Failed to get src data\n"), imgd);
	return (imgd);
}
