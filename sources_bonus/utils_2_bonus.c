/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:05:20 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/07/23 14:05:21 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	perrcmd(char *cmd)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	perror("");
}

void	free_cmds(t_cmd *cmds)
{
	size_t	i;

	i = 0;
	while (i < cmds->count)
		free_split_arr(cmds->arr[i++]);
}

void	free_if_fail(t_cmd *cmds, int fail)
{
	if (!fail)
		free_cmds(cmds);
}
