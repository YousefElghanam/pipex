/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:52:04 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/07/21 21:34:24 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	exec_first_cmd(t_abst *d)
{
	if (d->is_here_doc)
	{
		d->iofd[0] = open(".tmp", O_RDONLY);
		if (d->iofd[0] == -1)
			(free_all(d), perror("here_doc"), exit(1));
	}
	dup2(d->pipefd[0][1], STDOUT_FILENO);
	dup2(d->iofd[0], STDIN_FILENO);
	close_pipes(d);
	if (execve(d->cmds.arr[0][0], d->cmds.arr[0], __environ)
		== -1)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(ft_strrchr(d->cmds.arr[0][0], '/') + 1, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (close(0), close(1), 1);
	}
	return (0);
}

static int	exec_cmd(t_abst *d, size_t counter)
{
	if (counter == 0)
		return (exec_first_cmd(d));
	if (d->counter + 1 == d->cmds.count)
		dup2(d->iofd[1], STDOUT_FILENO);
	else
		dup2(d->pipefd[d->counter][1], STDOUT_FILENO);
	dup2(d->pipefd[d->counter - 1][0], STDIN_FILENO);
	close_pipes(d);
	if (execve(d->cmds.arr[d->counter][0], d->cmds.arr[d->counter], __environ)
		== -1)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(ft_strrchr(d->cmds.arr[d->counter][0], '/') + 1, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (close(0), close(1), 1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_abst	d;

	if (!init_data(argc, argv, &d))
		return (unlink(".tmp"), 1);
	while (d.counter < d.cmds.count)
	{
		if (d.counter + 1 < d.cmds.count)
			if (pipe(d.pipefd[d.counter]) == -1)
				return (unlink(".tmp"), free_all(&d), perror("pipex"), 1);
		d.pid = fork();
		if (d.pid == -1)
			return (unlink(".tmp"), free_all(&d), perror("pipex"), 1);
		if (d.pid == 0)
			exec_cmd(&d, d.counter);
		d.counter++;
	}
	close_pipes(&d);
	while (wait(NULL) > 0)
		;
	return (unlink(".tmp"), free_all(&d), 0);
}
