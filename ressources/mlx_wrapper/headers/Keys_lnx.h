/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Keys_lnx.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:49:05 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/29 12:42:20 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYS_LNX_H
# define KEYS_LNX_H

# include "../mlx_utils.h"

# define NUM_1_KEY        '1'  // 49
# define NUM_2_KEY        '2'  // 50
# define NUM_3_KEY        '3'  // 51
# define NUM_4_KEY        '4'  // 52
# define NUM_5_KEY        '5'  // 53
# define NUM_6_KEY        '6'  // 54
# define NUM_7_KEY        '7'  // 55
# define NUM_8_KEY        '8'  // 56
# define NUM_9_KEY        '9'  // 57
# define NUM_0_KEY        '0'  // 48

# define NUM_A_KEY            'a'  // 97
# define NUM_B_KEY            'b'  // 98
# define NUM_C_KEY            'c'  // 99
# define NUM_D_KEY            'd'  // 100
# define NUM_E_KEY            'e'  // 101
# define NUM_F_KEY            'f'  // 102
# define NUM_G_KEY            'g'  // 103
# define NUM_H_KEY            'h'  // 104
# define NUM_I_KEY            'i'  // 105
# define NUM_J_KEY            'j'  // 106
# define NUM_K_KEY            'k'  // 107
# define NUM_L_KEY            'l'  // 108
# define NUM_M_KEY            'm'  // 109
# define NUM_N_KEY            'n'  // 110
# define NUM_O_KEY            'o'  // 111
# define NUM_P_KEY            'p'  // 112
# define NUM_Q_KEY            'q'  // 113
# define NUM_R_KEY            'r'  // 114
# define NUM_S_KEY            's'  // 115
# define NUM_T_KEY            't'  // 116
# define NUM_U_KEY            'u'  // 117
# define NUM_V_KEY            'v'  // 118
# define NUM_W_KEY            'w'  // 119
# define NUM_X_KEY            'x'  // 120
# define NUM_Y_KEY            'y'  // 121
# define NUM_Z_KEY            'z'  // 122

# define NUM_QSTION_KEY	20
# define NUM_EXCL_KEY	20
# define NUM_DOT_KEY	47
# define NUM_TIRET_KEY	44
# define NUM_DEL_KEY	51
# define NUM_ENTER_KEY	36
# define NUM_CTRL_KEY	256
# define NUM_SHIFT_KEY	257
# define NUM_LFTCMD_KEY	259
# define NUM_RGTCMD_KEY	260
# define NUM_LFTOPT_KEY	261
# define NUM_RGTOPT_KEY	262

# define ESC_KEY         65307   // ASCII de ESC

# define LEFT_KEY       65361
# define RIGHT_KEY      65363
# define DOWN_KEY       65364
# define UP_KEY         65362

# define SPACE_KEY       ' '   // ASCII de l'espace
# define ENTER_KEY       13   // ASCII de Return (ou '\n' = 10 selon config)
# define BACKSPACE_KEY   8    // ASCII de Backspace (ou 127 selon config)
# define TAB_KEY         9    // ASCII de Tabulation
# define SHIFT_KEY      65505
# define CTRL_KEY       256
# define ALT_KEY        261

# define MOUSE_NOPRESS    0
# define MOUSE_PRESS      1
# define MOUSE_DPRESS     2
# define MOUSE_SCRL_UP    4
# define MOUSE_SCRL_D     5
# define MOUSE_SCRL_L     7
# define MOUSE_SCRL_R     6
# define MOUSE_RELEASE    4

# define KEY_PRESS        KeyPress
# define KEY_RELEASE      KeyRelease

#endif