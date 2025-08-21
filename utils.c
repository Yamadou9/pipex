/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydembele <ydembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:36:54 by ydembele          #+#    #+#             */
/*   Updated: 2025/08/20 20:40:14 by ydembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	malloc_pid(t_x *x, int ac, char **av)
{
	if (ft_strncmp(av[1], "here_doc", 8) != 0)
		x->pid = malloc(sizeof(pid_t) * (ac - 3));
	else
		x->pid = malloc(sizeof(pid_t) * (ac - 4));
	if (!x->pid)
		exit_error("malloc", x, 1);
}

void	pipe_fork(pid_t pid, t_x *x)
{
	if (pipe(x->fd) == -1)
		exit_error("pipe", x, 1);
	pid = fork();
	if (pid == -1)
		exit_error("fork", x, 1);
}

void	wait_doc(t_x *x, pid_t pid)
{
	close(x->fd[1]);
	x->prev_nb[0] = x->fd[0];
	waitpid(pid, NULL, 0);
}

char	*ft_env(char **env, char *cmd, t_x x)
{
	x.local = NULL;
	x.path = NULL;
	x.i = 0;
	while (env[x.i++])
	{
		if (ft_strncmp("PATH=", env[x.i], 5) == 0)
		{
			x.local = ft_split(env[x.i] + 5, ':');
			if (!x.local)
				return (0);
		}
	}
	x.i = 0;
	if (!x.local)
		return (0);
	while (x.local[x.i++])
	{
		x.path = ft_strslashjoin(x.local[x.i], cmd);
		if (!x.path)
			return (free_all(x.local), NULL);
		if (access(x.path, F_OK | X_OK) == 0)
			return (free_all(x.local), x.path);
		free(x.path);
	}
	return (free_all(x.local), free(x.path), NULL);
}
