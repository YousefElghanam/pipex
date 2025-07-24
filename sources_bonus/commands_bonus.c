/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:11:28 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/07/21 21:26:29 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	swap_pathed_cmd(char *pathed_cmd, char **cmd)
{
	char	*tmp;

	tmp = *cmd;
	*cmd = pathed_cmd;
	free(tmp);
}

static int	search_paths(char **path, char **cmd, int *ret)
{
	size_t	i;
	char	*pathed_cmd;

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
	write(2, *cmd, ft_strlen(*cmd));
	write(2, ": command not found\n", 20);
	*ret = 0;
	return (free_split_arr(path), 1);
}

static int	create_pathed_cmd(char **cmd, int *ret)
{
	size_t	i;
	char	**path;

	i = 0;
	while (__environ[i])
	{
		if (ft_strncmp(__environ[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	if (!__environ[i] || ft_strlen(__environ[i]) < 6)
		return (write(2, "invalid PATH\n", 13), 0);
	path = ft_split(__environ[i] + 5, ':');
	if (!path)
		return (0);
	return (search_paths(path, cmd, ret));
}

int	create_cmds(int argc, char **argv, t_cmd *cmds, int is_here_doc)
{
	size_t	i;
	int		ret;

	i = is_here_doc;
	cmds->count = 0;
	ret = 1;
	while (i + 3 < (size_t)argc && i < 100000)
	{
		cmds->arr[i - is_here_doc] = ft_split(argv[i + 2], ' ');
		if (!cmds->arr[i - is_here_doc])
			return (perror("pipex"), free_cmds(cmds), 0);
		cmds->count += 1;
		if (!ft_strchr(cmds->arr[i - is_here_doc][0], '/'))
		{
			if (!create_pathed_cmd(&cmds->arr[i - is_here_doc][0], &ret))
				return (free_cmds(cmds), 0);
		}
		else if (access(cmds->arr[i - is_here_doc][0], F_OK) != 0)
			(perrcmd(cmds->arr[i - is_here_doc][0]), ret = 0);
		i++;
	}
	cmds->arr[i - is_here_doc] = NULL;
	return (free_if_fail(cmds, ret), ret);
}
