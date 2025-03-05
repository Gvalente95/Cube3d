/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 16:32:42 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/05 14:18:16 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_UTILS_H
# define MLX_UTILS_H

# include "headers/vectors.h"
# include "headers/entity.h"
# include "headers/colors.h"
# include "headers/Keys.h"
# include "headers/game.h"

# include "libft/libft.h"
# include "mlx/mlx.h"
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include <time.h>
# include <fcntl.h>

# define TXT_COL				-1
# define BUTTON_BGRND_COL		BLACK

# define PART_BASE_SIZE			35

# define SCRN_W 1400
# define SCRN_H 800

# define CELL_SIZE 60
# define EPSILON 0.00001f

# define B_Y 50
# define B_X 100
# define TXT_SCALE	20
# define READ_BUFFER_SIZE 	50

# define PRT_PNGS_LEN		6
# define PRT_AMOUNT			500

typedef struct s_input
{
	char	*buffer;
	int		index;
}	t_input;

typedef struct s_image_data
{
	t_vec2	pos;
	t_vec2	size;
	void	*dest;
	double	x_ratio;
	double	y_ratio;
	int		*src_data;
	int		*dst_data;
	int		bpp;
	int		len;
	int		bps;
	int		endian;
	int		*scl_d;
}	t_image;

typedef struct s_md
{
	t_gs		gst;
	t_map_d		mp_d;
	t_timer		timer;
	t_prt		**particles;
	t_map		map;
	void		*sky;
	void		*floor;
	void		*mlx;
	void		*win;
	void		*cursor;
	void		*cursor_detect;
	void		*cursor_grab;
	void		*menu_bgrnd;
	void		*bgrnd_img;
	void		**env_images;
	void		**prt_img;
	void		**wall_txtr;
	void		*minimap;
	t_ent		*hpbar;
	t_ent		*key;
	t_ent		*pickaxe;
	t_ent		*bgrnd;
	t_ent		**images;
	t_ent		**star_icon;
	t_ent		**bg_env;
	t_ent		plr;
	t_ent		*exit;
	t_ent		*selected;
	t_ent		**all_images;
	t_vec2		mmap_size;
	int			mmap_ic_size;
	int			show_minimap;
	t_vec3		mouse_pos;
	t_vec3		mouse_prv_pos;
	t_vec3		mouse_world_pos;
	t_vec2		win_size;
	t_vec2		mouse_grid_pos;
	t_vec3f		cam_offset;
	t_vec2		prt_base_size;
	pid_t		spike_sound;
	pid_t		bgrnd_au;
	pid_t		bgrnd_mus;
	int			debug_mode;
	int			key_prs[512];
	int			key_clicked;
	int			mouse_pressed;
	int			mouse_clicked;
	int			time;
	int			total_coins;
	int			total_stars;
	int			death_amount;
	int			music_index;
	int			is_typing;
	int			wall_hug;
	int			coin_au_timer;
	int			jump_timer;
	int			particles_alive;
	int			index;
	int			move_counter;
	int			cur_category;
	int			has_key;
	int			coins_left;
	int			images_len;
	int			bg_env_len;
	int			t_len;
	int			floor_color;
	int			sky_color;
	char		*cwd;
}	t_md;


t_vec2	get_v2_grid_pos(t_md *md, t_vec2 pos);
t_vec3	get_grid_pos(t_md *md, t_vec3 pos);

//		KEYCODE.c
char	map_keycode_to_char4(int keycode);
char	map_keycode_to_char3(int keycode);
char	map_keycode_to_char2(int keycode);
char	map_keycode_to_char(int keycode);

//		TOOLS.c
int		r_range(int min, int max);
void	*flip_image_x(t_md *md, void *img, t_vec2 size);
int		render_img(t_md *md, void *img, t_vec3 pos, t_vec2 offset);

//		INPUT_MOUSE.c
int		mouse_event_handler(int button, int x, int y, void *param);
int		mouse_release_handler(int button, int x, int y, void *param);
int		mouse_motion_handler(int x, int y, void *param);
int		update_mouse(t_md *md);

//		FREE_c.c
int		free_button(t_md *md, t_but *but);
int		free_buttons(t_md *md);
int		free_gst(t_md *md);

//		TIME2.c
void	stop_timer(t_timer *timer);
void	resume_timer(t_timer *timer);
double	get_total_time(t_timer *timer);
void	handle_timers(t_md *md, t_timer *tm);

//		INPUT.c
int		handle_key_press(int keycode, t_md *md);
int		handle_key_release(int keycode, t_md *md);
void	reset_mlx_values(t_md *md);
int		close_window(t_md *md);

//		TIME.c
double	get_time_in_seconds(void);
void	init_time_data(t_md *md, t_lvl_data *data);
double	check_timer(double timer);
void	start_timer(double *timer);
void	stop_timer(t_timer *timer);

//		FREE_b.c
int		free_void(void *elem);
int		free_void_array(void **elements, int i);
int		free_particles(t_md *md, t_prt ***prts);
int		free_player_animations(t_ent *plr, t_md *md);
int		free_md2(t_md *md, int free_count);

//		IMAGES_b.c
void	*set_img_color(void *frame, t_vec2 size, int col, float str);
void	set_transparency(void *src, void *dest, t_vec2 size, float trnsp);
void	*split_img_at_x(void *img, t_vec2 img_size, int cutoff, t_dir dir);
void	*get_image_copy(t_md *md, void *src, t_vec2 src_size);
void	*add_img(char *relative_path, int *width, int *height, t_md *md);
void	render_cursor(t_md *md, int has_hov);

//		INIT_WRAPPER.c
int		init_md(t_md *md);
void	init_wrapper(t_md *md, t_vec2 win_size, char *win_name, int row_amount);

//		IMAGES_a.c
void	set_new_size(t_image *q, t_vec2 *old_size, t_vec2 *new_size);
void	*scale_img(t_md *md, void *img, t_vec2 *old_size, t_vec2 new_size);
void	*scale_abs_img(t_md *md, void *img, t_vec2 *old_size, t_vec2 new_size);

//		FREE_a.c
void	free_vec2_array(t_vec2 **array);
void	free_vec3_array(t_vec3 **array);
void	free_vec4_array(t_vec4 **array);
int		free_images(t_md *md, void ***images);
int		free_ent(t_ent **ent);
int		free_ents(t_ent ***ents);
int		free_md(t_md *md, int quit);

//		AUDIO.c
pid_t	play_random_sound(const char *path, int len, const char *format);
int		mute_unmute_audio(int mute);
pid_t	play_sound(const char *filename, int loop);
pid_t	play_index(const char *filepath, int index);
void	stop_sound(pid_t pid);
int		is_audio_playing(pid_t pid);

//		COLORS.c
int		get_trgb(unsigned char t, unsigned char r, \
	unsigned char g, unsigned char b);
unsigned char	get_t(int trgb);
unsigned char	get_r(int trgb);
unsigned char	get_g(int trgb);
unsigned char	get_b(int trgb);
t_vec3	get_grid_posf(t_md *md, t_vec3f pos);

#endif