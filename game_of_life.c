/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_of_life.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:30:34 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/28 10:34:58 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ressources/mlx_wrapper/mlx_utils.h"

typedef struct	s_cell
{
	int 		x;
	int			y;
	int			alive;
	int			is_hov;
	double	state_change_time;
	int			colors[2];
}	t_cell;

typedef struct s_pattern
{
	t_cell		*cells;
	t_vec2		size;
	const char	*name;
	int			cell_len;
}	t_shape;

typedef struct s_cell_data
{
	t_cell	*cells;
	int		cell_len;
	t_vec2	cell_sz;
	int		row;
	t_image	*screen;
	t_image	*overlay;
	t_md	*md;
	int		hov_i;
	int		cell_clr[2];
	int		bgr_clr;
	int		update_sim;
	int		show_keys;
	int		pattern_index;
	t_shape	shapes[5];
	int		has_fade;
	int		reverse;
	int		speed_point;
	double	frame_dur;
}	t_cell_data;

void	update_overlay(t_cell_data *cd)
{
	t_md *md;

	md = cd->md;
	flush_img(cd->screen, cd->bgr_clr, 10, 0);
	flush_img(cd->overlay, cd->bgr_clr, 10, 0);
	if (!cd->show_keys)
	{
		rnd_fast_txt(md, (t_txtd){0, cd->overlay->size.y - md->prm.txt_sc * 5, -1, -1, cd->overlay}, "H = Show keys");
		return ;
	}
	rnd_fast_txt(md, (t_txtd){0, cd->overlay->size.y - md->prm.txt_sc * 5, -1, -1, cd->overlay}, "%-10s [%s]", "ENTER : stop/start", cd->update_sim ? "ON" : "OFF");
	rnd_fast_txt(md, (t_txtd){0, cd->overlay->size.y - md->prm.txt_sc * 3, -1, -1, cd->overlay}, "%-10s -a %-7.7s +d", "shape", cd->shapes[cd->pattern_index].name);
	rnd_fast_txt(md, (t_txtd){0, cd->overlay->size.y - md->prm.txt_sc * 2, -1, -1, cd->overlay}, "%-10s -s %.4fs +w", "frame dur", cd->frame_dur);
	rnd_fast_txt(md, (t_txtd){0, cd->overlay->size.y - md->prm.txt_sc * 1, -1, -1, cd->overlay}, "clear [R] - Next [SPC] - Erase [Shft + click]");
}

int	lerp_color(int start, int end, float t)
{
	int r = ((start >> 16) & 0xFF) + t * (((end >> 16) & 0xFF) - ((start >> 16) & 0xFF));
	int g = ((start >> 8) & 0xFF) + t * (((end >> 8) & 0xFF) - ((start >> 8) & 0xFF));
	int b = (start & 0xFF) + t * ((end & 0xFF) - (start & 0xFF));

	return (r << 16) | (g << 8) | b;
}

void	render_cells(t_cell_data *cd, t_cell *cells)
{
	t_vec2	screen_pos;
	int		clr;
	int		i;
	t_md	*md;
	t_cell	cell;
	float	elapsed;
	float	fade_dur;
	float	t;

	md = cd->md;
	fade_dur = cd->frame_dur * 2.0f;
	i = -1;
	while (++i < cd->cell_len)
	{
		cell = cells[i];
		if (cell.is_hov)
			clr = 0xAAAAAA;
		else if (!cd->has_fade)
			clr = cell.colors[cell.alive];
		else
		{
			elapsed = get_time_in_seconds() - cell.state_change_time;
			if (elapsed >= fade_dur)
				clr = cell.colors[cell.alive];
			else
			{
				t = elapsed / fade_dur;
				clr = lerp_color(cell.colors[cell.alive], cell.colors[!cell.alive], t);
			}
		}
		screen_pos = (t_vec2){cell.x * cd->cell_sz.x + 1, cell.y * cd->cell_sz.y + 1};
		draw_pixels(cd->screen, screen_pos, sub_vec2(cd->cell_sz, v2(1)), clr);
		cells[i].is_hov = 0;
	}
	if (cd->show_keys)
		draw_img(cd->overlay, cd->screen, v2(0), -1);
	mlx_put_image_to_window(md->mlx, md->win, cd->screen->img, 0, 0);
}


