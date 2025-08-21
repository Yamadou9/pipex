/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydembele <ydembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:13:14 by ydembele          #+#    #+#             */
/*   Updated: 2025/08/21 22:01:05 by ydembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	x->exit_code = 0;
	x->status = 0;
	x->outfl = -1;
	x->infile = -1;
}

void	my_wait(t_x *x)
{
	if (waitpid((*x).pid2, &(*x).status, 0) == -1)
		exit_error("waitpid", *x, 1, 1);
	if (WIFEXITED((*x).status))
		(*x).exit_code = WEXITSTATUS((*x).status);
}
