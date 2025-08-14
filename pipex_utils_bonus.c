/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydembele <ydembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:13:14 by ydembele          #+#    #+#             */
/*   Updated: 2025/08/06 15:39:00 by ydembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	my_close(int fd1, int fd2, int fd3, int fd4)
{
	if (fd1 >= 0)
		close (fd1);
	if (fd2 >= 0)
		close (fd2);
	if (fd3 >= 0)
		close (fd3);
	if (fd4 >= 0)
		close (fd4);
}

void	null_function(t_x *x)
{
	(*x).all_cmd = NULL;
	(*x).cmd = NULL;
	(*x).path = NULL;
	(*x).local = NULL;
}

void my_wait(t_x *x)
{
	int	i;

	i = 0;
	my_close(x->p_nb[0], x->infile, x->outfl, -1);
	while (i < x->n_pid)
	{
		waitpid(x->pid[i], NULL, 0);
		i++;
	}
	free(x->pid);
}

void	next(t_x *x)
{
	int	tmp_fd;

	tmp_fd = x->p_nb[0];
	my_close(x->prev_nb[0], x->prev_nb[1], x->p_nb[1], -1);
	x->prev_nb[0] = tmp_fd;
	x->prev_nb[1] = -1;
	x->n_pid++;
}

void	initialisation(t_x *x, char **av, int ac)
{
	null_function(x);
	x->prev_nb[0] = -1;
	x->prev_nb[1] = -1;
	x->n_pid = 0;
	x->p_nb[0] = -1;
	x->p_nb[1] = -1;
	x->outfl = -1;
	x->pid = NULL;
	x->line = NULL;
	if (ft_strncmp(av[1], "here_doc", 8) != 0)
	{
		x->infile = open(av[1], O_RDONLY);
		if (x->infile < 0)
			exit_error("open", *x, 1);
		x->outfl = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else
	{
		x->infile = -1;
		x->outfl = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	if (x->outfl < 0)
		exit_error("open", *x, 1);
	x->pid = malloc(sizeof(pid_t) * (ac - 3));
	if (!x->pid)
		exit_error("malloc", *x, 1);
}
