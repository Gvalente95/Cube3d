/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalente <gvalente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:53:43 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/13 05:41:14 by gvalente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_H
# define CUBE_H

# include "mlx_wrapper/mlx_utils.h"
# include "lists/lists.h"
# include "gnl/get_next_line.h"
# include <math.h>
# include <X11/X.h>

# define RESOLUTION		10
//		SCREEN
# define SCRN_W			1200
# define SCRN_H			800
//		RAYS
# define RAY_DEPTH		5000
# define FOV			60
//		PLR
# define PLRSPD 12.0f
# define ACCSPD 0.6f
# define PLR_HEIGHT 20
# define PLR_JUMPPOW 1
# define GRAVITY .1
# define SCROLL_SPD .3
//		CURSOR

# define STARS_AMOUNT	200
# define CROSS_SCALE	10


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
void	*ld_txtr(t_md *md, t_vec2 final_size, char *path);
void	**ld_txtrs(t_md *md, t_vec2 final_size, char *dirpath);

//		render
void	render(t_md *md);
void	render_init_img(t_md *md, void *img, t_vec2 size, char *txt);
int		is_in_screen(t_md *md, t_vec3 pos, t_vec2 size);
void	color_img(void *frame, t_vec2 size, int col, t_vec4 d);
int		compute_perspective_change(t_md *md, float *height, float ray_dst);

//		update
int		update_and_render(t_md *md);
int		update_player(t_md *md);
int		move_ent(t_ent *e);
int		update_ents(t_md *md);

//		collision
void	set_collisions(t_md *md, t_ent *e);
int		is_collision(t_ent *a, t_ent *b);

//		free
int		safe_free(void *item);
int		free_and_quit(t_md *d, const char *msg, \
const char *attr);

//		strcmp_tools
int		same_str(const char *a, const char *b);
int		char_in_str(char c, const char *txt);
int		chr_amnt(const char *str, char c);
int		get_arr_len(void **arr);
char	*truncate_at_end(const char *str, char cut_letter);
void	setstr(char **str, char *new);

//		string_tools.c
char	*md_strjoin(t_md *d, char const *s1, char const *s2);
char	*md_strdup(t_md *d, const char	*s1);
void	*md_malloc(t_md *d, ssize_t size);
void	*md_realloc(t_md *d, void *ptr, size_t new_size);
char	*md_strndup(t_md *d, const char	*s1, ssize_t n);
int		only_contains(char *str, char *to_contain);
int		get_char_index(const char *str, char to_check);
char	*ft_megajoin(char *a, char *b, char *c, char *d);

//		debug.c
void	show_init_information(t_md *md);
void	show_update_information(t_md *md);
void	show_fps(t_md *md, t_vec2 pos);
void	show_vec3(t_md *md, char *label, t_vec3 vec, t_vec2 pos);
void	show_vec3f(t_md *md, char *label, t_vec3f vec, t_vec2 pos);

//		text.c
void	rnd_abs_txt(t_md *md, t_vec4 data, const char *format, ...);
void	render_text(t_md *md, t_vec4 data, const char *format, ...);

//		math_tools.c
float	minmaxf(float min, float max, float value);
float	minf(float a, float b);
float	maxf(float a, float b);
t_vec3f	normalize_vec3f(t_vec3f vec, float min, float max);

//		ftoa.c
char	*ftoa(float num, int precision);

//		rays.c
void	render_ray(t_md *md, t_ray *ray);
void	render_rays(t_md *md, t_vec3f start_pos);
void	draw_txt_line(t_md *md, float dist, t_ent *col, t_ray *ray);
void	dda_ray(t_md *md, t_ray *ray, int ray_color);
t_ent	*check_in_map(t_md *md, t_ray *ray);
void	draw_pxl(t_md *md, t_ray *ray, float height, t_ent *col, t_vec2 txt_p);

int		vec4_to_color(int r, int g, int b, int a);
int		ft_sign(float a);
int		my_put_pixel(void *frame, int x, int y, int col);

#endif