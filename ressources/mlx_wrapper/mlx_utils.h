/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 16:32:42 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/25 14:03:06 by giuliovalen      ###   ########.fr       */
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
//# include "mlx_linux/mlx.h"
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

# define MAX_RAYS 			10000
# define ENT_FRAMES_MAX		20
# define MOUSE_SENSITIVITY .2
# define ANIM_REFRESH		.1
# define MAX_RAY_SPRITE	10

typedef struct s_draw_data
{
	t_image	*src;
	t_image	*dst;
	t_vec2	src_pos;
	t_vec2	dst_pos;
	t_vec2	pos;
	float	blend_factor;
	int		recolor;
}	t_draw_data;

typedef struct s_image_data
{
	t_vec2	size;
	t_vec2	pos;
	char	path[20];
	char	*addr;
	void	*img;
	int		*src;
	int		size_line;
	int		bpp;
	int		bps;
	int		endian;
	int		*scl_d;
}	t_image;

typedef struct s_mmap
{
	t_image		*bg;
	t_image		*img;
	t_image		*sphere;
	t_image		*arrow;
	t_vec3f		ray_pos[MAX_RAYS];
	t_vec2		size;
	int			collapsed;
	int			ic_scl;
	int			collaps_scl;
	int			active;
	int			mray_len;
	int			bgr_color;
}	t_mmap;

typedef struct s_hit_data
{
	t_ent	*hit;
	t_vec3f	post_at_hit;
	float	dist_at_e;
	int		vertical_hit_at_e;
}	t_hit_data;

typedef struct s_ray
{
	t_hit_data	hit_data[MAX_RAY_SPRITE];
	int			hits_len;
	t_ent		*wall_hit;
	t_ent		*check_hit;
	t_vec3f		pos;
	t_vec3f		start;
	t_vec3f		dir;
	float		distance;
	float		angle;
	int			vertical_hit;
	int			index;
	int			color;
	int			had_door;
	int			steps;
	int			check_steps;
	int			teleported_once;
	int			vrcl;
}	t_ray;

typedef struct s_hud
{
	t_image		*sky;
	t_image		*sky_flipy;
	t_image		*floor;
	t_image		*rgun;
	t_image		*base_sky;
	t_image		*base_floor;
	t_image		*overlay;
	t_image		*lock_x_icon;
	t_image		*lock_y_icon;
	t_image		*amm_icon;
	t_image		*amm2_icon;
	t_image		*key_icon;
	t_image		*key2_icon;
	t_image		*hp_icon;
	int			unlocked_weapons[WEAPON_TYPE_LEN];
	int			bgr_color;
	int			floor_color;
	int			sky_color;
	int			floor_start;
	int			new_floor_start;
	int			active_background;
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
	float			glitch_intensity;
	float			glow_intensity;
	float			fog;
	int				anti_alias;
}	t_post_fx_data;

typedef struct s_parameters
{
	float			difficulty;
	float			ray_depth;
	int				resolution;
	float			audio_volume;
	float			height;
	int				show_rays;
	int				real_mode;
	int				debug_mode;
	float			plr_speed;
	float			rot_speed;
	float			res_value;
	float			txt_scale;
	float			zoom;
	float			fov;
	int				ent_mode;
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
	t_vec2			lock_rotation;
	int				focus;
	int				pressed;
	int				click;
	int				locked;
	int				hide;
}	t_mouse;

typedef struct s_portal_gun
{
	t_vec2		entrance_pos;
	t_vec2		exit_pos;
	t_ent		*entrance;
	t_ent		*exit;
	t_wrd_dir	entrance_dir;
	t_wrd_dir	exit_dir;
	int			last_shot_exit;
	t_ent		*last_passage;
}	t_portal_gun;

typedef struct s_texture_data
{
	t_vec2			e_sizes[ENT_TYPE_LEN];
	t_vec2			e_sizes2d[ENT_TYPE_LEN];
	t_image			****mobs_txtrs;
	t_image			****mobs_txtrs_mini;
	t_image			***pickup_txtr;
	t_image			***wpn_txtr;
	t_image			***pickup_txtr_mini;
	t_image			***wpn_txtr_2d;
	t_image			**wall_img;
	t_image			**wall_img2d;
	t_image			*door_txtr;
	t_image			*door_txtr_mini;
	const char		*ents_tp_map[ENT_TYPE_LEN];
	const char		*ents_types_names[ENT_TYPE_LEN];
	const char		*weapons_names[WEAPON_TYPE_LEN];
	const char		*mob_names[MOB_TYPE_LEN];
	const char		*pickup_names[PCKP_TYPE_LEN];
	const char		*ents_act_names[ENT_ACTION_LEN];
	const char		*plr_act_names[PLR_ACTION_LEN];
	int				size_2d;
	float			texture_accumulator;
}	t_texture_data;

