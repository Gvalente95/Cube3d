/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:30:54 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/03 09:59:00 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "../mlx_utils.h"

typedef struct s_image_data	t_image;

typedef struct s_time_handler
{
	double	game_start;
	double	shoot_timer;
	double	delta_time;
	double	prev_time;
	double	cur_tm;
	double	tm_anim;
	double	tm_level;
	double	tm_fe;
	double	tm_menu;
	double	tm_walk;
	double	elapsed_pause;
	double	no_portal_cooldown;
	double	trg[3];
	double	fe_time;
	int		stars;
	int		is_game_running;
	int		time;
	int		fps;
	int		trig_anim;
	int		trig_fe;
	int		trig_walk;
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
	t_vec2			pos;
	char			label[50];
	int				*value;
	int				active;
	unsigned int	key_trigger;
	char			key_show[5];
}	t_button;

typedef struct s_menu
{
	t_image			*overlay;
	t_image			*freeze_frame;
	t_button		buttons[20];
	t_slider		sliders[50];
	t_slider		*selected_slider;
	int				slider_fill_clr;
	int				slider_bgr_clr;
	int				bgr_color;
	int				menu_time;
	int				slider_index;
	int				refresh_bg;
	int				refresh_ui;
	int				slider_hov;
	int				button_hov;
	int				active;
}	t_menu;

#endif