/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images_data.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:31:53 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/16 23:30:29 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

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
		flush_img(img_data, color, -1, path != NULL);
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


void	copy_image_src(t_md *md, t_image *new_img, t_image *src_img)
{
	int		src_index;
	int		dst_index;
	t_vec2	pos;

	pos = get_v2(-1, -1);
	while (++ pos.y < src_img->size.y)
	{
		pos.x = -1;
		while (++pos.x < src_img->size.x)
		{
			src_index = pos.y * (src_img->size_line / 4) + pos.x;
			dst_index = pos.y * (new_img->size_line / 4) + pos.x;
			new_img->src_data[dst_index] = src_img->src_data[src_index];
		}
	}
}

t_image	*copy_image(t_md *md, t_image *src_img)
{
	t_image	*new_img;

	if (!md || !src_img || !src_img->src_data)
		return (NULL);
	new_img = malloc(sizeof(t_image));
	if (!new_img)
		return (NULL);
	new_img->img = mlx_new_image(md->mlx, src_img->size.x, src_img->size.y);
	if (!new_img->img)
		return (free(new_img), NULL);
	new_img->src_data = (int *)mlx_get_data_addr(new_img->img, &new_img->bpp, \
		&new_img->size_line, &new_img->endian);
	if (!new_img->src_data)
		return (mlx_destroy_image(md->mlx, new_img->img), free(new_img), NULL);
	copy_image_src(md, new_img, src_img);
	new_img->size = src_img->size;
	new_img->bpp = src_img->bpp;
	new_img->size_line = src_img->size_line;
	new_img->endian = src_img->endian;
	return (new_img);
}
