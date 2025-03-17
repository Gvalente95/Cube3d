/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:30:15 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/16 09:00:01 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTORS_H
# define VECTORS_H

# include "../mlx_utils.h"

# define EPSILON 0.00001f

typedef struct s_Vector2
{
	int	x;
	int	y;
}	t_vec2;

typedef struct s_Vector3
{
	int	x;
	int	y;
	int	z;
}	t_vec3;

typedef struct s_Vector3f
{
	float	x;
	float	y;
	float	z;
}	t_vec3f;

typedef struct s_Vector4
{
	int	r;
	int	g;
	int	b;
	int	a;
}	t_vec4;


typedef struct s_Vector4f
{
	float	r;
	float	g;
	float	b;
	float	a;
}	t_vec4f;

//		VECTOR_2.c
t_vec2	get_v2(int x, int y);
t_vec2	v3_to_v2(t_vec3 vec);
int		cmp_vec2(t_vec2 a, t_vec2 b);
int		print_vec2(t_vec2 a, const char *label);
t_vec2	center_pos(t_vec2 a, t_vec2 a_size, t_vec2 b, t_vec2 b_size);

//		VECTOR_3.c
t_vec3	get_v3(int x, int y, int z);
t_vec3	v2_to_v3(t_vec2 vec);
int		cmp_vec3(t_vec3 a, t_vec3 b);
int		print_vec3(t_vec3 a, const char *label);

//		VECTOR_3f.c
t_vec3f	get_v3f(float x, float y, float z);
t_vec3f	v3_to_v3f(t_vec3 vec);
int		cmp_vec3f(t_vec3f a, t_vec3f b, float margin);
int		print_vec3f(t_vec3f a, const char *label);

//		VECTOR_4.c
t_vec4	get_v4(int r, int g, int b, int a);
t_vec3	v4_to_v3(t_vec4 a);
int		cmp_vec4(t_vec4 a, t_vec4 b);
int		print_vec4(t_vec4 a, const char *label);

//		VECTOR_4f.c
t_vec4f	get_v4f(float r, float g, float b, float a);
int		print_vec4f(t_vec4f a, const char *label);
t_vec3f	v4f_to_v3f(t_vec4f a);

//		VECTOR_b
t_vec2	v2_center(t_vec2 a, t_vec2 b);
t_vec3f	v3f_center(t_vec3f a, t_vec3f b);
t_vec3	v3_center(t_vec3 a, t_vec3 b);
int		v2_bounds(t_vec3f a, t_vec3f bnd_pos, t_vec3f bnd_size);
int		v3f_bounds(t_vec3f a, t_vec3f pos, t_vec3f size);
int		v3_bounds(t_vec3 a, t_vec3 pos, t_vec3 size);

#endif