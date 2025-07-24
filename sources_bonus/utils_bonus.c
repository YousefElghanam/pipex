/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:51:03 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/07/11 22:56:10 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	safe_close(int fd)
{
	if (fd > -1)
		close(fd);
}

void	free_all(t_abst *d)
{
	free(d->pipefd);
	free_cmds(&d->cmds);
}

void	free_split_arr(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	close_pipes(t_abst *d)
{
	size_t	i;

	i = 0;
	safe_close(d->iofd[0]);
	safe_close(d->iofd[1]);
	while (i < d->cmds.count - 1)
	{
		safe_close(d->pipefd[i][0]);
		safe_close(d->pipefd[i][1]);
		i++;
	}
}

int	is_limiter(char *line, char *limiter)
{
	size_t	limiter_len;

	limiter_len = ft_strlen(limiter);
	if (ft_strncmp(line, limiter, limiter_len) == 0
		&& line[limiter_len] == '\n')
		return (1);
	return (0);
}
