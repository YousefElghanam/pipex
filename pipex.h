#ifndef PIPEX_H
# define PIPEX_H

# include <libft.h>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
#include <errno.h>

typedef struct s_cmd
{
	char	**arr[100001];
	size_t	count;
} t_cmd;

typedef struct s_abst
{
	int		(*pipefd)[2];
	int		iofd[2];
	int		status;
	size_t	counter;
	pid_t	pid;
	t_cmd	cmds;
}	t_abst;

/* commands.c */
int		create_cmds(int argc, char **argv, t_cmd *cmds);
void	free_cmds(t_cmd *cmds);

#endif