int	get_new_cell_state(t_cell_data *cd, int c_i, int rev)
{
	int			row = cd->row;
	t_cell		*cells = cd->cells;
	const int	indexes[8] = {
		c_i - row - 1, c_i - row, c_i - row + 1,
		c_i - 1, c_i + 1,
		c_i + row - 1, c_i + row, c_i + row + 1
	};
	const int x = c_i % row;
	int	i;
	int	live_cells;

	(void)rev;
	live_cells = 0;
	i = -1;
	while (++i < 8)
	{
		int idx = indexes[i];
		if (idx < 0 || idx >= cd->cell_len)
			continue;
		if ((i == 0 || i == 3 || i == 5) && (x == 0))
			continue;
		if ((i == 2 || i == 4 || i == 7) && (x == row - 1))
			continue;
		if (idx > cd->cell_len)
			continue ;
		if (idx < 0)
			continue ;
		if (cells[idx].alive)
			live_cells++;
	}
	if (!cells[c_i].alive)
		return (live_cells == 3);
	return (live_cells == 2 || live_cells == 3);
}

void	set_cell_at_mouse(t_cell_data *cd, t_vec2 win_sz, t_vec2 mouse_p, t_mouse mouse)
{
	const int		should_set = mouse.pressed == MOUSE_PRESS;
	const int		should_erase = should_set && cd->md->key_prs[SHIFT_KEY] == 1;
	const t_vec2	tile_p = div_v2(win_sz, cd->row);
	const t_vec2	mouse_coord = get_v2(mouse_p.x / tile_p.x, mouse_p.y / tile_p.y);
	const t_shape	shape = cd->shapes[cd->pattern_index];
	t_vec2			screen_p;
	int				i;

	i = -1;
	while (++i < shape.cell_len)
	{
		screen_p = add_vec2(mouse_coord, get_v2(shape.cells[i].x, shape.cells[i].y));
		if (screen_p.x < 0 || screen_p.x > cd->row || screen_p.y < 0 || screen_p.y > cd->row)
			continue ;
		int cur_index = cd->row * screen_p.y + screen_p.x;
		if (should_erase)
			cd->cells[cur_index].alive = 0;
		else if (should_set && shape.cells[i].alive)
			cd->cells[cur_index].alive = shape.cells[i].alive;
		cd->cells[cur_index].is_hov = shape.cells[i].alive;
	}
	if (cd->md->mouse.click == MOUSE_PRESS)
		play_sound(cd->md, AU_TUK);
}

void	clear_cells(t_cell_data *cd)
{
	int	i;

	i = -1;
	while (++i < cd->cell_len)
		cd->cells[i].alive = 0;
	cd->update_sim = 0;
	play_sound(cd->md, AU_LOW_CLICK);
}

int	update_cells(t_cell_data *cd, int rev)
{
	int	i;
	int	new_states[cd->cell_len];
	int	new_state;
	int	one_has_changed;

	one_has_changed = 0;
	i = -1;
	while (++i < cd->cell_len)
		new_states[i]  = get_new_cell_state(cd, i, rev);	
	i = -1;
	while (++i < cd->cell_len)
	{
		new_state = new_states[i];
		if (new_state != cd->cells[i].alive)
		{
			cd->cells[i].state_change_time = get_time_in_seconds();
			one_has_changed = 1;
		}
		cd->cells[i].alive = new_states[i];
	}
	cd->md->timer.time++;
	return (one_has_changed);
}

