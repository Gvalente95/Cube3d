/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KEYCODE.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:14:31 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/01/15 22:09:40 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

char	map_keycode_to_char4(int keycode)
{
	if (keycode == NUM_V_KEY)
		return ('v');
	if (keycode == NUM_W_KEY)
		return ('w');
	if (keycode == NUM_X_KEY)
		return ('x');
	if (keycode == NUM_Y_KEY)
		return ('y');
	if (keycode == NUM_Z_KEY)
		return ('z');
	if (keycode == NUM_QSTION_KEY)
		return ('?');
	if (keycode == NUM_EXCL_KEY)
		return ('!');
	if (keycode == NUM_DOT_KEY)
		return ('.');
	if (keycode == NUM_TIRET_KEY)
		return ('-');
	if (keycode == NUM_ENTER_KEY)
		return ('\n');
	return (printf("\"%d\" Keycode not found\n", keycode), '\0');
}

char	map_keycode_to_char3(int keycode)
{
	if (keycode == NUM_K_KEY)
		return ('k');
	if (keycode == NUM_L_KEY)
		return ('l');
	if (keycode == NUM_M_KEY)
		return ('m');
	if (keycode == NUM_N_KEY)
		return ('n');
	if (keycode == NUM_O_KEY)
		return ('o');
	if (keycode == NUM_P_KEY)
		return ('p');
	if (keycode == NUM_Q_KEY)
		return ('q');
	if (keycode == NUM_R_KEY)
		return ('r');
	if (keycode == NUM_S_KEY)
		return ('s');
	if (keycode == NUM_T_KEY)
		return ('t');
	if (keycode == NUM_U_KEY)
		return ('u');
	return (map_keycode_to_char4(keycode));
}

char	map_keycode_to_char2(int keycode)
{
	if (keycode == NUM_A_KEY)
		return ('a');
	if (keycode == NUM_B_KEY)
		return ('b');
	if (keycode == NUM_C_KEY)
		return ('c');
	if (keycode == NUM_D_KEY)
		return ('d');
	if (keycode == NUM_E_KEY)
		return ('e');
	if (keycode == NUM_F_KEY)
		return ('f');
	if (keycode == NUM_G_KEY)
		return ('g');
	if (keycode == NUM_H_KEY)
		return ('h');
	if (keycode == NUM_I_KEY)
		return ('i');
	if (keycode == NUM_J_KEY)
		return ('j');
	return (map_keycode_to_char3(keycode));
}

char	map_keycode_to_char(int keycode)
{
	if (keycode == NUM_0_KEY)
		return ('0');
	if (keycode == NUM_1_KEY)
		return ('1');
	if (keycode == NUM_2_KEY)
		return ('2');
	if (keycode == NUM_3_KEY)
		return ('3');
	if (keycode == NUM_4_KEY)
		return ('4');
	if (keycode == NUM_5_KEY)
		return ('5');
	if (keycode == NUM_6_KEY)
		return ('6');
	if (keycode == NUM_7_KEY)
		return ('7');
	if (keycode == NUM_8_KEY)
		return ('8');
	if (keycode == NUM_9_KEY)
		return ('9');
	return (map_keycode_to_char2(keycode));
}
