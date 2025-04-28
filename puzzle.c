/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   puzzle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:30:34 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/28 15:28:07 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

typedef struct	s_cell
{
	t_image		*img;
	t_vec2		pos;
	int			alive;
	int			is_hov;
	int			set;
	t_vec2		valid_p;
	int			colors[2];
}	t_cell;

typedef struct s_cell_data
{
	t_dblst	*cells;
	t_image	*full_img;
	t_md	*md;
	t_vec2	cell_sz;
	int		cell_amount;
	int		screen_clr;
	int		cells_shuffled;
	int		shuff_amount;
	int		update_shuffle;
	double	shuff_timer;
	t_image	*screen;
	t_cell	*sel;
}	t_cell_data;

void	render_puz(t_cell_data *cd)
{
	t_dblst	*node;
	t_cell	*c;
	t_md	*md;

	node = cd->cells;
	md = cd->md;
	flush_img(cd->screen, cd->screen_clr, 10, -1);
	while (node)
	{
		c = (t_cell *)node->content;
		if (c->set)
			draw_img(c->img, cd->screen, c->pos, -1);
		node = node->next;
	}
	node = cd->cells;
	while (node)
	{
		c = (t_cell *)node->content;
		if (!c->set)
			draw_img(c->img, cd->screen, c->pos, -1);
		node = node->next;
	}
	mlx_put_image_to_window(md->mlx, md->win, cd->screen->img, 0, 0);
}

int	move_piece(t_cell_data *cd, t_vec2 mouse_pos)
{
	t_dblst	*node;
	t_cell	*c;
	t_dblst	*new_last;
	t_dblst	*last;

	if (cd->sel)
		return (cd->sel->pos = sub_vec2(mouse_pos, div_v2(cd->sel->img->size, 2)), 1);
	new_last = NULL;
	node = cd->cells;
	while (node)
	{
		c = (t_cell *)node->content;
		if (!c->set && v2_touch(c->pos, cd->cell_sz, mouse_pos, v2(2)))
			new_last = node;
		node = node->next;
	}
	if (!new_last)
		return (0);
	cd->sel = new_last->content;
	cd->sel->pos = sub_vec2(mouse_pos, div_v2(cd->sel->img->size, 2));
	if (new_last->prev)
		new_last->prev->next = new_last->next;
	if (new_last->next)
		new_last->next->prev = new_last->prev;
	last = dblst_last(cd->cells);
	last->next = new_last;
	new_last->prev = last;
	new_last->next = NULL;
	cd->cells = dblst_first(new_last);
	return (1);
}

t_dblst	*get_rand_node(t_cell_data *cd, t_dblst	*start_n)
{
	const int	rand_stop_chance = dblst_size(cd->cells);
	t_dblst		*node;

	node = cd->cells;
	while (node)
	{
		if (node == start_n)
		{
			node = node->next;
			continue ;
		}
		if (r_range_seed(&cd->md->r_seed, 0, rand_stop_chance - 1) != 0)
			return (node);
		if (!node->next)
			return (node);
		node = node->next;
	}
	return (NULL);
}

void shuffle_pieces(t_cell_data *cd)
{
    t_dblst *node, *random_node;
    t_cell *current_cell, *random_cell;
    t_vec2 temp_pos;
    int size, rand_index, i;

    size = dblst_size(cd->cells);
    node = cd->cells;
    for (i = 0; i < size; ++i)
    {
        current_cell = (t_cell *)node->content;
        rand_index = r_range_seed(&cd->md->r_seed, 0, size - 1);
        random_node = cd->cells;
        for (int j = 0; j < rand_index; ++j)
        {
            random_node = random_node->next;
        }
        random_cell = (t_cell *)random_node->content;
        temp_pos = current_cell->pos;
        current_cell->pos = random_cell->pos;
        random_cell->pos = temp_pos;

        node = node->next;
    }
    cd->shuff_amount--;
    if (cd->shuff_amount <= 0)
    {
        cd->cells_shuffled = 1;
    }
}

