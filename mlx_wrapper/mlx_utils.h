/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 16:32:42 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/13 05:15:51 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_UTILS_H
# define MLX_UTILS_H

# include "headers/vectors.h"
# include "headers/entity.h"
# include "headers/colors.h"
# include "headers/game.h"

# include "libft/libft.h"
# include "../lists/lists.h"
# include "mlx_linux/mlx.h"
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include <time.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <X11/X.h>

# ifdef IS_LINUX
#  define LIN 1
#  define IMG_PATH         "xpm"
#  define IMG_FORMAT       ".xpm"
#  define CURSOR_SPR_PATH  "xpm/utils/cursor/default.xpm"
#  define CURS_DTC_PATH    "xpm/utils/cursor/hand_open.xpm"
#  define CURS_GRB_PATH    "xpm/utils/cursor/hand_closed.xpm"
#  define FONT_SPRITE_PATH "xpm/utils/font/"
#  include "headers/Keys_lnx.h"
# else
#  define LIN 0
#  define IMG_PATH         "png"
#  define IMG_FORMAT       ".png"
#  define CURSOR_SPR_PATH  "png/utils/cursor/default.png"
#  define CURS_DTC_PATH    "png/utils/cursor/hand_open.png"
#  define CURS_GRB_PATH    "png/utils/cursor/hand_closed.png"
#  define FONT_SPRITE_PATH "png/utils/font/"
#  include "headers/Keys_mac.h"
# endif

typedef struct s_input
{
	char	*buffer;
	int		index;
}	t_input;

typedef struct s_image_data
{
	t_vec2	size;
	t_vec2	pos;
	char	*data;
	void	*dest;
	double	x_ratio;
	double	y_ratio;
	int		*src_data;
	int		*dst_data;
	int		size_line;
	int		bpp;
	int		len;
	int		bps;
	int		endian;
	int		*scl_d;
}	t_image;

# define MAX_RAYS 	10000
# define ENT_FRAMES_MAX	5

typedef struct s_mmap
{
	void		*bgrnd;
	void		*plr_icon;
	t_vec2		size;
	int			ic_scl;
	int			active;
	int			mray_len;
	t_vec3f		ray_pos[MAX_RAYS];
}	t_mmap;

typedef struct s_ray
{
	t_vec3f	hit;
	t_vec3f	start;
	t_vec3f	pos;
	t_vec3f	dir;
	float	distance;
	float	angle;
	float	median;
	int		hit_wall_index;  	// 1 si mur vertical, 0 si horizontal
	int		hit_vrt;  	// 1 si mur vertical, 0 si horizontal
	t_vec3f	side_dst; 		// Distance jusqu’à la prochaine ligne verticale - horizontale
	t_vec3f	delta_dst; 	// Distance entre deux lignes verticales - horizontales
	t_vec2	step_p; 		// Direction du pas en X-Y (-1 ou 1)
	int		index;			// index du rayon et index de la colonne de pixel qu'on va render
	int		color;
	t_ent	*found_e;
	t_vec3f	pos_at_e;
	int		hit_vrt_at_e;
}	t_ray;

typedef struct s_md
{
	t_gs		gst;
	t_map_d		mp_d;
	t_timer		timer;
	t_map		map;
	t_mmap		mmap;
	t_ray		rays[MAX_RAYS];
	t_dblst		*entities;
	t_dblst		*particles;
	t_ent		plr;
	t_vec2		win_size;
	t_vec2		e_sizes[ENT_TYPE_LEN];
	t_vec2		e_sizes_2d[ENT_TYPE_LEN];
	t_vec3f		input_mov;
	t_vec3f		mouse_pos;
	t_vec3f		mouse_prv_pos;
	t_vec3f		mouse_delta;
	t_vec3f		mouse_world_pos;
	t_vec2		mouse_grid_pos;
	t_vec3f		cam_ofst;
	t_vec3f		wrd_mv_offst;
	t_vec3f		plr_wrd_mv;
	pid_t		bgrnd_au;
	pid_t		bgrnd_mus;
	t_image		bgrnd_img_data;
	const char	*ents_tp_map[1];
	const char	*ents_tp_names[ENT_TYPE_LEN];
	const char	*ents_act_names[ENT_ACTION_LEN];
	const char	*dir_labels[4];
	char		base_map_path[50];
	char		img_dir_path[20];
	char		img_format[20];
	void		*sky;
	void		*floor;
	void		*mlx;
	void		*win;
	void		*center;
	void		*cursor;
	void		*curs_dtc;
	void		*curs_grb;
	void		*bgrnd_img;
	void		**prt_img;
	void		**wall_txtr;
	void		**wall_txtr_2d;
	void		**txtr_2d;
	void		****e_frms;
	int			(*mlx_put)(void *mlx, void *win, void *img, int x, int y);
	void		*(*mlx_make)(void *mlx, char *name, int *with, int *height);
	int			is_linux;
	int			rgb[17];
	int			key_prs[512];
	int			txt_scale;
	int			size_2d;
	int			init_steps;
	int			mouse_focus;
	int			show_rays;
	int			ray_mode;
	int			debug_mode;
	int			key_clicked;
	int			mouse_pressed;
	int			mouse_clicked;
	int			mouse_hide;
	int			time;
	int			death_amount;
	int			row_amount;
	int			t_len;
	int			floor_color;
	int			sky_color;
	int			fps;
	int			prv_fps;
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
int		free_void_array(void **elements);
int		free_particles(t_md *md, t_prt ***prts);
int		free_player_animations(t_ent *plr, t_md *md);
int		free_md2(t_md *md, int free_count);

//		IMAGES_b.c
void	*set_img_color(void *frame, t_vec2 size, int col, float str);
void	set_transparency(void *src, void *dest, t_vec2 size, float trnsp);
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
int		free_vec2_array(t_vec2 **array);
int		free_vec3_array(t_vec3 **array);
int		free_vec4_array(t_vec4 **array);
int		free_images(t_md *md, void ***images, char *label);
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
t_vec3	get_grid_posf(t_md *md, t_vec3f pos);
int		str_to_color(const char *line);
void	update_ent_frame(t_md *md, t_ent *e);

#endif