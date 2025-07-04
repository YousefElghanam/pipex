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
	char	**arr[100];
	size_t	count;
} t_cmd;

/* commands.c */
size_t	create_cmds(int argc, char **argv, char **cmds[100]);
void	free_all_cmds(char **arr[100], size_t count);

#endif