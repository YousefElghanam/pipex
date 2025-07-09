/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:11:31 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/07/09 21:47:17 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	close_pipes(t_abst *d)
{
	size_t	i;

	i = 0;
	close(d->iofd[0]);
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

static void	exec_first_cmd(t_abst *d)
{
	char	*line;

	line = "x";
	// if (!d->is_here_doc)
	dup2(d->iofd[0], STDIN_FILENO);
	// else
	// {
	// 	ft_printf("OMG");
	while (ft_strncmp(line, d->limiter, ft_strlen(d->limiter)) && line)
		line = get_next_line(STDIN_FILENO);
	// }
	dup2(d->pipefd[0][1], STDOUT_FILENO);
	close_pipes(d);
	if (execve(d->cmds.arr[0][0], d->cmds.arr[0], __environ)
		== -1)
		(free_all(d), exit(1));
}

static void	exec_cmd(t_abst *d, size_t counter)
{
	// ft_printf("cmd (%s) == arg (%s)\n", d->cmds.arr[d->counter][0], d->cmds.arr[d->counter][1]);
	if (counter == 0)
	{
		exec_first_cmd(d);
		return ;
	}
	if (d->counter + 1 == d->cmds.count)
		dup2(d->iofd[1], STDOUT_FILENO);
	else
	{
		dup2(d->pipefd[d->counter][1], STDOUT_FILENO);
		close(d->pipefd[d->counter][1]);
	}
	dup2(d->pipefd[d->counter - 1][0], STDIN_FILENO);
	close_pipes(d);
	if (execve(d->cmds.arr[d->counter][0], d->cmds.arr[d->counter], __environ)
		== -1)
		(free_all(d), exit(1));
}

void	print_all_cmds(t_cmd *cmds)
{
	size_t	i;
	size_t	counter;

	i = 0;
	while (i < cmds->count)
	{
		ft_printf("%d: ", i);
		counter = 0;
		while (cmds->arr[i][counter])
		{
			ft_printf("%s ", cmds->arr[i][counter]);
			counter++;
		}
		ft_printf("\n");
		i++;
	}
}

void	free_all(t_abst *d)
{
	free(d->pipefd);
	free_cmds(&d->cmds);
}

int	main(int argc, char **argv)
{
	t_abst	d;

	if (argc < 5)
		return (1);
	if (!init_abst(argc, argv, &d))
		return (ft_printf("init_error\n"), 1);
	// print_all_cmds(&d.cmds);
	while (d.counter < d.cmds.count)
	{
		if (d.counter + 1 < d.cmds.count)
			if (pipe(d.pipefd[d.counter]) == -1)
				return (free_all(&d), 1);
		d.pid = fork();
		if (d.pid == -1)
			return (free_all(&d), 1);
		if (d.pid == 0)
			exec_cmd(&d, d.counter);
		d.counter++;
	}
	close_pipes(&d);
	while (wait(&d.status) > 0)
		;
	return (free_all(&d), 0);
}
