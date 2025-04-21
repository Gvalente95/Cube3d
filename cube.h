/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:53:43 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/21 16:01:06 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_H
# define CUBE_H

# include "ressources/mlx_wrapper/mlx_utils.h"
# include "ressources/lists/lists.h"
# include "ressources/gnl/get_next_line.h"
# include <math.h>
//# include <X11/X.h>

# define IMG_PATH			"ressources/xpm/"
# define RESOLUTION		10
//		SCREEN
# define SCRN_W			1300
# define SCRN_H			800
//		RAYS
# define RAY_DEPTH		50
//		PLR
# define PLRSPD			800
# define ACCSPD			0.3
# define HEIGHT			0
# define BOB_AMOUNT		.2
# define BOB_SPD		7
# define GRAVITY		.04
# define ARROW_ROT_SPEED 2
# define MOUSESPD		.5
# define START_HP		10
# define SHOOT_REFRESH	.1
# define WALK_REFRESH	.5
# define ANIM_REFRESH	.05

# define MAX_AMMO		50
# define MAX_KEY		3
//		HUD
# define STARS_AMOUNT	1000
# define CROSS_SCALE	10
//		ENNEMY
# define ENNEMY_SPEED	.1
# define RAY_ZOOM		50

# define THREADS_BATCH	16
# define FLOOR_WORKERS	4

# define FE_PER_TILE	100
# define REVEAL_DISTANCE 5
# define TARGET_FPS		100

//		init/init_menu.c
void	set_menu_pos(t_md *md, t_menu *menu, \
	t_vec3 sldr_offst, t_vec3 but_ofst);
void	init_menu_overlay(t_md *md, t_menu *menu);
void	init_menu(t_md *md, t_menu *menu);
int		replace_window(t_md *md, int new_w, int new_h);

//	init/init_frames2.c
void	handle_mobs_frames(t_md *md, t_image ****frames, \
	t_image ****mini, t_mob_types type);
void	init_mobs_frames(t_md *md);
t_image	**init_mini(t_md *md, t_image ***mini, char *path);
t_image	**init_weapon(t_md *md, t_image ***mini, char *path);

//	init/init_hud.c
void	init_fx(t_md *md, t_fx_data *fx);
void	init_hud(t_md *md, t_hud *hud);
void	init_fes(t_md *md, t_env_manager *env, int tlen);

//	init/init_map.c
char	*get_resized_line(char *buffer, int width, char replace_end_with);
char	*redimension_map(char *map_buffer, t_vec2 size);
int		init_map(t_md *md, char *file_name);

//	init/init_minimap.c
void	init_minimap(t_md *md, t_mmap *mmap);

//	init/init_labels.c
void	init_action_labels(t_texture_data *td);
void	init_weapon_labels(t_texture_data *td);
void	init_ents_labels(t_texture_data *td);
void	init_labels(t_texture_data *txd);

//	init/init_frames.c
void	init_weapon_frames(t_md *md, t_texture_data *td);
void	init_pickup_frames(t_md *md, t_texture_data *td);
void	store_entities_sizes(t_texture_data *td, int base);
void	init_ents_data(t_md *md, t_texture_data *txd);

//	init/init_ent_frames.c
int		init_ent_frames(t_md *md, t_texture_data *txd, t_ent *e);

//	init/init_menu_elements.c
void	init_menu_elements(t_md *md, t_menu *menu);

//	init/init_map_data.c
void	init_map_data(t_md *md);

//	init/init_background.c
void	init_background(t_md *md, t_hud *hud, t_vec2 win_sz);

//	init/init_map_validator.c
int		flood_fill(t_map *map, char *str, int index, int depth);
void	print_unvalid_flood(char *flooded_map);
int		find_breach(t_md *md, t_map *map, int plr_index);
int		validate_map(t_md *md, char *map);

//	init/init_entities.c
t_ent	*init_ent(t_md *md, char c, t_vec2 pos, int map_index);
void	init_entities(t_md *md, t_vec2 pos);

//	init/init_cube.c
int		init_cube(t_md *md, char *file_arg, int start_debug);

//	raycasting/ray_tools_2.c
void	set_ray_color(t_md *md, t_ray *ray);
int		update_ray_grid_pos(t_md *md, t_ray *ray);
int		correct_fisheye(t_md *md, t_ray *ray, t_ent *e, float dist);
int		get_wall_orientation(t_ray *ray);
int		compute_row_start(t_md *md, t_ent *e, float ray_dst);

//	raycasting/ray_cast.c
int		cast_check_ray(t_md *md, t_ray *ray, t_vec3f start_pos, t_ent *check);
int		cast_ray(t_md *md, t_ray *ray, t_vec2 visu_offset);
void	compute_ray_directions(t_md *md, t_vec3f *dir_vals, int rays_amount);
void	cast_rays(t_md *md);