typedef struct s_md
{
	void			*mlx;
	void			*win;
	t_ent			plr;
	t_ent			**mapped_ents;
	t_dblst			*entities;
	t_hud			hud;
	t_menu			menu;
	t_map			map;
	t_portal_gun	portal_gun;
	t_mmap			mmap;
	t_timer			timer;
	t_post_fx_data	fx;
	t_mouse			mouse;
	t_texture_data	txd;
	t_parameters	prm;
	t_image			*screen;
	t_image			*center;
	t_ray			rays[MAX_RAYS];
	t_vec3f			cam_ofst;
	t_vec3f			wrd_mv_offst;
	t_vec3f			plr_wrd_mv;
	t_vec2			win_size;
	t_vec3f			input_mov;
	t_vec2			input_offst;
	t_vec3f			plr_rot;
	t_vec3f			cam_pos;
	float			bob_time;
	int				key_prs[512];
	int				key_clicked;
	void			*var_;
	const char		*dir_labels[4];
	char			base_map_path[50];
	unsigned int	random_seed;
	int				rgb[19];
	int				init_steps;
	int				update_frames;
	int				t_len;
	int				var;
	int				score;
	int				is_linux;
	int				(*mlx_put)(void *mlx, void *win, void *img, int x, int y);
	void			*(*mlx_make)(void *mlx, char *name, int *with, int *height);
}	t_md;

t_vec2			get_v2_grid_pos(t_md *md, t_vec2 pos);
t_vec3			get_grid_pos(t_md *md, t_vec3 pos);

//		TOOLS.c
int				r_range(int min, int max);
int				r_range_seed(unsigned int *g_seed, int min, int max);

void			*flip_image_x(t_md *md, void *img, t_vec2 size);

//		INPUT_MOUSE.c
int				mouse_event_handler(int button, int x, int y, void *param);
int				mouse_release_handler(int button, int x, int y, void *param);
int				mouse_motion_handler(int x, int y, void *param);
int				update_mouse(t_md *md);

//		TIME2.c
void			stop_timer(t_timer *timer);
void			resume_timer(t_timer *timer);
double			get_total_time(t_timer *timer);
void			handle_timers(t_md *md, t_timer *tm);

//		INPUT.c
int				handle_key_press(int keycode, t_md *md);
int				handle_key_release(int keycode, t_md *md);
void			reset_mlx_values(t_md *md);
int				close_window(t_md *md);

//		RENDER.c
int				draw_pixel(t_image *dst, t_vec2 pos, int color, float opacity);
int				draw_pixels(t_image *dst, t_vec2 start, t_vec2 size, int clr);
int				draw_img(t_image *from, t_image *onto, t_vec2 pos, int drawovr);
void			flush_img(t_image *src, int clr, float opacity, int ign_alpha);
void			draw_transp_img(t_image *src, t_image *dst, t_vec2 pos, float trnsp);

//		TIME.c
double			get_time_in_seconds(void);
double			check_timer(double timer);
void			start_timer(double *timer);
void			stop_timer(t_timer *timer);

//		FREE_b.c
int				free_void(void *elem);
int				free_void_array(void **elements);
int				free_md2(t_md *md, int free_count, int quit);

//		IMAGES_a.c
t_vec3f			set_new_size(t_image *q, t_vec2 *old_size, t_vec2 *new_size);
void			*scale_img_keep_ratio(t_md *md, void *img, t_vec2 *old_size, \
	t_vec2 new_size);
void			*resize_img(t_md *md, void *img, t_vec2 *old_size, t_vec2 new_size);
void			set_img_data_color(t_image *img_data, t_vec2 size, int col, float str);

//		IMAGES_b.c
void			*get_image_copy(t_md *md, void *src, t_vec2 src_size);
void			*add_img(char *relative_path, int *width, int *height, t_md *md);
void			render_cursor(t_md *md, t_image *screen, int has_hov);

//	data.x = ratio | data.y = add IMG_PATH
void			*ld_txtr(t_md *md, t_vec2 final_size, char *path, t_vec2 data);
void			**ld_txtrs(t_md *md, t_vec2 final_size, char *dirpath, \
	int maintain_ratios);
