/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josefelghnam <josefelghnam@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:44:46 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/07/19 18:19:58 by josefelghna      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <libft.h>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>

typedef struct s_cmd
{
	char	**arr[100001];
	size_t	count;
}	t_cmd;

typedef struct s_abst
{
	int		(*pipefd)[2];
	int		iofd[2];
	size_t	counter;
	pid_t	pid;
	t_cmd	cmds;
}	t_abst;

/* utils.c */
void	free_all(t_abst *d);
void	free_split_arr(char **arr);
void	close_pipes(t_abst *d);

/* init.c */
int		init_data(int argc, char **argv, t_abst *d);

/* commands.c */
int		create_cmds(int argc, char **argv, t_cmd *cmds);
void	free_cmds(t_cmd *cmds);

#endif