/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josefelghnam <josefelghnam@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:11:28 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/07/19 18:54:00 by josefelghna      ###   ########.fr       */
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

static int	search_paths(char **path, char **cmd)
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
	return (free_split_arr(path), 0);
}

static int	create_pathed_cmd(char **cmd)
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
	if (!__environ[i])
		return (write(2, "PATH is unset\n", 14), 0);
	path = ft_split(__environ[i] + 5, ':');
	if (!path)
		return (0);
	return (search_paths(path, cmd));
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
		cmds->arr[i - is_here_doc] = ft_split(argv[i + 2], ' ');
		if (!cmds->arr[i - is_here_doc])
			return (perror("pipex"), free_cmds(cmds), 0);
		cmds->count += 1;
		if (!ft_strchr(cmds->arr[i - is_here_doc][0], '/'))
			if (!create_pathed_cmd(&cmds->arr[i - is_here_doc][0]))
				return (free_cmds(cmds), 0);
		i++;
	}
	cmds->arr[i - is_here_doc] = NULL;
	return (1);
}