int	update_puz(t_cell_data *cd)
{
	t_md	*md;
	t_mouse	ms;

	md = cd->md;
	ms = md->mouse;
	if (!cd->cells_shuffled)
	{
		upd_timer(&cd->shuff_timer, get_time_in_seconds(), .4f, &cd->update_shuffle);
		if (cd->update_shuffle)
			shuffle_pieces(cd);
	}
	else if (ms.pressed == MOUSE_PRESS)
		move_piece(cd, ms.real);
	else
	{
		if (ms.click == MOUSE_RELEASE && cd->sel && v2_touch(cd->sel->valid_p, cd->sel->img->size, cd->sel->pos, cd->sel->img->size))
		{
			cd->sel->pos = cd->sel->valid_p;
			cd->sel->set = 1;
			flush_img(cd->sel->img, _WHITE, .8f, 0);
		}
		cd->sel = NULL;
	}
	render_puz(cd);
	return (1);
}

void	draw_split(t_image *src, t_image *dst, t_vec2 draw_p, t_vec2 size)
{
	t_vec2 p;
	int     src_i;
	int     dst_i;
	t_vec2	dst_cord;
	t_vec2	src_cord;

	p = v2(-1);
	while (++p.y < size.y)
	{
		p.x = -1;
		while (++p.x < size.x)
		{
			src_cord = (t_vec2){p.x + draw_p.x, p.y + draw_p.y};
			dst_cord = p;
			src_i = src_cord.y * (src->size_line / 4) + src_cord.x;
			dst_i = dst_cord.y * (dst->size_line / 4) + dst_cord.x;
			if (src_i >= 0 && src_i <= src->size_line * src->size.y / 4 &&
				dst_i >= 0 && dst_i <= dst->size_line * dst->size.y / 4)
				dst->src[dst_i] = src->src[src_i];
		}
	}
}

void	init_cell_data(char *img_path, t_cell_data *cd, t_vec2 win_sz, t_vec2 pieces_len)
{
	const int	amount = pieces_len.x * pieces_len.y;
	t_vec2		p;
	t_cell		*c;
	t_md		*md;
	t_vec2		draw_p;

	md = cd->md;
	cd->full_img = init_abs_img(md, win_sz, img_path);
	cd->screen_clr = v4_to_color(200, 200, 200, 255);
	cd->screen = init_img(md, win_sz, NULL, cd->screen_clr);
	cd->cell_sz.x = win_sz.x / pieces_len.x;
	cd->cell_sz.y = win_sz.y / pieces_len.y;
	cd->cells = NULL;
	cd->shuff_amount = 5;
	cd->sel = NULL;
	cd->cells_shuffled = 0;
	p = v2(-1);
	cd->cell_amount = amount;
	while (++p.y < pieces_len.y)
	{
		p.x = -1;
		while (++p.x < pieces_len.x)
		{
			c = malloc(sizeof(t_cell));
			c->img = init_img(md, cd->cell_sz, NULL, -1);
			draw_p = (t_vec2){p.x * c->img->size.x, p.y * c->img->size.y};
			draw_split(cd->full_img, c->img, draw_p, cd->cell_sz);
			c->pos = (t_vec2){p.x * cd->cell_sz.x, p.y * cd->cell_sz.y};
			c->valid_p = c->pos;
			c->set = 0;
			dblst_add_back(&cd->cells, dblst_new((void *)c));
		}
	}
}

int	main(int argc, char *argv[])
{
	const t_vec2	win_sz = (t_vec2){800, 800};
	const int		pieces_len = 3;
	t_md			md;
	t_cell_data		cd;

	init_wrapper(&md, win_sz, "Puzzle", 10);
	cd.md = &md;
	if (argc == 2)
		init_cell_data(argv[1], &cd, win_sz, v2(pieces_len));
	else
		init_cell_data("0.xpm", &cd, win_sz, v2(pieces_len));
	init_fonts(&md);
	md.prm.au_on = 1;
	md.timer.tm_anim = 0;
	md.timer.trig_anim = 0;
	mlx_hook(md.win, 2, 0, handle_key_press, &md);
	mlx_hook(md.win, 3, 0, handle_key_release, &md);
	mlx_hook(md.win, 17, 0, close_window, &md);
	mlx_loop_hook(md.mlx, update_puz, (void *)&cd);
	mlx_loop(md.mlx);
}
