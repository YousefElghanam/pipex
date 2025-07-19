/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josefelghnam <josefelghnam@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:11:25 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/07/19 18:21:37 by josefelghna      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	init_pipes(t_abst *d)
{
	size_t	i;

	i = 0;
	d->pipefd = malloc(sizeof(int [2]) * (d->cmds.count - 1));
	if (!d->pipefd)
		return (0);
	while (i < d->cmds.count - 1)
	{
		d->pipefd[i][0] = -1;
		d->pipefd[i][1] = -1;
		i++;
	}
	return (1);
}

static int	open_io_files(int argc, char **argv, t_abst *d)
{
	d->iofd[0] = open(argv[1], O_RDONLY);
	if (d->iofd[0] == -1)
		return (0);
	d->iofd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (d->iofd[1] == -1)
		return (0);
	return (1);
}

int	init_data(int argc, char **argv, t_abst *d)
{
	d->counter = 0;
	if (!create_cmds(argc, argv, &d->cmds))
		return (0);
	if (!init_pipes(d))
		return (perror("pipex"), free_all(d), 0);
	if (!open_io_files(argc, argv, d))
		return (perror("pipex"), free_all(d), 0);
	return (1);
}
