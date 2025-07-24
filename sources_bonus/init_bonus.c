/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:11:25 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/07/24 16:28:04 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

static void	fill_here_doc(t_abst *d)
{
	char	*line;

	while (1)
	{
		ft_printf ("> ");
		line = get_next_line(0);
		if (!line)
		{
			ft_printf("\nwarning: here-document at line 42 delimited by ");
			ft_printf("end-of-file (wanted `%s')\n", d->limiter);
			break ;
		}
		if (is_limiter(line, d->limiter))
		{
			free(line);
			break ;
		}
		if (write(d->iofd[0], line, ft_strlen(line)) == -1)
		{
			(close(d->iofd[0]), free_all(d));
			(free(line), perror("pipex: here_doc"), exit(1));
		}
		free(line);
	}
}

static int	here_doc(int argc, char **argv, t_abst *d)
{
	if (access(".tmp", F_OK) == 0)
		unlink(".tmp");
	d->iofd[0] = open(".tmp", O_RDWR | O_CREAT, 0666);
	if (d->iofd[0] == -1)
	{
		perror("pipex: .tmp: ");
		return (0);
	}
	fill_here_doc(d);
	d->iofd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (d->iofd[1] == -1)
	{
		write(2, "pipex: ", 7);
		write(2, argv[argc - 1], ft_strlen(argv[argc - 1]));
		write(2, ": ", 2);
		perror("");
		return (0);
	}
	close(d->iofd[0]);
	return (1);
}

static int	open_io_files(int argc, char **argv, t_abst *d)
{
	if (!d->is_here_doc)
	{
		d->iofd[0] = open(argv[1], O_RDONLY);
		if (d->iofd[0] == -1)
		{
			write(2, "pipex: ", 7);
			write(2, argv[1], ft_strlen(argv[1]));
			write(2, ": ", 2);
			perror("");
			return (0);
		}
		d->iofd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (d->iofd[1] == -1)
		{
			write(2, "pipex: ", 7);
			write(2, argv[argc - 1], ft_strlen(argv[argc - 1]));
			write(2, ": ", 2);
			perror("");
			return (0);
		}
	}
	else if (!here_doc(argc, argv, d))
		return (0);
	return (1);
}

int	init_data(int argc, char **argv, t_abst *d)
{
	d->is_here_doc = 0;
	d->counter = 0;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		d->is_here_doc = 1;
		d->limiter = argv[2];
	}
	if (argc < 5 + d->is_here_doc)
		return (write(2, "pipex: not enough arguments\n", 28), 0);
	if (!open_io_files(argc, argv, d))
		return (0);
	if (!create_cmds(argc, argv, &d->cmds, d->is_here_doc))
	{
		(close(d->iofd[0]), close(d->iofd[1]));
		return (0);
	}
	if (!init_pipes(d))
	{
		(close(d->iofd[0]), close(d->iofd[1]));
		return (perror("pipex"), free_all(d), 0);
	}
	return (1);
}
