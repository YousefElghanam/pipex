#include "pipex.h"

void	free_all(t_abst *d)
{
	free(d->pipefd);
	free_cmds(&d->cmds);
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

static int	open_io_files(char **argv, t_abst *d)
{
	d->iofd[0] = open(argv[1], O_RDONLY);
	if (d->iofd[0] == -1)
		return (0);
	d->iofd[1] = open(argv[d->cmds.count + 2], O_WRONLY | O_TRUNC);
	if (d->iofd[1] == -1)
		return (0);
	return (1);
}

void	close_pipes(t_abst *d)
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

void	exec_first_cmd(t_abst *d)
{
	dup2(d->pipefd[0][1], STDOUT_FILENO);
	dup2(d->iofd[0], STDIN_FILENO);
	close_pipes(d);
	if (execve(d->cmds.arr[0][0], d->cmds.arr[0], __environ)
		== -1)
		(free_all(d), exit(1));
}

void	exec_cmd(t_abst *d, size_t counter)
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

int	init_pipes(t_abst *d)
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

int	init_abst(int argc, char **argv, t_abst *d)
{
	if (!create_cmds(argc, argv, &d->cmds))
		return (0);
	if (!init_pipes(d))
		return (free_all(d), 0);
	if (!open_io_files(argv, d))
		return (free_all(d), 0);
	d->counter = 0;
	return (1);
}

int	main(int argc, char **argv)
{
	t_abst	d;

	if (argc < 5)
		return (1);
	if (!init_abst(argc, argv, &d))
		return (ft_printf("init_error\n"), 1);
	// print_all_cmds(&d.cmds);
	// while (d.counter < d.cmds.count)
	// {
	// 	if (d.counter + 1 < d.cmds.count)
	// 		if (pipe(d.pipefd[d.counter]) == -1)
	// 			return (free_all(&d), 1);
	// 	d.pid = fork();
	// 	if (d.pid == -1)
	// 		return (free_all(&d), 1);
	// 	if (d.pid == 0)
	// 		exec_cmd(&d, d.counter);
	// 	d.counter++;
	// }
	// close_pipes(&d);
	// while (wait(&d.status) > 0)
	// 	;
	// // perror(strerror(errno));
	// // ft_printf("HERE: (%d)\n", d.status);
	return (free_all(&d), 0);
}
