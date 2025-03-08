/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TEXT.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 06:30:21 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/07 13:53:43 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	display_letter(t_md *md, char c, t_vec4 data)
{
	t_ent	l;
	char	*path;
	char	*p;
	char	*txt;

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
	path = ft_strjoin(FONT_SPRITE_PATH, txt);
	p = ft_strjoin(path, ".png");
	free(path);
	if (access(p, F_OK) == -1)
		return (free(p), free(txt), 0);
	l.frame = mlx_png_file_to_image(md->mlx, p, &l.size.x, &l.size.y);
	if (l.frame)
	{
		l.frame = scale_img(md, l.frame, &l.size, get_v2(data.a, data.a));
		if (data.b != -1)
			set_img_color(l.frame, l.size, data.b, .5);
		mlx_put_image_to_window(md->mlx, md->win, l.frame, data.r, data.g);
	}
	return (free(p), free(txt), mlx_destroy_image(md->mlx, l.frame), l.size.x);
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

//	DATA.b = COLOR | DATA.a = SCALE
void	rnd_abs_txt(t_md *md, t_vec4 data, const char *format, ...)
{
	char	buff[256];
	va_list	args;

	va_start(args, format);
	vsnprintf(buff, sizeof(buff), format, args);
	va_end(args);
	display_text(md, buff, data);
}