int	update(t_cell_data *cd)
{
	t_timer		*tm;
	t_md		*md;
	int			should_update;

	md = cd->md;
	md->mouse.hide = 0;
	set_cell_at_mouse(cd, cd->screen->size, md->mouse.real, md->mouse);
	tm = &cd->md->timer;
	if (wrap_int(&cd->pattern_index, 0, 4, ((md->key_click == D_KEY) - (md->key_click == A_KEY))))
		update_overlay(cd);
	if (md->key_click == F_KEY)
		cd->has_fade = !cd->has_fade;
	cd->speed_point = minmax(0, 30, cd->speed_point + (md->key_click == W_KEY) - (md->key_click == S_KEY));
	if (md->key_click == W_KEY || md->key_click == S_KEY)
		update_overlay(cd);
	if (md->key_click == H_KEY)
	{
		cd->show_keys = !cd->show_keys;
		update_overlay(cd);
	}
	if (md->key_click == ENTER_KEY)
	{
		cd->update_sim = !cd->update_sim;
		update_overlay(cd);
		play_sound(cd->md, AU_LOW_CLICK);
	}
	else if (md->key_click == R_KEY)
		clear_cells(cd);
	cd->frame_dur = (double)(maxf(0.01f, cd->speed_point * .1f));
	upd_timer(&tm->tm_anim, get_time_in_seconds(), cd->frame_dur, &tm->trig_anim);
	should_update = cd->update_sim && tm->trig_anim && md->mouse.pressed == MOUSE_NOPRESS;
	if (md->key_click == SPACE_KEY)
	{
		should_update = 1;
		cd->update_sim = 0;
	}
	if (should_update && !update_cells(cd, cd->reverse || md->key_click == A_KEY))
	{
		update_overlay(cd);
		cd->update_sim = 0;
	}
	if (should_update || md->mouse.delta.x || md->mouse.delta.y || md->key_click)
		render_cells(cd, cd->cells);
	reset_mlx_values(cd->md);
	return (1);
}

void	init_cell_data(t_cell_data *cd, t_vec2 win_sz, int cell_amount)
{
	int			i;
	t_vec2		pos;
	const int	row = cell_amount / 4;

	cd->row = row;
	cd->cell_sz = div_v2(win_sz, row);
	cd->cell_sz.x = max(2, cd->cell_sz.x);
	cd->cell_sz.y = max(2, cd->cell_sz.y);
	cd->cell_len = row * row;
	cd->cells = malloc(sizeof(t_cell) * (row * row));
	cd->cell_clr[0] = _WHITE;
	cd->cell_clr[1] = _BLACK;
	i = 0;
	pos = v2(-1);
	while (++pos.y < row)
	{
		pos.x = -1;
		while (++pos.x < row)
			cd->cells[i++] = (t_cell){pos.x, pos.y, 0, 0, 0, {cd->cell_clr[0], cd->cell_clr[1]}};
	}
}

void	init_shape(t_shape *p, t_vec2 size, const char *pattern, const char *label)
{
	t_cell		*c;
	int			i;
	t_vec2		pos;
	int			cell_i;

	p->size = size;
	p->cell_len = size.x * size.y;
	c = malloc(sizeof(t_cell) * p->cell_len);
	pos = v2(0);
	cell_i = 0;
	i = -1;
	while (pattern[++i])
	{
		if (pattern[i] == '\n')
		{
			pos.x = 0;
			pos.y++;
			continue ;
		}
		if (cell_i >= p->cell_len)
			break ;
		c[cell_i++] = (t_cell){pos.x++, pos.y, pattern[i] == '#', 0, 0, {0, 0}};
	}
	p->cells = c;
	p->name = label;
}

void	init_shapes(t_cell_data *cd)
{
	const int	pattern_len = 5;
	const t_vec2	sizes[5] = {
		{1, 1}, // Block
		{3, 3}, // Blinker
		{3, 3}, // Glider
		{4, 4}, // Toad
		{4, 4}  // Beacon
	};
	const char	*labels[5] = {"Block", "Blinker", "Glider", "Toad", "Beacon"};
	const char *shapes[5] = {
		// Block (stable)
		"#",
		// Blinker (oscillator)
		".#.\n"
		".#.\n"
		".#.\n",
		// Glider (spaceship)
		".#.\n"
		"..#\n"
		"###\n",
		// Toad (oscillator)
		".###\n"
		"###.\n"
		"....\n"
		"....\n",
		// Beacon (oscillator)
		"##..\n"
		"##..\n"
		"..##\n"
		"..##\n"
	};
	int	i = -1;
	while (++i < pattern_len)
		init_shape(&cd->shapes[i], sizes[i], (char *)shapes[i], labels[i]);
}

