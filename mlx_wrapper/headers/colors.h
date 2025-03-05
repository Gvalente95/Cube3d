/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 05:11:14 by gvalente          #+#    #+#             */
/*   Updated: 2025/01/16 03:52:02 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

//COLORS
unsigned char	get_t(int trgb);
unsigned char	get_r(int trgb);
unsigned char	get_g(int trgb);
unsigned char	get_b(int trgb);

# define COLOR_RED        0xFFFF0000  // Fully opaque red
# define COLOR_GREEN      0xFF00FF00  // Fully opaque green
# define COLOR_BLUE       0xFF0000FF  // Fully opaque blue
# define COLOR_YELLOW     0xFFFFFF00  // Fully opaque yellow
# define COLOR_CYAN       0xFF00FFFF  // Fully opaque cyan
# define COLOR_MAGENTA    0xFFFF00FF  // Fully opaque magenta
# define COLOR_WHITE      0xFFFFFFFF  // Fully opaque white
# define BLACK      	0xFF000000  // Fully opaque black

# define COLOR_TRANSPARENT_RED      0x80FF0000  // 50% transparent red
# define COLOR_TRANSPARENT_GREEN    0x8000FF00  // 50% transparent green
# define COLOR_TRANSPARENT_BLUE     0x800000FF  // 50% transparent blue
# define COLOR_TRANSPARENT_YELLOW   0x20FFFFFF  // 50% transparent yellow
# define COLOR_TRANSPARENT_CYAN     0x8000FFFF  // 50% transparent cyan
# define COLOR_TRANSPARENT_MAGENTA  0x80FF00FF  // 50% transparent magenta
# define COLOR_TRANSPARENT_WHITE    0x80FFFFFF  // 50% transparent white
# define COLOR_TRANSPARENT_BLACK    0x80000000  // 50% transparent black

# define COLOR_TXT_RED				"\033[31m"
# define COLOR_RESET				"\033[0m"
# define COLOR_TXT_GREEN			"\033[32m"
#endif
