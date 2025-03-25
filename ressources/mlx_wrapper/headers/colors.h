/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 05:11:14 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/24 20:55:46 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

//COLORS
unsigned char	get_t(int trgb);
unsigned char	get_r(int trgb);
unsigned char	get_g(int trgb);
unsigned char	get_b(int trgb);

# define RGB_RED	0
# define RGB_GREEN	1
# define RGB_BLUE	2
# define RGB_CYAN	3
# define RGB_MAGENT	4
# define RGB_GRAY	5
# define RGB_BROWN	6
# define RGB_TEAL	7
# define RGB_LIME	8
# define RGB_GOLD	9
# define RGB_SILVER	10
# define RGB_TURQ	11
# define RGB_INDIGO	12
# define RGB_VIOLET	13
# define RGB_CORAL	14
# define RGB_WHITE	15
# define RGB_BLACK	16
# define RGB_YELLOW	17
# define RGB_ORANGE	18

# define COLOR_RED        		0xFFFF0000  // Fully opaque red
# define COLOR_GREEN      		0xFF00FF00  // Fully opaque green
# define COLOR_BLUE       		0xFF0000FF  // Fully opaque blue
# define COLOR_YELLOW     		0xFFFFFF00  // Fully opaque yellow
# define COLOR_CYAN      		0xFF00FFFF  // Fully opaque cyan
# define COLOR_MAGENTA    		0xFFFF00FF  // Fully opaque magenta
# define COLOR_WHITE      		0xFFFFFFFF  // Fully opaque white
# define COLOR_BLACK			0xFF000000  // Fully opaque black

# define PORTAL_START			0xFF111111  // Fully opaque black
# define PORTAL_ENT				0xFF222222  // Fully opaque black

# define COLOR_TRANSP_RED      0x80FF0000  // 50% transparent red
# define COLOR_TRANSP_GREEN    0x8000FF00  // 50% transparent green
# define COLOR_TRANSP_BLUE     0x800000FF  // 50% transparent blue
# define COLOR_TRANSP_YELLOW   0x20FFFFFF  // 50% transparent yellow
# define COLOR_TRANSP_CYAN     0x8000FFFF  // 50% transparent cyan
# define COLOR_TRANSP_MAGENTA  0x80FF00FF  // 50% transparent magenta
# define COLOR_TRANSP_WHITE    0x80FFFFFF  // 50% transparent white
# define COLOR_TRANSP_BLACK    0x80000000  // 50% transparent black

# define COLOR_TXT_RED				"\033[31m"
# define COLOR_TXT_RESET			"\033[0m"
# define COLOR_TXT_GREEN			"\033[32m"
#endif
