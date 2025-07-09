/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:11:28 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/07/09 21:18:50 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	free_split_arr(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static void	swap_pathed_cmd(char *pathed_cmd, char **cmd)
{
	char	*tmp;

	tmp = *cmd;
	*cmd = pathed_cmd;
	free(tmp);
}

static int	create_pathed_cmd(char **cmd)
{
	size_t	i;
	char	*pathed_cmd;
	char	**path;

	i = 0;
	while (__environ[i++])
		if (ft_strncmp(__environ[i - 1], "PATH=", 5) == 0)
			break ;
	path = ft_split(__environ[i - 1] + 5, ':');
	if (!path)
		return (0);
	i = 0;
	while (path[i])
	{
		pathed_cmd = ft_strsjoin(path[i], "/", *cmd);
		if (!pathed_cmd)
			return (free_split_arr(path), 0);
		if (access(pathed_cmd, F_OK) == 0)
			return (swap_pathed_cmd(pathed_cmd, cmd), free_split_arr(path), 1);
		free(pathed_cmd);
		i++;
	}
	return (free_split_arr(path), 1);
}

void	free_cmds(t_cmd *cmds)
{
	size_t	i;

	i = 0;
	while (i < cmds->count)
		free_split_arr(cmds->arr[i++]);
}

int	create_cmds(int argc, char **argv, t_cmd *cmds, int is_here_doc)
{
	size_t	i;

	i = is_here_doc;
	cmds->count = 0;
	while (i + 3 < (size_t)argc && i < 100000)
	{
		cmds->arr[i] = ft_split(argv[i + 2], ' ');
		if (!cmds->arr[i])
			return (free_cmds(cmds), 0);
		cmds->count += 1;
		if (!ft_strchr(cmds->arr[i][0], '/'))
			if (!create_pathed_cmd(&cmds->arr[i][0]))
				return (free_cmds(cmds), 0);
		i++;
	}
	cmds->arr[i] = NULL;
	return (1);
}
