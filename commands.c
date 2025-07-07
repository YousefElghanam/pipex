#include "pipex.h"

void	free_cmd_arr(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	free_cmds(t_cmd *cmds, size_t start, size_t count)
{
	size_t	i;

	i = 0;
	while (start < cmds->count && cmds->count && i < count)
	{
		free_cmd_arr(cmds->arr[start]);
		cmds->arr[start] = NULL;
		cmds->count -= 1;
		start += 1;
		i += 1;
	}
}
int	create_cmds(int argc, char **argv, t_cmd *cmds)
{
	size_t	i;

	i = 0;
	cmds->count = 0;
	// I can only store "100" ([0]->[99]) commands, not 101. Always save the last one to be a null, so I always NULL terminate the array
	while (i + 3 < (size_t)argc && i < 100)
	{
		cmds->arr[i] = ft_split(argv[i + 2], ' ');
		if (!cmds->arr[i] && i)
		{
			free_cmds(cmds, 0, i);
			return (0);
		}
		cmds->count += 1;
		i++;
	}
	cmds->arr[i] = NULL;
	return (1);
}
