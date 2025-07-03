#include "pipex.h"

int	child(int pipe_fd[2])
{
	// char	*argv[3];

	// argv[0] = "echo";
	// argv[1] = "$PATH";
	// argv[2] = NULL;
	// execve("/bin/echo", argv, __environ);
	ft_putstr_fd("hi from child\n\n\n", pipe_fd[1]);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (1);
}

int	main(void)
{
	int		pipe_fd[2];
	pid_t	pid;
	char	buf[3];

	if (pipe(pipe_fd) < 0)
		return (1);
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0) // in child
		child(pipe_fd);
	else
	{
		waitpid(pid, NULL, 0);
		read(pipe_fd[0], buf, 3);
		write(1, buf, 3);
		// while (buf)
		// {
			// ft_printf("Parent got: (%s) from child\n", buf);
			// free(buf);
			// buf = get_next_line(pipe_fd[0]);
		// }
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		ft_printf("\nPARENT END\n");
	}
	
	return (0);
}