//	raycasting/draw_sprite_column.c
void	paint_ent(t_md *md, t_ent *e, t_vec2 txtr_coord);
int		get_prspctive_offset(t_md *md, float ray_dst, t_ent *e);
void	draw_sprite_slice(t_md *md, t_ent *ent, t_vec2 winp, t_vec3f crd);
void	draw_sprite_pxl(t_md *md, t_ray *ray, \
	t_ent *sprite, float sprt_scrn_width);
void	draw_sprite(t_md *md, t_ray *ray, t_hit_data hit_data);

//	raycasting/portal_raycast.c
int		dir_to_rotation(t_wrd_dir a, t_wrd_dir b);
void	rotate_90_deg(t_vec3f *vec, int times);
int		get_portal_index(t_md *md, t_ray *ray, t_ent *portal);
int		translate_ray(t_md *md, t_ray *ray, t_ent *portal, int view_index);

//	raycasting/portal_draw.c
void	draw_portal(t_md *md, t_ent *e, t_vec2 pos);

//	raycasting/ray_move.c
t_ent	*search_in_grid(t_md *md, t_ray *ray, float distance);
int		ray_move(t_md *md, t_ray *ray, t_vec2 visu_offset);

//	raycasting/draw_wall_column.c
int		draw_wall_line(t_md *md, float dist, t_ent *wall, t_ray *ray);

//	raycasting/ray_tools.c
void	draw_blood(t_md *md, t_image *img, t_vec2 pos, int color);
t_vec2	get_2d_ray_pos(t_md *md);
int		render_ray(t_md *md, t_ray *ray, t_vec2 visu_offset);
void	init_base_ray(t_ray *ray, int index, t_vec3f start_pos, float distance);
int		validate_check_hit(t_md *md, t_ray *ray, t_ent *ent, t_ent_type type);

//	raycasting/ray_cast_threads.c
void	update_ray_data(t_md *md, t_ray *ray, t_vec3f dir_val);
void	*cast_thread_batch(void *content);

//	tools/map_gen.c
char	*get_cmps_map(t_vec2 size, int rects_amount, int i);
char	*get_new_map(int difficulty, t_vec2 *size, char *data_info);

//	tools/parse_tools.c
void	trim_excess_spaces(char **line);
int		trim_excess_newlines(char **map, int len);
void	remove_chars(t_md *md, char **txt, const char *to_remove);
int		get_to_find_index(char *str, char *to_find);

//	tools/map_gen_2.c
void	set_doors(char *map, int doors_amount);
void	set_characters(char *map, int difficulty);
char	*set_map_with_base(char *map);

//	tools/debug.c
void	show_init_information(t_md *md);
void	show_fps(t_md *md, t_vec2 pos);
void	show_update_information(t_md *md);
void	print_color(int color, const char *label);

//	tools/map_gen_tools.c
char	*get_rectangle(t_vec2 size);
int		get_char_amount(char *buffer, char c);
void	close_map(char *map, t_vec2 size, int len);

//	tools/text.c
char	*get_img_path(char c);
int		rnd_fast_txt(t_md *md, t_txtd data, const char *format, ...);

//	tools/image_tools.c
void	color_img(void *frame, t_vec2 size, int col, t_vec4 d);
int		str_to_color(const char *line);

//	tools/ftoa.c
char	*ftoa(float num, int precision);

//	update/update_plr.c
void	set_weapon_index(t_md *md);
int		update_player(t_md *md, t_ent *plr);

//	update/update_input.c
void	update_input(t_md *md);

//	update/update.c
double	update_time(t_md *md, t_timer *timer);
int		set_menu_mode(t_md *md, t_menu *menu, int mode);
int		update_and_render(t_md *md);

//	update/movement.c
int		update_map_index(t_md *md, t_ent *e);
void	move_ent_to_target(t_md *md, t_ent *e, t_vec3f target_p);
void	update_mob_actions(t_md *md, t_ent *e);

//	update/update_menu.c
int		update_menu(t_md *md, t_menu *menu);

//	update/update_ents.c
void	reset_mapped_end(t_md *md, t_ent *e);
void	update_ent_frame(t_ent *e);
int		update_ents(t_md *md);

//	update/collisions.c
int		is_collision(t_ent *a, t_ent *b, t_vec2 a_size);
int		set_collisions(t_md *md, t_ent *e, t_vec2 e_size);

//	update/update_projectile.c
void	plr_shoot(t_md *md);

//	update/update_plr_movement.c
int		move_player(t_md *md, t_ent *e);
int		update_player_mov(t_md *md, t_ent *plr);

//	update/collisions_portal.c
int		dir_to_angle(t_wrd_dir dir);
int		get_portal_angle_offset(t_wrd_dir start_dir, t_wrd_dir end_dir);
int		validate_portal_collision(t_md *md, t_ent *b);

