/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_fast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 00:37:32 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/04 12:13:01 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

static int	display_quick_letter(t_md *md, char c, t_vec4 data)
{
	t_image	*l;
	t_image	*screen;

	l = md->txd.font[(unsigned char)c];
	if (!l || !l->img)
		return (printf("Error\n%c not found\n", c), data.g);
	screen = md->screen;
	if (md->menu.active && md->menu.freeze_frame)
		screen = md->menu.freeze_frame;
	if (data.b != -1)
		draw_clr_img(l, screen, get_v2(data.r, data.g), \
		get_v3(data.b, 3, data.z));
	else
		draw_img(l, screen, get_v2(data.r, data.g), -1);
	return (data.a);
}

static int	display_quick_text(t_md *md, char *text, t_vec4 data)
{
	int		i;
	t_vec4	cur_pos;
	int		total_width;

	i = -1;
	cur_pos = get_v4(data.r, data.g, data.b, data.a);
	total_width = 0;
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
		cur_pos.r += display_quick_letter(md, ft_toupper(text[i]), cur_pos);
		total_width += md->prm.txt_sc * 1.5;
	}
	return (total_width);
}

//	DATA = (x pos, y pos, text color, text scale) return: text width
int	rnd_fast_txt(t_md *md, t_vec4 data, const char *format, ...)
{
	char	buff[256];
	va_list	args;
	int		txt_width;

	if (data.b == -1)
		data.b = -1;
	data = get_v4(data.r, data.g, data.b, data.a);
	va_start(args, format);
	vsnprintf(buff, sizeof(buff), format, args);
	va_end(args);
	txt_width = display_quick_text(md, buff, data);
	return (txt_width);
}

static char	*get_font_path(t_md *md, const char *base_path, char c)
{
	char	*path;
	char	*letter;

	if (c == '.')
		letter = md_strdup(md, "dot");
	else if (c == ':')
		letter = md_strdup(md, "db_dot");
	else if (c == '/')
		letter = md_strdup(md, "bar");
	else
	{
		letter = md_malloc(md, 2);
		letter[0] = c;
		letter[1] = '\0';
	}
	path = ft_megajoin(base_path, letter, ".xpm", NULL);
	return (path);
}

void	init_fonts(t_md *md)
{
	const char	*base_path = "utils/font/";
	char		*path;
	t_image		*img;
	int			c;

	c = 31;
	while (++c < 126)
	{
		if (char_in_str((char)c, "$&*;=\\^`{|}"))
			path = get_font_path(md, base_path, '?');
		else
			path = get_font_path(md, base_path, (char)c);
		img = init_img(md, v2(md->prm.txt_sc), path, -1);
		clean_img(img);
		md->txd.font[(unsigned char)c] = img;
		free(path);
	}
}
