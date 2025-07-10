/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:11:31 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/07/10 21:29:09 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	close_pipes(t_abst *d)
{
	size_t	i;

	i = 0;
	if (d->iofd[0] > -1 && !d->is_here_doc)
		close(d->iofd[0]);
	if (d->iofd[1] > -1)
		close(d->iofd[1]);
	while (i < d->cmds.count - 1)
	{
		if (d->pipefd[i][0] > -1)
			close(d->pipefd[i][0]);
		if (d->pipefd[i][1] > -1)
			close(d->pipefd[i][1]);
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

void	fill_here_doc(t_abst *d)
{
	char	*line;

	line = get_next_line(0);
	while (line && !is_limiter(line, d->limiter))
	{
		if (write(d->iofd[0], line, ft_strlen(line)) == -1)
			(free(line), perror("pipex"), exit(1));
		free(line);
		line = get_next_line(0);
	}
	perror("pipex");
	// 	if (read(d->iofd[0], line, 1) > 0)
	// 		(free(line), perror("pipex"), exit(1));
	free(line);
}

void	handle_here_doc(t_abst *d)
{
	if (!d->is_here_doc)
		return ;
	if (d->counter == 0)
		fill_here_doc(d);
	else
		(d->iofd[0] = -1, d->is_here_doc = 0);
}

static void	exec_first_cmd(t_abst *d)
{
	if (d->is_here_doc)
	{
		close(d->iofd[0]);
		d->iofd[0] = open(".here_doc", O_RDONLY);
		if (!d->iofd[0])
			(free_all(d), perror("pipex"), exit(1));
	}
	dup2(d->pipefd[0][1], STDOUT_FILENO);
	dup2(d->iofd[0], STDIN_FILENO);
	close_pipes(d), close(d->iofd[0]);
	if (execve(d->cmds.arr[0][0], d->cmds.arr[0], __environ)
		== -1)
		(free_all(d), perror("pipex"), exit(1));
}

static void	exec_cmd(t_abst *d, size_t counter)
{
	if (counter == 0)
		exec_first_cmd(d);
	if (d->counter + 1 == d->cmds.count)
		dup2(d->iofd[1], STDOUT_FILENO);
	else
		dup2(d->pipefd[d->counter][1], STDOUT_FILENO);
	// ft_printf("fd: %d\n", d->pipefd[d->counter - 1][0]);
	dup2(d->pipefd[d->counter - 1][0], STDIN_FILENO);
	close_pipes(d);
	if (execve(d->cmds.arr[d->counter][0], d->cmds.arr[d->counter], __environ)
		== -1)
		(free_all(d), perror("pipex"), exit(1));
}

void	free_all(t_abst *d)
{
	free(d->pipefd);
	free_cmds(&d->cmds);
	// unlink(".here_doc");
}

int	main(int argc, char **argv)
{
	t_abst	d;

	if (argc < 5)
		return (ft_printf("pipex: not enough arguments\n"), 1);
	if (!init_abst(argc, argv, &d))
		return (perror("pipex"), 1);	
	while (d.counter < d.cmds.count)
	{
		if (d.counter + 1 < d.cmds.count)
			if (pipe(d.pipefd[d.counter]) == -1)
				return (free_all(&d), perror("pipex"), 1);
		handle_here_doc(&d);
		d.pid = fork();
		if (d.pid == -1)
			return (free_all(&d), perror("pipex"), 1);
		if (d.pid == 0)
			exec_cmd(&d, d.counter);
		d.counter++;
	}
	close_pipes(&d), close(d.iofd[0]);
	while (wait(&d.status) > 0)
		;
	return (free_all(&d), 0);
}
