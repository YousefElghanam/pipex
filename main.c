#include "pipex.h"

// int	child(int pipefd[2])
// {
// 	close(pipefd[0]);
// 	char	*argv[3];

// 	argv[0] = "echo";
// 	argv[1] = "main.c";
// 	argv[2] = NULL;
// 	dup2(pipefd[1], STDOUT_FILENO);
// 	execve("/bin/cat", argv, __environ);
// 	close(pipefd[1]);
// 	return (1);
// }

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
	d->iofd[1] = open(argv[d->cmds.count + 2], O_WRONLY);
	if (d->iofd[0] < 0 || d->iofd[1] < 0)
		return (0);
	return (1);
}

void	child_process(t_abst *d)
{
	close(d->pipefd[0]);
	ft_printf("cmd (%s) == arg (%s)\n", d->cmds.arr[d->counter][0], d->cmds.arr[d->counter][1]);
	dup2(d->pipefd[1], STDOUT_FILENO);
	dup2(d->iofd[0], STDIN_FILENO);
	if (execve(d->cmds.arr[d->counter][0], d->cmds.arr[d->counter],
		__environ) == -1)
		exit(errno);
	close(d->pipefd[1]);
	free_cmds(&d->cmds, 0, d->cmds.count);
}

void	child_process_2(t_abst *d)
{
	if (d->counter + 1 == d->cmds.count)
		dup2(d->iofd[1], STDOUT_FILENO);	
	else
		dup2(d->pipefd[1], STDOUT_FILENO);
	dup2(d->pipefd[0], STDIN_FILENO);
	if (execve(d->cmds.arr[d->counter][0], d->cmds.arr[d->counter],
		__environ) == -1)
		exit(errno);
	close(d->pipefd[1]);
	close(d->pipefd[0]);
	free_cmds(&d->cmds, 0, d->cmds.count);
}

int	init_abst(int argc, char **argv, t_abst *d)
{
	if (!create_cmds(argc, argv, &d->cmds)
		|| !open_io_files(argv, d)
		|| init_pipes()) ///////////////////////////////////// HERE, Need to allocate dynamic number of pipes ((CAREFUL FREE and SEARCHC OTHER WAYS FOR VARIABLE PIPES))
	d->counter = 0;
	return (1);
}

int	main(int argc, char **argv)
{
	t_abst	d;

	if (argc < 5)
		return (1);
	if (!init_abst(argc, argv, &d))
		return (free_cmds(&d.cmds, 0, d.cmds.count), 1);
	print_all_cmds(&d.cmds);
	d.pid = fork();
	if (d.pid == -1)
		return (free_cmds(&d.cmds, 0, d.cmds.count), 1);
	if (d.pid == 0)
		(child_process(&d), exit(errno));
	while (++d.counter < d.cmds.count)
	{
		d.pid2 = fork();
		if (d.pid2 == -1)
			return (free_cmds(&d.cmds, 0, d.cmds.count), 1);
		if (d.pid2 == 0)
			(child_process_2(&d), exit(errno));
	}
	(close(d.pipefd[0]), close(d.pipefd[1]));
	return (free_cmds(&d.cmds, 0, d.cmds.count), 0);
}
