/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:11:25 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/07/10 20:58:03 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	init_pipes(t_abst *d)
{
	size_t	i;
	
	i = 0;
	d->pipefd = malloc(sizeof(int[2]) * (d->cmds.count - 1));
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
	if (d->is_here_doc)
	{
		unlink(".here_doc");
		d->iofd[0] = open(".here_doc", O_RDWR | O_CREAT | O_TRUNC);
		d->iofd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND);		
	}
	else
	{
		d->iofd[0] = open(argv[1], O_RDONLY);
		d->iofd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC);
	}
	if (d->iofd[0] == -1)
		return (0);
	if (d->iofd[1] == -1)
		return (0);
	return (1);
}

int	init_abst(int argc, char **argv, t_abst *d)
{
	d->is_here_doc = 0;
	d->counter = 0;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		(d->is_here_doc = 1, d->limiter = argv[2]);
	if (!create_cmds(argc, argv, &d->cmds, d->is_here_doc))
		return (0);
	if (!init_pipes(d))
		return (free_all(d), 0);
	if (!open_io_files(argc, argv, d))
		return (free_all(d), 0);
	return (1);
}
