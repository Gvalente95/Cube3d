/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:38:37 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/04/01 11:47:36 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUDIO_H
#define AUDIO_H

# include "../mlx_utils.h"

# define AU_WALK_METAL		"ressources/audio/footsteps/metal/"
# define AU_WALK_GRASS		"ressources/audio/footsteps/grass/"
# define AU_RELOAD			"ressources/audio/reload/0.mp3"
# define AU_SLICE			"ressources/audio/shoot/0.mp3"
# define AU_SHOOT			"ressources/audio/shoot/1.mp3"
# define AU_CLICK			"ressources/audio/UI/click.mp3"
# define AU_HOV				"ressources/audio/UI/hov.mp3"
# define AU_GRAB			"ressources/audio/UI/hov.mp3"
# define AU_OPEN			"ressources/audio/open.mp3"
# define AU_WALK_STEP		"ressources/audio/footsteps/steps/"
# define AU_PORTAL_SHOOT	"ressources/audio/portal_shoot.mp3"
# define AU_WIND			"ressources/audio/wind.mp3"
# define AU_MUS				"ressources/audio/music.mp3"

typedef struct s_au_manager
{
	pid_t	wind_pid;
	pid_t	amb_pid;
	pid_t	mus_pid;
}	t_au_manager;

//		var/audio.c
pid_t			play_random_sound(t_md *md, const char *path, int len);
pid_t			play_sound(t_md *md, const char *filename);
pid_t			play_index(t_md *md, const char *filepath, int index);
void			stop_sound(pid_t pid);
int				is_audio_playing(t_md *md, pid_t pid);
int				play_loop(t_md *md, pid_t *pid, char *filename, int depend);

#endif