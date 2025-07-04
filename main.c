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


int	main(int argc, char **argv)
{
	int		pipefd[2];
	int		fd[2];
	pid_t	pid;
	t_cmd	cmd;

	if (argc < 5)
		return (1);
	cmd.count = create_cmds(argc, argv, cmd.arr);
	// ft_printf("%s", cmd.arr[0][0]);
	fd[0] = open(argv[1], O_RDONLY);
	fd[1] = open(argv[4], O_WRONLY);
	// valgrind = open("valgrind.txt", O_WRONLY);
	if (pipe(pipefd) < 0)
		return (free_all_cmds(cmd.arr, cmd.count), 1);
	pid = fork();
	if (pid < 0)
		return (free_all_cmds(cmd.arr, cmd.count), 1);
	if (pid > 0)
	{
		close(pipefd[0]);
		dup2(fd[0], STDIN_FILENO);
		// dup2(STDERR_FILENO, valgrind);
		dup2(pipefd[1], STDOUT_FILENO);
		execve(cmd.arr[0][0], cmd.arr[0], __environ);
	}
	else
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		execve(cmd.arr[1][0], cmd.arr[1], __environ);
		close(pipefd[0]);
	}
	perror(strerror(errno));
	return (free_all_cmds(cmd.arr, cmd.count), 0);
}
