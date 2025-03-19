/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:53:43 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/19 05:20:45 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_H
# define CUBE_H

# include "ressources/mlx_wrapper/mlx_utils.h"
# include "ressources/lists/lists.h"
# include "ressources/gnl/get_next_line.h"
# include <math.h>
//# include <X11/X.h>

# define RESOLUTION		5
//		SCREEN
# define SCRN_W			1000
# define SCRN_H			600
//		RAYS
# define RAY_DEPTH		50
//		PLR
# define PLRSPD 75
# define ACCSPD 0.5
# define PLR_HEIGHT 20
# define PLR_JUMPPOW .2
# define GRAVITY .015
# define ARROW_ROT_SPEED 1
# define MOUSE_ROT_SPD	.1
# define START_HP		10
//		CURSOR

# define SHOOT_REFRESH	.1

# define STARS_AMOUNT	2000
# define CROSS_SCALE	10
# define ENNEMY_SPEED	.1
# define MAX_PARTICLES	50
# define PARTICLE_SPEED 4000
# define IMG_PATH		"ressources/xpm/"

typedef enum polar_dir
{
	NORTH,
	SOUTH,
	EAST,
	WEST
}	t_wrd_dir;

//		init
int		init_cube(t_md *md, char *file_arg, int start_debug);
void	init_ents_data(t_md *md);
int		init_map(t_md *md, char *file_name);
void	init_map_data(t_md *md);
void	init_action_labels(t_md *md);
void	init_ents_labels(t_md *md);
void	init_entities(t_md *md, t_vec2 pos);
void	init_labels(t_md *md);
t_ent	*init_ent(t_md *md, char c, t_vec2 pos, int map_index);
void	init_menu(t_md *md, t_menu *menu);
void	init_hud(t_md *md);
void	init_menu_elements(t_md *md, t_menu *menu);
void	handle_mobs_frames(t_md *md, t_image ****frames, \
	t_image ****mini, t_mob_types type);
void	init_mobs_frames(t_md *md);
t_image	**init_mini(t_md *md, t_image ***mini, char *path, t_ent_type type);
t_image	**init_weapon(t_md *md, t_image ***mini, char *path);
void	init_ent_frames(t_md *md, t_ent *e, char c);

//		render
void	render(t_md *md);
void	color_img(void *frame, t_vec2 size, int col, t_vec4 d);
int		compute_perspective_change(t_md *md, float *height, float ray_dst);
void	render_minimap(t_md *md, t_mmap *mp);
void	render_background(t_md *md);

//		update
int		update_and_render(t_md *md);
int		update_player(t_md *md, t_ent *plr);
int		move_ent(t_md *md, t_ent *e);
int		move_player(t_md *md, t_ent *e);
int		update_ents(t_md *md);
int		update_menu(t_md *md, t_menu *menu);
int		update_menu_input(t_md *md, t_menu *menu);
void	update_mob_actions(t_md *md, t_ent *e);

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

//		text.c
void	rnd_abs_txt(t_md *md, t_vec4 data, const char *format, ...);
void	render_text(t_md *md, t_vec4 data, const char *format, ...);

//		math_tools.c
int		minmax(int min, int max, int curr);
float	minmaxf(float min, float max, float value);
float	minf(float a, float b);
float	maxf(float a, float b);
int		ft_sign(float a);

//		ftoa.c
char	*ftoa(float num, int precision);

//		rays.c
t_vec2	get_centered_ray_position(t_md *md);
int		render_ray(t_md *md, t_ray *ray, t_ent *ray_hit, t_vec2 visu_center);
void	cast_rays(t_md *md, t_vec3f start_pos);
void	draw_wall_line(t_md *md, float dist, t_ent *col, t_ray *ray);
void	draw_blood(t_md *md, t_image *img, t_vec2	pos, int color);

int		vec4_to_color(int r, int g, int b, int a);

void	reset_mapped_end(t_md *md, t_ent *e);
int		ent_in_bounds(t_ent *ent, t_ent *bounds);

//		filters
void	apply_antialiasing(t_image *img);
void	set_hue(t_image *img, t_vec4f rgb_factors);
void	apply_scanlines(t_image *img, float darken_factor);
void	apply_rgb_glitch(t_image *img, int intensity);
void	apply_dithering(t_image *img, float dither_strength);

void	plr_shoot(t_md *md);
void	store_entities_sizes(t_md *md, t_vec2 base);

//		MAP_GEN
char	*get_new_map(int difficulty, t_vec2 *size, char *data_info);
int		validate_map(t_md *md, char *map, int len);

void	generate_maze(char *map, t_vec2 size);
void	set_characters(char *map, int difficulty);
int		get_char_amount(char *buffer, char c);

#endif