//	render/render_minimap.c
void	render_mmap_ray(t_md *md, int ray_index, int color);
void	render_minimap_ray(t_md *md);
void	show_minimap_entity(t_md *md, t_ent *e, t_image *screen, int no_redraw);
void	render_minimap_entities(t_md *md, t_mmap *mp, t_vec2 center);
void	render_minimap(t_md *md, t_mmap *mp);

//	render/filters.c
void	apply_antialiasing(t_image *img);
void	apply_scanlines(t_image *img, float factor);
void	apply_noise(t_md *md, t_image *img, float factor, float colors_amount);

//	render/render_hud.c
void	draw_locks_icons(t_md *md, t_hud *hud, t_vec2 winsz);
void	draw_info(t_md *md, t_vec2 *screen_pos, t_image *icon, int value);
void	draw_game_info(t_md *md, t_hud *hud, t_vec2 winsz);
void	draw_hud_weapon(t_md *md, t_hud *hud, t_vec2 winsz);
void	render_hud_elements(t_md *md, t_hud *hud);

//	render/render.c
void	render_cursor(t_md *md, t_image *screen, int has_hov);
void	render_2d_ent(t_md *md, t_ent *e, t_vec2 centr);
void	apply_fx(t_md *md, t_image *screen, t_fx_data *fx);
void	render(t_md *md);

//	render/filters_2.c
void	apply_barrel_fx(t_image *img, t_vec2 sz, t_vec2 c, float intensity);
void	apply_color_banding(t_image *img, float intensity);
void	apply_bloom(t_image *img, float intensity);
void	apply_vignette(t_image *img, float intensity, int color);
void	set_hue(t_image *img, t_vec4f rgb_factors);

//	render/render_background.c
void	render_2d_floor(t_md *md);
void	render_background(t_md *md);

//	render/dithering.c
void	apply_error(t_image *img, t_vec2 pos, t_vec4 err_rgb, t_vec3f data);
void	dither(t_image *img, t_vec3 pos, unsigned int *palette, float str);
void	apply_dithering(t_image *img, float dither_strength, \
	unsigned int *palette, int palette_size);

//	render/render_minimap_cmp.c
void	show_cmps_mmap(t_md *md, t_vec2 center, int view_dist);
void	draw_sprite_thread(t_md *md, t_ent *e, t_vec2 win_sz, float fogalpha);
int		is_in_list(t_dblst *lst, t_ent *e);
void	draw_found_ents(t_md *md, t_thrd_manager *mon);
void	render_menu(t_md *md, t_menu *menu);
void	render_slider(t_md *md, t_slider *sldr, t_image *screen, float alpha);
void	reset_grass(t_md *md, t_fe *fe);

void	draw_stored_fe(t_md *md);
int		render_fe(t_md *md, t_fe *fe, int width);

int		update_key_input(t_md *md, t_menu *menu, unsigned int c);
void	init_fonts(t_md *md);

int		draw_wall_line_dda(t_md *md, float dist, t_ent *hit, t_ray *ray);
int		ray_move_dda(t_md *md, t_ray *ray);
int		update_and_render_fe(t_md *md, t_floor_draw_d d, t_fe **prv_fe);
void	draw_raycast_background(t_md *md, t_ray *ray);
void	draw_ceiling(t_md *md, t_floor_draw_d d);
int		draw_stored_sprite_hits(t_md *md, t_ray *ray);
int		display_quick_letter(t_md *md, char c, t_txtd data);
int		update_autocam(t_md *md, t_autocam *autocam);
void	update_audio(t_md *md, t_au_manager *au);
void	init_fade_intro(t_md *md, double duration);
void	render_autocam_text(t_md *md, float t);
int		add_ent_at_cord(t_md *md, t_ent *e, t_vec2 new_cord);

//		map_ents
void	init_mapped_ent(t_md *md);
int		remove_ent_at_cord(t_md *md, t_vec2 cord);
t_ent	*get_mapped_at_cord(t_md *md, t_vec2 cord);
t_ent	*get_mapped_at_pos(t_md *md, t_vec2f pos);

void	render_logo_cube(t_md *md, t_menu *menu);
void	update_logo_cube(t_md *md, t_mouse mouse, t_menu *menu);
void	update_menu_element_end(t_menu *menu, int *hov, int cur_hov);
void	center_menu_txt(t_md *md, t_vec2 pos_ofst, int scale, char *name);
int		render_sky(t_md *md, t_image *bufr);
void	cap_fps(t_timer *tm, double frame_duration);
void	init_pokemon_frames(t_md *md, t_texture_data *txd);
void	show_pointed_data(t_md *md, t_vec2 p, t_ent *e);

#endif