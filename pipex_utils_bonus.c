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
