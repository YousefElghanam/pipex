/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:11:31 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/07/10 18:45:48 by jel-ghna         ###   ########.fr       */
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

int	is_limiter(char *line, char *limiter)
{
	size_t	limiter_len;

	limiter_len = ft_strlen(limiter);
	if (ft_strncmp(line, limiter, limiter_len) == 0
		&& line[limiter_len] == '\n')
		return (1);
	return (0);
}

void	here_doc(t_abst *d)
{
	char	*line;

	line = get_next_line(0);
	while (line && !is_limiter(line, d->limiter))
	{
		write(d->iofd[0], line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	
	free(line);
}

static void	exec_first_cmd(t_abst *d)
{
	ft_printf("%d>><<<<<\n", d->pipefd[0][1]);
	dup2(d->pipefd[0][1], STDOUT_FILENO);
	// if (d->is_here_doc)
	dup2(d->iofd[0], STDIN_FILENO);
	close_pipes(d);
	// unlink(".here_doc");
	if (execve(d->cmds.arr[0][0], d->cmds.arr[0], __environ)
		== -1)
		(free_all(d), exit(1));
}

static void	exec_cmd(t_abst *d, size_t counter)
{
	if (counter == 0)
	{
		exec_first_cmd(d);
		return ;
	}
	ft_printf("%d>><<<<<\n", d->pipefd[0][0]);
	if (d->counter + 1 == d->cmds.count)
		dup2(d->iofd[1], STDOUT_FILENO);
	else
	{
		dup2(d->pipefd[d->counter][1], STDOUT_FILENO);
		close(d->pipefd[d->counter][1]);
	}
	// if (d->is_here_doc && counter != d->cmds.count - 1)
	// {
	// 	close(d->pipefd[d->counter - 1][0]);
	// 	d->pipefd[d->counter - 1][0] = open(".here_doc", O_RDONLY);
	// }
	ft_printf("here\n");
	// if (d->is_here_doc)
	// 	dup2(d->iofd[0], STDIN_FILENO);
	// else
	lseek(d->pipefd[0][0], 0, SEEK_SET);
	ft_printf("%s\n", get_next_line(d->pipefd[0][0]));
	dup2(d->pipefd[0][0], STDIN_FILENO);
	// ft_printf(">>>>>> i IS:%d\n", i);
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
	
	while (d.counter < d.cmds.count)
	{
		if (d.counter + 1 < d.cmds.count)
			if (pipe(d.pipefd[d.counter]) == -1)
				return (free_all(&d), 1);
		if (d.is_here_doc && d.counter == 0)
			here_doc(&d);
		d.pid = fork();
		if (d.pid == -1)
			return (free_all(&d), 1);
		if (d.pid == 0)
			exec_cmd(&d, d.counter);
		// else if (d.pid > 0) {
		// 	// in the parent
		// 	// close(d.pipefd[d.counter][1]);
			
		// }
		d.counter++;
	}
	close_pipes(&d);
	while (wait(&d.status) > 0)
		;
	return (free_all(&d), 0);
}