float get_brightness(t_vec4 rgba)
{
	return (rgba.r + rgba.g + rgba.b) / 3.0f;
}

int	get_rev_color(t_vec4 rgba)
{
	return (v4_to_color(255 - rgba.r, 255 - rgba.g, 255 - rgba.b, rgba.a));
}

void	set_cells_to_image(t_cell_data *cd, char *path)
{
	t_image	*img;
	t_vec2	pos;
	int		img_index;
	t_vec4	rgba_center, rgba_right, rgba_down;
	float	brightness_center, brightness_right, brightness_down;
	const float threshold = 30.0f;

	img = init_abs_img(cd->md, get_v2(cd->row, cd->row), path);
	pos = v2(-1);
	while (++pos.y < img->size.y)
	{
		pos.x = -1;
		while (++pos.x < img->size.x)
		{
			img_index = (pos.y * (img->size_line / 4) + pos.x);
			rgba_center = color_to_v4(img->src[img_index]);
			if (0)
			{
				brightness_center = get_brightness(rgba_center);

				float diff = 0;
				if (pos.x + 1 < img->size.x)
				{
					rgba_right = color_to_v4(img->src[pos.y * (img->size_line / 4) + (pos.x + 1)]);
					brightness_right = get_brightness(rgba_right);
					diff += fabsf(brightness_center - brightness_right);
				}
				if (pos.y + 1 < img->size.y)
				{
					rgba_down = color_to_v4(img->src[(pos.y + 1) * (img->size_line / 4) + pos.x]);
					brightness_down = get_brightness(rgba_down);
					diff += fabsf(brightness_center - brightness_down);
				}
				if (diff > threshold)
					cd->cells[pos.y * cd->row + pos.x].alive = 1;
			}
			cd->cells[pos.y * cd->row + pos.x].colors[cd->cells[pos.y * cd->row + pos.x].alive] = img->src[img_index];
			cd->cells[pos.y * cd->row + pos.x].colors[!cd->cells[pos.y * cd->row + pos.x].alive] = get_rev_color(rgba_center);
		}
	}
}

int	main(int argc, char *argv[])
{
	const t_vec2	win_sz = (t_vec2){800, 800};
	const int		cell_amount = 800;
	t_md			md;
	t_cell_data		cd;

	init_wrapper(&md, win_sz, "Game of life", 10);
	cd.md = &md;
	init_cell_data(&cd, win_sz, cell_amount);
	if (argc == 2)
		set_cells_to_image(&cd, argv[1]);
	md.prm.au_on = 1;
	cd.bgr_clr = v4_to_color(200, 200, 200, 255);
	cd.screen = init_img(&md, win_sz, NULL, cd.bgr_clr);
	cd.update_sim = 0;
	cd.reverse = 0;
	cd.speed_point = 0;
	cd.pattern_index = 0;
	cd.has_fade = 0;
	cd.show_keys = 1;
	init_shapes(&cd);
	cd.overlay = init_img(&md, get_v2(md.prm.txt_sc * 25, md.prm.txt_sc * 5), NULL, cd.bgr_clr);
	update_overlay(&cd);
	printf("cell len: %d rows: %d\n", cd.cell_len, cd.row);
	mlx_hook(md.win, 2, 0, handle_key_press, &md);
	mlx_hook(md.win, 3, 0, handle_key_release, &md);
	mlx_hook(md.win, 17, 0, close_window, &md);
	mlx_loop_hook(md.mlx, update, (void *)&cd);
	mlx_loop(md.mlx);
}
