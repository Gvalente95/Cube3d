/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:53:43 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/25 19:30:49 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_H
# define CUBE_H

# include "ressources/mlx_wrapper/mlx_utils.h"
# include "ressources/lists/lists.h"
# include "ressources/gnl/get_next_line.h"
# include <math.h>
//# include <X11/X.h>

# define AU_WALK			"ressources/audio/footsteps/0.mp3"
# define AU_RUN				"ressources/audio/footsteps/1.mp3"
# define AU_RELOAD			"ressources/audio/reload/0.mp3"
# define AU_SLICE			"ressources/audio/shoot/0.mp3"
# define AU_SHOOT			"ressources/audio/shoot/1.mp3"
# define AU_CLICK			"ressources/audio/UI/click.mp3"
# define AU_HOV				"ressources/audio/UI/hov.mp3"
# define AU_GRAB			"ressources/audio/UI/hov.mp3"
# define AU_OPEN			"ressources/audio/open.mp3"
# define AU_WALK_STEP		"ressources/audio/footsteps/steps/"
# define AU_PORTAL_SHOOT	"ressources/audio/portal_shoot.mp3"

# define IMG_PATH			"ressources/xpm/"

# define RESOLUTION		25
//		SCREEN
# define SCRN_W			1750
# define SCRN_H			1200
//		RAYS
# define RAY_DEPTH		50
//		PLR
# define PLRSPD			600
# define ACCSPD			0.5
# define HEIGHT			0
# define BOB_AMOUNT		.6
# define BOB_SPD		7
# define PLR_JUMPPOW	.2
# define GRAVITY		.015
# define ARROW_ROT_SPEED 2
# define MOUSESPD		.05
# define START_HP		10
# define SHOOT_REFRESH	.1
# define MAX_AMMO		50
# define MAX_KEY		3
//		HUD
# define STARS_AMOUNT	1000
# define CROSS_SCALE	10
//		ENNEMY
# define ENNEMY_SPEED	.1
# define RAY_ZOOM		25

//		init
int		init_cube(t_md *md, char *file_arg, int start_debug);
void	init_ents_data(t_md *md, t_texture_data *td);
int		init_map(t_md *md, char *file_name);
void	init_map_data(t_md *md);
void	init_action_labels(t_texture_data *td);
void	init_ents_labels(t_texture_data *td);
void	init_entities(t_md *md, t_vec2 pos);
void	init_labels(t_md *md, t_texture_data *txd);
t_ent	*init_ent(t_md *md, char c, t_vec2 pos, int map_index);
void	init_menu(t_md *md, t_menu *menu);
void	init_hud(t_md *md, t_hud *hud);
void	init_menu_elements(t_md *md, t_menu *menu);
void	handle_mobs_frames(t_md *md, t_image ****frames, \
	t_image ****mini, t_mob_types type);
void	init_mobs_frames(t_md *md);
t_image	**init_mini(t_md *md, t_image ***mini, char *path);
t_image	**init_weapon(t_md *md, t_image ***mini, char *path);
void	init_ent_frames(t_md *md, t_texture_data *txd, t_ent *e);

//		render
void	render(t_md *md);
void	color_img(void *frame, t_vec2 size, int col, t_vec4 d);
int		compute_perspective_change(t_md *md, float ray_dst);
void	render_minimap(t_md *md, t_mmap *mp);
void	render_background(t_md *md);

//		update
int		update_and_render(t_md *md);
int		update_player(t_md *md, t_ent *plr);
int		move_player(t_md *md, t_ent *e);
int		update_ents(t_md *md);
int		update_menu(t_md *md, t_menu *menu);
int		update_menu_input(t_md *md, t_menu *menu);
void	update_mob_actions(t_md *md, t_ent *e);
int		update_player_mov(t_md *md);
void	set_weapon_index(t_md *md);
int		set_menu_mode(t_md *md, t_menu *menu, int mode);

//		collision
int		set_collisions(t_md *md, t_ent *e, t_vec2 e_size);
int		is_collision(t_ent *a, t_ent *b, t_vec2 a_size);

//		free
int		safe_free(void *item);
int		free_and_quit(t_md *d, const char *msg, \
const char *attr);

