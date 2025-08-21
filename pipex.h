/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydembele <ydembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:57:02 by ydembele          #+#    #+#             */
/*   Updated: 2025/08/21 19:44:10 by ydembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <unistd.h>
# include <sys/types.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>

typedef struct t_pipex
{
	int		infile;
	int		outfl;
	char	**cmd;
	char	*all_cmd;
	char	*path;
	char	**local;
	int		p_nb[2];
	int		pid2;
	int		i;
	int		status;
	int		exit_code;
}	t_x;

char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);
int		ft_strncmp(char *s1, char *s2, int n);
void	free_all(char **str);
void	null_function(t_x *x);
void	my_close(int fd1, int fd2, int fd3, int fd4);
void	my_wait(t_x *x);
void	exit_error(char *msg, t_x x, int i, int code);
size_t	ft_strlen(const char *s);

#endif