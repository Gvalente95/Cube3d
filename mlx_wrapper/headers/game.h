/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:30:54 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/01/15 22:07:42 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "../mlx_utils.h"

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
	double	level_timer;
	double	menu_timer;
	double	elapsed_pause;
	double	trg[3];
	int		stars;
	int		is_game_running;
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

typedef struct s_menu_button
{
	t_timer		time_data;
	char		*label;
	int			color;
	int			show;
	t_gamestate	nav;
	t_btn_fct	fct;
	void		*action;
	t_vec3		pos;
	t_vec2		size;
	void		*clk_bgr;
	void		*unact_bgr;
	void		*hov_bgr;
	void		*act_bgr;
	int			sound_played;
	int			active;
}	t_but;

typedef struct s_gst
{
	t_gamestate		prv_state;
	t_gamestate		state;
	t_but			***buttons;
	char			*menu_titles[9];
	int				pages_amount[9];
	int				cur_page[9];
	int				menu_time;
	int				g_menu_started;
	int				load_lbl_indx;
	int				active;
}	t_gs;

typedef struct s_lvl_data
{
	int		coins_collected;
	int		star_collected;
	double	best_time;
	double	trg_timer[3];
	char	*level_name;
	char	*path_name;
}	t_lvl_data;

typedef struct s_world_data
{
	t_lvl_data	**lvl_data;
	int			secret_lvl_index;
	int			stars_clcted;
	int			lvl_count;
	int			stars_req;
	char		*name;
	char		*path;
}	t_wrd_data;

typedef struct s_mp_d
{
	t_wrd_data		**wrd_data;
	char			**wrld_names;
	int				worlds_amount;
	int				cur_wrld_index;
	int				cur_lvl_index;
	int				men_sel_ind;
	t_lvl_data		*cur_lvl;
}	t_map_d;


#endif