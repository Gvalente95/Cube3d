/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 16:32:42 by gvalente          #+#    #+#             */
/*   Updated: 2025/04/07 22:52:14 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_UTILS_H
# define MLX_UTILS_H

# include "headers/vectors.h"
# include "headers/entity.h"
# include "headers/colors.h"
# include "headers/game.h"
# include "headers/rays.h"
# include "headers/textures.h"
# include "headers/math.h"
# include "headers/math_2.h"
# include "headers/audio.h"
# include "headers/threads.h"

# include "libft/libft.h"
# include "../lists/lists.h"
# include "mlx_linux/mlx.h"
# include "mlx_mac/mlx.h"
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include <time.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
//# include <X11/X.h>

# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

# ifdef IS_LINUX
#  define LIN 1
#  include "headers/Keys_lnx.h"
# else
#  define LIN 0
#  include "headers/Keys_mac.h"
# endif

# define MOUSE_SENSITIVITY .2
# define MAPPED_ENT_MAX	100

typedef struct s_mmap
{
	t_image		*bg;
	t_image		*img;
	t_image		*sphere;
	t_image		*full_map;
	t_vec2		size;
	int			cmps;
	int			comps_scl;
	int			ic_scl;
	int			active;
	int			mray_len;
	int			bgr_color;
	int			revealed_cur;
	int			revealed_len;
}	t_mmap;

typedef struct s_hud
{
	t_image		*overlay;
	t_image		*rgun;
	t_image		*base_sky;
	t_image		*sky;
	t_image		*sky_flipy;
	t_image		*floor;
	t_image		*wall;
	t_image		*ceiling;
	t_image		*floor2d;
	t_image		*lock_x_icon;
	t_image		*lock_y_icon;
	t_image		*amm_icon;
	t_image		*amm2_icon;
	t_image		*key_icon;
	t_image		*key2_icon;
	t_image		*hp_icon;
	t_image		*center;
	int			unlocked_weapons[WEAPON_TYPE_LEN];
	int			bgr_color;
	int			floor_color;
	int			sky_color;
	int			fog_color;
	int			floor_start;
	int			wpn_index;
	int			weapon_frame;
	int			ammo;
	int			keys;
	int			hp;
}	t_hud;

typedef struct s_post_fx_data
{
	t_vec4f			hue;
	t_image			*vignette;
	unsigned int	palette[11];
	int				palette_size;
	float			rgb_distortion;
	float			scanlines;
	float			dithering;
	float			bloom_threshold;
	float			color_band;
	float			chrom_amount;
	float			barrel_amount;
	float			glow_intensity;
	float			fog;
	float			noise;
	int				anti_alias;
}	t_fx_data;

typedef struct s_parameters
{
	int				resolution;
	int				show_rays;
	int				view_2d;
	int				debug_mode;
	int				use_thrd;
	int				show_fps;
	int				fly_cam;
	int				ent_mode;
	int				max_view_sprite;
	int				au_on;
	int				use_grass;
	int				use_ceiling;
	int				use_floor;
	int				super_view;
	int				use_sky;
	float			bob_amount;
	float			sun_x;
	float			sun_y;
	float			grass_w;
	float			fe_speed;
	float			ray_depth;
	float			height;
	float			plr_speed;
	float			rot_speed;
	float			res_value;
	float			txt_sc;
	float			zoom;
	float			floor_glide;
	float			fov;
	float			win_x;
	float			floor_fov;
	float			win_y;
}	t_parameters;

typedef struct s_mouse
{
	t_image			*cursor;
	t_image			*curs_dtc;
	t_image			*curs_grb;
	t_vec3f			pos;
	t_vec2			real;
	t_vec2			prev;
	t_vec2			delta;
	t_vec2			world;
	t_vec2			grid_pos;
	t_vec2			lock_rot;
	int				focus;
	int				pressed;
	int				click;
	int				locked;
	int				hide;
}	t_mouse;

typedef struct s_cam
{
	t_vec3f			input_mov;
	t_vec2			input_offst;
	t_vec3f			rot;
	t_vec3f			pos;
	t_vec3f			ofst;
	t_vec3f			wrd_mv_offst;
	t_vec3f			plr_wrd_mv;
	t_ent			*pointed;
	float			bob_time;
	int				is_moving;
	int				x_dir_start;

}	t_cam;

typedef struct s_text_data
{
	int		x;
	int		y;
	int		color;
	int		scale;
	t_image	*onto;
}	t_txtd;

typedef struct s_md
{
	void			*mlx;
	void			*win;
	t_image			*screen;
	t_vec2			win_sz;
	t_ent			plr;
	t_ent			*map_ents[MAPPED_ENT_MAX][MAPPED_ENT_MAX];
	t_dblst			*entities;
	t_hud			hud;
	t_menu			menu;
	t_map			map;
	t_portal		portal;
	t_mmap			mmap;
	t_autocam		autocam;
	t_timer			timer;
	t_fx_data		fx;
	t_mouse			mouse;
	t_ray			rays[MAX_RAYS];
	t_texture_data	txd;
	t_cam			cam;
	t_parameters	prm;
	t_thrd_manager	thrd_manager;
	t_au_manager	au;
	t_env_manager	env;
	char			base_map_path[50];
	unsigned int	r_seed;
	int				key_prs[65536];
	int				key_clicked;
	int				last_key;
	int				init_steps;
	int				rgb[20];
	int				t_len;
	int				var;
	int				score;
	int				is_linux;
	int				(*mlx_put)(void *mlx, void *win, void *img, int x, int y);
	void			*(*mlx_make)(void *mlx, char *name, int *with, int *height);
}	t_md;

//		input/input_tools.c
void			wrap_mouse(t_md *md, int delta_x, int delta_y);
void			set_mouse_lock(t_md *md, int lock);

//		input/input_mouse.c
int				mouse_event_handler(int button, int x, int y, void *param);
int				mouse_release_handler(int button, int x, int y, void *param);
int				mouse_motion_handler(int x, int y, void *param);
int				update_mouse(t_md *md);

//		input/input.c
int				handle_key_press(int keycode, t_md *md);
int				handle_key_release(int keycode, t_md *md);
void			reset_mlx_values(t_md *md);
int				close_window(t_md *md);

//		free/free_image.c
int				free_image_data(t_md *md, t_image *img_data);
int				free_images_data(t_md *md, t_image **images, const char *label);
int				free_images_array(t_md *md, t_image ***arr, const char *label);
int				free_mob_images(t_md *md, t_ent *e, char *label);

//		free/free_elements.c
int				free_hud(t_md *md, t_hud *hud);
int				free_txd(t_md *md, t_texture_data *txd);
int				free_var(t_md *md, t_mmap *mmap, t_fx_data *fx, t_mouse *mouse);
int				free_menu(t_md *md, t_menu *menu);
int				free_ents(t_md *md);

//		free/free.c
int				safe_free(void *item);
int				free_void_array(void **elements);
int				free_void(void *elem);
int				free_md(t_md *md, int quit);
int				free_and_quit(t_md *d, const char *msg, const char *attribute);

//		time/time.c
void			stop_timer(t_timer *timer);
void			resume_timer(t_timer *timer);
double			get_total_time(t_timer *timer);
double			get_time_in_seconds(void);
double			check_timer(double timer);
void			start_timer(double *timer);
void			init_timer(t_md *md, t_timer *timer);
void			reset_fps_timer(t_timer *timer);

//		math/math_tools_3.c
int				r_range_seed(unsigned int *g_seed, int min, int max);
int				r_range(int min, int max);
float			f_range(float min, float max);

//		init_wrapper.c
unsigned int	get_r_seed(void);
int				init_screen(t_md *md, t_vec2 win_sz, int res, char *win_name);
int				init_md(t_md *md);
void			init_os_params(t_md *md);
void			init_wrapper(t_md *md, t_vec2 win_sz, char *win_name, int res);

//		string/string_3.c
int				same_str(const char *a, const char *b);
int				char_in_str(char c, const char *txt);
int				chr_amnt(const char *str, char c);
int				get_arr_len(void **arr);
char			*truncate_at_end(const char *str, char cut_letter);

//		string/string.c
char			*md_strjoin(t_md *d, char const *s1, char const *s2);
char			*md_strdup(t_md *d, const char *s1);
void			*md_malloc(t_md *d, ssize_t size);
void			*md_realloc(t_md *d, void *ptr, size_t new_size);
char			*md_strndup(t_md *d, const char *s1, ssize_t n);

//		string/string_2.c
void			setstr(char **str, char *new);
int				only_contains(char *str, char *to_contain);
char			*ft_megajoin(const char *a, const char *b, \
	const char *c, const char *d);
int				get_char_index(const char *str, char to_check);
char			*ft_strndup(const char *s1, ssize_t n);

t_vec3			get_grid_posf(t_md *md, t_vec3f pos);
t_vec3			get_grid_pos(t_md *md, t_vec3 pos);

void			show_vec2(t_md *md, char *label, t_vec2 vec, t_vec2 pos);
void			show_float(t_md *md, char *label, float value, t_vec2 pos);
void			show_int(t_md *md, char *label, int value, t_vec2 pos);
void			show_vec3f(t_md *md, char *label, t_vec3f vec, t_vec2 pos);
void			show_vec3(t_md *md, char *label, t_vec3 vec, t_vec2 pos);
void			show_vec2(t_md *md, char *label, t_vec2 vec, t_vec2 pos);
int				is_in_screen(t_md *md, t_vec3 pos, t_vec2 size);
int				ent_in_bounds(t_ent *ent, t_ent *bounds);
void			upd_timer(double *tmr, double cur_tm, double incr, int *event);
void			update_fe(t_md *md, t_vec2 start, \
	t_fe *fe, t_floor_draw_d d);

//				camera.c
void			update_cam(t_md *md, t_cam *cam);
t_vec3f			update_fly_cam(t_md *md, t_cam *cam, float spd);
int				color_diff(int c1, int c2);
void			clean_img(t_image *img);

#endif