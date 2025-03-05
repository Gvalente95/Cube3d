/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AUDIO.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 04:54:44 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/01/15 22:09:07 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int	g_au_volume = 1;

pid_t	play_random_sound(const char *path, int len, const char *format)
{
	pid_t	sound;
	char	*with_index;
	char	*fullpath;
	char	*index_str;
	int		index;

	if (!g_au_volume)
		return (0);
	index = r_range(0, len - 1);
	index_str = ft_itoa(index);
	with_index = ft_strjoin(path, index_str);
	free(index_str);
	fullpath = ft_strjoin(with_index, format);
	free(with_index);
	sound = play_sound(fullpath, 0);
	free(fullpath);
	return (sound);
}

int	mute_unmute_audio(int mute)
{
	g_au_volume = mute;
	return (mute);
}

pid_t	play_sound(const char *filename, int loop)
{
	pid_t	pid;

	(void)loop;
	if (!g_au_volume)
		return (0);
	pid = fork();
	if (pid == 0)
	{
		if (execlp("afplay", "afplay", filename, (char *) NULL) == -1)
		{
			perror("execlp failed");
			exit(EXIT_FAILURE);
		}
	}
	return (pid);
}

pid_t	play_index(const char *filepath, int index)
{
	pid_t	pid;
	char	*index_txt;
	char	*path_with_index;
	char	*full_path;

	index_txt = ft_itoa(index);
	path_with_index = ft_strjoin(index_txt, ".mp3");
	full_path = ft_strjoin(filepath, path_with_index);
	pid = play_sound(full_path, 0);
	free(full_path);
	free(path_with_index);
	free(index_txt);
	return (pid);
}

void	stop_sound(pid_t pid)
{
	if (pid > 0)
		kill(pid, SIGTERM);
}

int	is_audio_playing(pid_t pid)
{
	int	status;

	if (!g_au_volume)
		return (1);
	if (waitpid(pid, &status, WNOHANG) == 0)
		return (1);
	return (0);
}
