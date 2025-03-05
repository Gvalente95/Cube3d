/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:53:43 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/05 14:29:10 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_H
# define CUBE_H

# include "mlx_wrapper/mlx_utils.h"
# include "lists/lists.h"
# include "gnl/get_next_line.h"
# include <math.h>

# define CURSOR_SPR_PATH "mlx_png/cursor/default.png"
# define CURSOR_DETECT_PATH "mlx_png/cursor/hand_open.png"
# define CURSOR_GRAB_PATH "mlx_png/cursor/hand_closed.png"
# define FONT_SPRITE_PATH "mlx_png/font/"

# define PLRSPD 3.5f
# define ACCSPD 0.2f
# define DEBUG_SCALE 10

//		update
int		update_and_render(t_md *md);

//		render
void	render(t_md *md);

//		free
int		safe_free(void *item);
int		free_and_quit(t_md *d, const char *msg, \
const char *attr);

//		init
int		init_cube(t_md *md, char *file_arg, int start_debug);
int		init_map(t_md *md, char *file_name);
void	init_map_data(t_md *md);

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

int		convert_line_to_color(const char *line);
char	*extract_line(const char *str);

//		debug.c
void	show_init_information(t_md *md);
void	show_update_information(t_md *md);

//		text.c
void	rnd_abs_txt(t_md *md, t_vec4 data, const char *format, ...);
void	render_text(t_md *md, t_vec4 data, const char *format, ...);

//		math_tools.c
float	minmaxf(float min, float max, float value);
float	minf(float a, float b);
float	maxf(float a, float b);
t_vec3f	normalize_vec3f(t_vec3f vec, float min, float max);
void	color_img(void *frame, t_vec2 size, int col, t_vec4 d);

#endif