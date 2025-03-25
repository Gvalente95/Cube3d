/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 04:54:44 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/23 16:41:50 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx_utils.h"

pid_t	play_random_sound(t_md *md, const char *path, int len)
{
	pid_t	sound;
	char	*with_index;
	char	*fullpath;
	char	*index_str;
	int		index;

	if (!md->prm.audio_volume)
		return (0);
	index = r_range(0, len - 1);
	index_str = ft_itoa(index);
	with_index = ft_strjoin(path, index_str);
	free(index_str);
	fullpath = ft_strjoin(with_index, ".mp3");
	free(with_index);
	sound = play_sound(md, fullpath);
	free(fullpath);
	return (sound);
}

pid_t	play_sound(t_md *md, const char *filename)
{
	pid_t	pid;

	if (!md->prm.audio_volume)
		return (0);
	if (access(filename, F_OK) == -1)
		return (printf("%s audio file not found\n", filename), 0);
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

pid_t	play_index(t_md *md, const char *filepath, int index)
{
	pid_t	pid;
	char	*index_txt;
	char	*path_with_index;
	char	*full_path;

	if (!md->prm.audio_volume)
		return (0);
	index_txt = ft_itoa(index);
	path_with_index = ft_strjoin(index_txt, ".mp3");
	full_path = ft_strjoin(filepath, path_with_index);
	pid = play_sound(md, full_path);
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

int	is_audio_playing(t_md *md, pid_t pid)
{
	int	status;

	if (!md->prm.audio_volume)
		return (1);
	if (waitpid(pid, &status, WNOHANG) == 0)
		return (1);
	return (0);
}
