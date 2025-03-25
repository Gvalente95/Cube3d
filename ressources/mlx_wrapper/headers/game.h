/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:30:54 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 16:10:29 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "../mlx_utils.h"

typedef struct s_image_data t_image;

typedef enum e_gamestate
{
	in_game,
	strt_men,
	main_men,
	map_men,
	load_men,
	opt_men,
	edit_men,
	cred_men,
	end_men,
	gamestate_count,
}	t_gamestate;

typedef struct s_time_handler
{
	double	game_start;
	double	shoot_timer;
	double	delta_time;
	double	prev_time;
	double	current_time;
	double	anim_timer;
	double	level_timer;
	double	menu_timer;
	double	elapsed_pause;
	double	trg[3];
	int		stars;
	int		is_game_running;
	int		time;
	int		fps;
	int		prv_fps;
	int		avrg_fps;
}	t_timer;

typedef struct s_map
{
	t_vec2	size;
	t_vec3	pos;
	char	*buffer;
	char	*name;
	int		len;
	int		coins_amount;
	int		coins_left;
	int		tries_amount;
	int		difficulty;
}	t_map;

typedef enum e_button_fct
{
	back,
	quit,
	edit_cur,
	edit_new,
	load,
	load_next,
	load_name,
	ch_mus,
	mute_swch,
	load_ind,
	dec_page,
	inc_page,
	t_button_fct_count,
}	t_btn_fct;

typedef struct s_slider
{
	t_image		*img;
	t_vec3f		limits;
	t_vec2		pos;
	char		label[50];
	float		*value;
	int			point;
	int			steps;
	int			active;
	int			base_point;
}	t_slider;

typedef struct s_menu_button
{
	t_image		*img;
	t_vec2		pos;
	t_btn_fct	function;
	char		label[50];
	int			*value;
	int			active;
}	t_button;

typedef struct s_menu
{
	t_image			*overlay;
	t_image			*freeze_frame;
	t_gamestate		prv_state;
	t_gamestate		state;
	t_button		buttons[9];
	t_slider		sliders[50];
	t_slider		*selected_slider;
	int				slider_fill_clr;
	int				slider_bgr_clr;
	const char		*menu_titles[9][50];
	int				bgr_color;
	int				pages_amount[9];
	int				cur_page[9];
	int				menu_time;
	int				slider_index;
	int				refresh;
	int				hov;
	int				active;
}	t_menu;

#endif