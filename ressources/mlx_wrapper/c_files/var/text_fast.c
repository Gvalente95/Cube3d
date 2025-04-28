/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_fast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 00:37:32 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/28 10:33:49 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../mlx_utils.h"

int	display_quick_letter(t_md *md, char c, t_txtd data)
{
	t_image	*l;
	t_image	*font_c;
	int		is_rescaled;

	is_rescaled = 0;
	c = ft_toupper(c);
	font_c = md->txd.font[(unsigned char)c];
	l = font_c;
	if (!l || !l->img)
		return (printf("Error\n%c not found\n", c), data.y);
	if (data.scale <= 0)
		data.scale = md->prm.txt_sc;
	else if (data.scale != md->prm.txt_sc)
	{
		l = copy_image(md, font_c, v2(data.scale), data.color);
		is_rescaled = 1;
	}
	if (data.color != -1 && !is_rescaled)
		draw_clr_img(l, data.onto, get_v2(data.x, data.y), \
		get_v3(data.color, 3, _BLACK));
	else
		draw_img(l, data.onto, get_v2(data.x, data.y), -1);
	if (is_rescaled)
		free_image_data(md, l);
	return (data.scale);
}

static int	display_line(t_md *md, char *text, t_txtd data)
{
	int		i;
	t_txtd	cur_pos;
	int		total_width;

	i = -1;
	cur_pos = data;
	total_width = 0;
	while (text[++i])
	{
		if (text[i] == '\n')
		{
			cur_pos.x = data.x;
			cur_pos.y += data.scale * 2 + 10;
			continue ;
		}
		if (text[i] == '	')
		{
			cur_pos.x += data.scale * 2;
			continue ;
		}
		cur_pos.x += display_quick_letter(md, text[i], cur_pos);
		total_width += md->prm.txt_sc * 1.5;
	}
	return (total_width);
}

//	DATA = (x pos, y pos, text color, text scale) return: text width
int	rnd_fast_txt(t_md *md, t_txtd data, const char *format, ...)
{
	char	buff[256];
	va_list	args;
	int		txt_width;

	va_start(args, format);
	vsnprintf(buff, sizeof(buff), format, args);
	va_end(args);
	if (!data.onto && md->menu.active && md->menu.freeze_frame)
		data.onto = md->menu.freeze_frame;
	if (!data.onto)
		data.onto = md->screen;
	txt_width = display_line(md, buff, data);
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
		letter[0] = ft_toupper(c);
		letter[1] = '\0';
	}
	path = ft_megajoin(base_path, letter, ".xpm", NULL);
	free(letter);
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
