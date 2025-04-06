/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_camera.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:49:48 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/05 18:15:02 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

void	update_cam_bob(t_md *md, t_ent *plr)
{
	float	bob_speed;
	float	bob_amount;
	float	vrt_offset;
	int		is_running;

	is_running = md->key_prs[SHIFT_KEY];
	if (md->plr.grounded && !cmp_vec3f \
		(get_v3f(plr->mov.x, plr->mov.y, 0), v3f(0), 0.01f))
		md->cam.bob_time += md->timer.delta_time;
	bob_speed = BOB_SPD * (1 + is_running);
	bob_amount = md->prm.bob_amount;
	if (md->key_prs[SHIFT_KEY])
		bob_amount *= 2;
	vrt_offset = sinf(md->cam.bob_time * bob_speed) * bob_amount;
	md->cam.pos.z += vrt_offset;
}

void	update_plr_offsets(t_md *md, t_cam *cam)
{
	const t_vec2	win_sz = md->win_sz;
	const t_vec2	plr_size = md->plr.size;
	const t_vec3f	plr_pos = md->plr.pos;
	t_vec3f			dspl;

	dspl.x = plr_pos.x - plr_size.x / 2 - win_sz.x / 2 + md->t_len / 2;
	dspl.y = plr_pos.y - plr_size.y / 2 - win_sz.y / 2 + md->t_len / 2;
	dspl.z = plr_pos.z;
	cam->ofst = dspl;
	cam->wrd_mv_offst.x += cam->plr_wrd_mv.x * 20;
	cam->wrd_mv_offst.y += cam->plr_wrd_mv.y * 20;
	cam->input_offst.x += (int)(cam->plr_wrd_mv.x);
	cam->input_offst.y -= (int)(cam->plr_wrd_mv.y);
}

void	update_cam(t_md *md, t_cam *cam)
{
	t_vec3f	pos;
	t_ent	*plr;

	plr = &md->plr;
	pos = plr->pos;
	cam->pos.x = pos.x + plr->size.x / 2 - (plr->dir.x * md->prm.zoom);
	cam->pos.y = pos.y + plr->size.y / 2 - (plr->dir.y * md->prm.zoom);
	cam->pos.z = pos.z - md->prm.height;
	md->plr.coord.x = pos.x / md->t_len;
	md->plr.coord.y = pos.y / md->t_len;
	md->plr.coord.z = pos.z / md->t_len;
	update_plr_offsets(md, cam);
	if (md->prm.bob_amount > 0)
		update_cam_bob(md, plr);
}

t_vec3f	update_fly_cam(t_md *md, t_cam *cam, float spd)
{
	t_vec3f			mov;
	const t_vec3f	cam_forward = normalize_vec3f(md->plr.dir);
	const t_vec3f	cam_right = get_v3f(-cam_forward.y, cam_forward.x, 0);
	const t_vec3f	cam_up = get_v3f(0, 0, -1);

	mov = v3f(0);
	if (md->key_prs[NUM_W_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_forward, spd));
	if (md->key_prs[NUM_S_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_forward, -spd));
	if (md->key_prs[NUM_D_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_right, spd));
	if (md->key_prs[NUM_A_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_right, -spd));
	if (md->key_prs[NUM_E_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_up, -spd));
	if (md->key_prs[NUM_Q_KEY])
		mov = add_vec3f(mov, scale_vec3f(cam_up, spd));
	if (md->key_prs[NUM_R_KEY])
		mov.z -= spd * .05;
	cam->input_mov.x = md->key_prs[NUM_A_KEY] - md->key_prs[NUM_D_KEY];
	cam->input_mov.y = md->key_prs[NUM_W_KEY] - md->key_prs[NUM_S_KEY];
	cam->input_mov.z = md->key_prs[NUM_E_KEY] - md->key_prs[NUM_Q_KEY];
	return (mov);
}
