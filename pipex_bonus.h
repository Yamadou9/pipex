/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydembele <ydembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:13:51 by ydembele          #+#    #+#             */
/*   Updated: 2025/08/18 20:32:59 by ydembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

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
	int		prev_nb[2];
	int		*pid;
	int		i;
	int		n_pid;
	char	*line;
	int		fd[2];
}	t_x;

char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);
int		ft_strncmp(char *s1, char *s2, int n);
void	free_all(char **str);
void	my_close(int fd1, int fd2, int fd3, int fd4);
void	null_function(t_x *x);
char	*get_next_line(int fd);
size_t	ft_strlen(const char *s);
void	my_wait(t_x *x);
void	next(t_x *x);
void	initialisation(t_x *x, char **av, int ac);
void	exit_error(char *msg, t_x *x, int code);
void	malloc_pid(t_x *x, int ac, char **av);

#endif