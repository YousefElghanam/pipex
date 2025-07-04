#include "pipex.h"

void	free_cmd(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	*arr = NULL;
}

void	free_all_cmds(char **arr[100], size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
		free_cmd(arr[i++]);
}

size_t	create_cmds(int argc, char **argv, char **cmds[100])
{
	size_t	i;

	i = 2;
	while (i + 1 < (size_t)argc)
	{
		cmds[i - 2] = ft_split(argv[i], ' ');
		if (!cmds[i - 2])
		{
			free_all_cmds(cmds, i - 1);
			exit(errno);
		}
		i++;
	}
	return (i - 2);
}