//		debug.c
void	show_init_information(t_md *md);
void	show_update_information(t_md *md);
void	show_fps(t_md *md, t_vec2 pos);

//	DATA = (x pos, y pos, text color, text scale) return: text width
int		rnd_abs_txt(t_md *md, t_vec4 data, const char *format, ...);
int		render_text(t_md *md, t_vec4 data, const char *format, ...);

//		math_tools.c
int		minmax(int min, int max, int curr);
float	minmaxf(float min, float max, float value);
float	minf(float a, float b);
float	maxf(float a, float b);
int		ft_sign(float a);

//		ftoa.c
char	*ftoa(float num, int precision);

//		rays.c
t_vec2	get_2d_ray_pos(t_md *md);
int		render_ray(t_md *md, t_ray *ray, t_ent *ray_hit, t_vec2 visu_center);
void	cast_rays(t_md *md, t_vec3f start_pos);
int		draw_wall_line(t_md *md, float dist, t_ent *wall, t_ray *ray);
void	draw_blood(t_md *md, t_image *img, t_vec2	pos, int color);
void	init_base_ray(t_ray *ray, int index, t_vec3f start_pos, float distance);

void	reset_mapped_end(t_md *md, t_ent *e);
int		ent_in_bounds(t_ent *ent, t_ent *bounds);

//		filters
void	apply_antialiasing(t_image *img);
void	set_hue(t_image *img, t_vec4f rgb_factors);
void	apply_scanlines(t_image *img, float darken_factor);
void	apply_dithering(t_image *img, float dither_strength, \
	unsigned int *palette, int palette_size);

void	plr_shoot(t_md *md);
void	store_entities_sizes(t_texture_data *td, t_vec2 base);

//		MAP_GEN
char	*get_new_map(int difficulty, t_vec2 *size, char *data_info);
int		validate_map(t_md *md, char *map);
void	set_characters(char *map, int difficulty);
int		get_char_amount(char *buffer, char c);

void	apply_barrel_distortion(t_image *img, float strength);
void	apply_color_banding(t_image *img, float levels);
void	apply_glitch(t_image *img, float intensity);
void	apply_bloom(t_image *img, float intensity);

int		cast_ray(t_md *md, t_ray *ray, t_vec2 visu_offset);
int		cast_check_ray(t_md *md, t_ray *ray, t_vec3f start_pos, t_ent *check);
void	render_hud_elements(t_md *md, t_hud *hud);
void	draw_rotated_img(t_md *md, t_image *from, t_image *onto, t_vec3f pos);

void	trim_excess_spaces(char **line);
int		trim_excess_newlines(char **map, int len);
void	remove_chars(t_md *md, char **txt, const char *to_remove);
void	print_color(int color, const char *label);
int		set_alpha(int color, float new_alpha);
t_ent	*search_in_grid(t_md *md, t_ray *ray, float distance);

char	*get_rectangle(t_vec2 size);
int		get_char_amount(char *buffer, char c);
int		get_to_find_index(char *map, char *to_find);
int		find_breach(char *buffer, int width, int len, int plr_index);
void	apply_vignette(t_image *img, float intensity, int color);
void	flipx_image_data(t_image *img);
char	*set_map_with_base(char *map);
void	close_map(char *map, t_vec2 size, int len);
void	update_cam(t_md *md, t_ent *plr);
void	apply_noise(t_md *md, t_image *img, float factor, float desaturate);


void	rotate_90_deg(t_vec3f *vec, int times);
int		dir_to_rotation(t_wrd_dir dir_a, t_wrd_dir dir_b);
void	draw_portal(t_md *md, t_ent *e, t_vec2 pos);
int		translate_ray(t_md *md, t_ray *ray, t_ent *portal, float distance);
int		calculate_verticality(t_md *md, t_ray *ray);
int		validate_check_hit(t_md *md, t_ray *ray, t_ent *ent, t_ent_type type);
void	init_background(t_md *md, t_hud *hud, t_vec2 win_sz);
int		check_portal_validity(t_md *md, t_ray *ray, t_ent *portal);
int		correct_fisheye(t_md *md, t_ray *ray, t_ent *e, float dist);

#endif