/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TEXT.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 06:30:21 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/15 12:13:52 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

char	*get_img_path(t_md *md, char c)
{
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
	path = ft_megajoin(md->image_dir, "/utils/font/", txt, md->img_format);
	free(txt);
	return (path);
}

static int	display_letter(t_md *md, char c, t_vec4 data)
{
	t_image	*l;
	char	*path;

	path = get_img_path(md, c);
	if (!path)
		return (printf("font not found: %c\n", c), 0);
	l = init_img_data(md, get_v2(data.a, data.a), path, -1);
	if (data.b != -1)
		flush_img(l, data.b, 0.5, 1);
	free(path);
	if (l->img)
	{
		draw_img(l, md->screen, get_v2(data.r, data.g), -1);
		free_image_data(md, l);
	}
	return (data.a);
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