t_image			*init_img(t_md *md, t_vec2 size, char *path, int color);
t_image			**init_images(t_md *md, t_vec2 size, char *path);
t_image			*scale_imgd(t_md *md, t_image *imgd, t_vec2 new_size, int keep_ratio);

//		INIT_WRAPPER.c
int				init_md(t_md *md);
void			init_wrapper(t_md *md, t_vec2 win_size, char *win_name, int resolution);

//		FREE_a.c
int				free_images(t_md *md, void ***images, char *label);
int				free_image_data(t_md *md, t_image *img_data);
int				free_images_data(t_md *md, t_image **images, char *label);
int				free_md(t_md *md, int quit);
int				safe_free(void *item);
int				free_and_quit(t_md *d, const char *msg, const char *attribute);

//		AUDIO.c
pid_t			play_random_sound(t_md *md, const char *path, int len);
pid_t			play_sound(t_md *md, const char *filename);
pid_t			play_index(t_md *md, const char *filepath, int index);
void			stop_sound(pid_t pid);
int				is_audio_playing(t_md *md, pid_t pid);

//		COLORS.c
int				get_trgb(unsigned char t, unsigned char r, \
	unsigned char g, unsigned char b);
t_vec3			get_grid_posf(t_md *md, t_vec3f pos);
int				str_to_color(const char *line);
void			update_ent_frame(t_ent *e);

//		STRING.c
char			*md_strjoin(t_md *d, char const *s1, char const *s2);
char			*md_strdup(t_md *d, const char	*s1);
void			*md_malloc(t_md *d, ssize_t size);
void			*md_realloc(t_md *d, void *ptr, size_t new_size);
char			*md_strndup(t_md *d, const char	*s1, ssize_t n);

//		strcmp_tools
int				same_str(const char *a, const char *b);
int				char_in_str(char c, const char *txt);
int				chr_amnt(const char *str, char c);
int				get_arr_len(void **arr);
char			*truncate_at_end(const char *str, char cut_letter);
void			setstr(char **str, char *new);

//		string_tools.c
int				only_contains(char *str, char *to_contain);
int				get_char_index(const char *str, char to_check);
char			*ft_megajoin(const char *a, const char *b, \
	const char *c, const char *d);

int				update_map_index(t_md *md, t_ent *e);
void			draw_sprite(t_md *md, t_ray *ray, t_hit_data hit_data);
int				is_in_screen(t_md *md, t_vec3 pos, t_vec2 size);

int				blend_color(int color_a, int color_b, float factor);

//		TIMER
void			init_timer(t_md *md, t_timer *timer);
double			update_time(t_md *md, t_timer *timer);
void			update_input(t_md *md);

//		LOAD_IMG
char			**get_frames(t_md *md, char *dir, int *amount, int max);
t_image			*copy_image(t_md *md, t_image *src_img, t_vec2 new_size, int color);
void			draw_sphere(t_image *txtr, t_vec2 pos, t_vec2 draw_size, t_vec3 data);
void			fill_transparency(t_image *src, int color);
void			draw_trimm_img(t_image *src, t_image *dst, \
	t_vec2 pos, t_vec3 draw_end);

//		VECTORS_SHOW
void			show_vec2(t_md *md, char *label, t_vec2 vec, t_vec2 pos);
void			show_vec3(t_md *md, char *label, t_vec3 vec, t_vec2 pos);
void			show_vec3f(t_md *md, char *label, t_vec3f vec, t_vec2 pos);
void			show_int(t_md *md, char *label, int value, t_vec2 pos);
void			show_float(t_md *md, char *label, float value, t_vec2 pos);
float			f_range(float min, float max);

unsigned int	get_random_seed(void);
char			*ft_strndup(const char	*s1, ssize_t n);
void			draw_colored_img(t_image *src, t_image *dst, t_vec2 pos, t_vec3 color_data);
float			get_alpha(int color);
int				set_alpha(int color, float new_alpha);

void			set_mouse_lock(t_md *md, int lock);
t_image			*init_abs_img(t_md *md, t_vec2 img_size, char *path);
void			wrap_mouse(t_md *md, int delta_x, int delta_y);
int				put_pixel_if_visible(t_draw_data *dd, int overlap, int blend, float trsp);
void			draw_line(t_image *onto, t_vec2 start, t_vec2 end, t_vec2 color_d);
void			draw_contour(t_image *img, int color, int thickness, float opacity);
t_vec4			color_to_v4(int color);

void			replace_image(t_md *md, t_image **dst, t_image *src);
void			flipy_image_data(t_image *img);

#endif