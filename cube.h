/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:53:43 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/15 02:13:28 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_H
# define CUBE_H

# include "mlx_wrapper/mlx_utils.h"
# include "lists/lists.h"
# include "gnl/get_next_line.h"
# include <math.h>
//# include <X11/X.h>

# define RESOLUTION		4
//		SCREEN
# define SCRN_W			800
# define SCRN_H			600
//		RAYS
# define RAY_DEPTH		3000
# define FOV			60
//		PLR
# define PLRSPD 12.0f
# define ACCSPD 0.6f
# define PLR_HEIGHT 20
# define PLR_JUMPPOW 1
# define GRAVITY .1
# define ARROW_ROTATION_SPD 15
# define SCROLL_SPD .2
//		CURSOR

# define STARS_AMOUNT	200
# define CROSS_SCALE	10
# define ENNEMY_SPEED	.1
# define MAX_PARTICLES	50
# define PARTICLE_SPEED 50

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

//		render
void	render(t_md *md);
void	color_img(void *frame, t_vec2 size, int col, t_vec4 d);
int		compute_perspective_change(t_md *md, float *height, float ray_dst);
void	render_minimap(t_md *md, t_mmap *mp);

//		update
int		update_and_render(t_md *md);
int		update_player(t_md *md, t_ent *plr);
int		move_ent(t_md *md, t_ent *e);
int		move_player(t_md *md, t_ent *e);
int		update_ents(t_md *md);

//		collision
int		set_collisions(t_md *md, t_ent *e);
int		is_collision(t_ent *a, t_ent *b);

//		free
int		safe_free(void *item);
int		free_and_quit(t_md *d, const char *msg, \
const char *attr);

//		debug.c
void	show_init_information(t_md *md);
void	show_update_information(t_md *md);
void	show_fps(t_md *md, t_vec2 pos);
void	show_vec2(t_md *md, char *label, t_vec2 vec, t_vec2 pos);
void	show_vec3(t_md *md, char *label, t_vec3 vec, t_vec2 pos);
void	show_vec3f(t_md *md, char *label, t_vec3f vec, t_vec2 pos);

//		text.c
void	rnd_abs_txt(t_md *md, t_vec4 data, const char *format, ...);
void	render_text(t_md *md, t_vec4 data, const char *format, ...);

//		math_tools.c
int		minmax(int min, int max, int curr);
float	minmaxf(float min, float max, float value);
float	minf(float a, float b);
float	maxf(float a, float b);
t_vec3f	normalize_vec3f(t_vec3f vec, float min, float max);
int		ft_sign(float a);

//		ftoa.c
char	*ftoa(float num, int precision);

//		rays.c
int		render_ray(t_md *md, t_ray *ray, t_ent *ray_hit);
void	cast_rays(t_md *md, t_vec3f start_pos);
void	draw_wall_line(t_md *md, float dist, t_ent *col, t_ray *ray);
t_ent	*check_in_map(t_md *md, t_ray *ray, float distance);

int		vec4_to_color(int r, int g, int b, int a);

void	reset_mapped_end(t_md *md, t_ent *e);
int		ent_in_bounds(t_ent *ent, t_ent *bounds);
void	render_background(t_md *md);

#endif