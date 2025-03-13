/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 06:30:21 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/12 17:00:24 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	display_letter(t_md *md, char c, t_vec4 data)
{
	t_ent	l;
	char	*path;
	char	*txt;

	c = ft_toupper(c);
	if (c == '.')
		txt = ft_strdup("dot");
	else if (c == ':')
		txt = ft_strdup("db_dot");
	else if (c == '/')
		txt = ft_strdup("bar");
	else
	{
		txt = malloc(2);
		txt[0] = c;
		txt[1] = '\0';
	}
	path = ft_megajoin(md->img_dir_path, "/utils/font/", txt, md->img_format);
	if (access(path, F_OK) == -1)
		return (free(txt), printf("font not found: %s\n", path), free(path), 0);
	l.frame = md->mlx_make(md->mlx, path, &l.size.x, &l.size.y);
	if (l.frame)
	{
		l.frame = scale_img(md, l.frame, &l.size, get_v2(data.a, data.a));
		if (data.b != -1)
			set_img_color(l.frame, l.size, data.b, .5);
		mlx_put_image_to_window(md->mlx, md->win, l.frame, data.r, data.g);
	}
	return (free(path), free(txt), mlx_destroy_image(md->mlx, l.frame), l.size.x);
}

static void	display_text(t_md *md, char *text, t_vec4 data)
{
	int		i;
	t_vec4	cur_pos;

	i = -1;
	cur_pos = get_v4(data.r, data.g, data.b, data.a);
	while (text[++i])
	{
		if (text[i] == '\n')
		{
			cur_pos.r = data.r;
			cur_pos.g += data.a * 2 + 10;
			continue ;
		}
		if (text[i] == '	')
		{
			cur_pos.r += data.a * 2;
			continue ;
		}
		cur_pos.r += display_letter(md, text[i], cur_pos);
	}
}

//	DATA.b = COLOR | DATA.a = SCALE
void	render_text(t_md *md, t_vec4 data, const char *format, ...)
{
	char	buff[256];
	va_list	args;

	if (data.b == -1)
		data.b = -1;
	data = get_v4(data.r + md->cam_ofst.x, data.g + \
		md->cam_ofst.y, data.b, data.a);
	va_start(args, format);
	vsnprintf(buff, sizeof(buff), format, args);
	va_end(args);
	display_text(md, buff, data);
}

//	DATA = (pos.x, pos.y, color, scale)
void	rnd_abs_txt(t_md *md, t_vec4 data, const char *format, ...)
{
	char	buff[256];
	va_list	args;

	va_start(args, format);
	vsnprintf(buff, sizeof(buff), format, args);
	va_end(args);
	display_text(md, buff, data);
}
