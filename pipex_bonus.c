/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydembele <ydembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:12:51 by ydembele          #+#    #+#             */
/*   Updated: 2025/08/21 21:50:33 by ydembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	exit_error(char *msg, t_x *x, int code)
{
	if (!msg)
	{
		write(2, x->cmd[0], ft_strlen(x->cmd[0]));
		write(2, ": command not found\n", 21);
	}
	else
		perror(msg);
	if (x->all_cmd != NULL)
	{
		free(x->all_cmd);
		x->all_cmd = NULL;
	}
	if (x->cmd != NULL)
	{
		free_all(x->cmd);
		x->cmd = NULL;
	}
	if (x->pid != NULL)
		free(x->pid);
	my_close(x->outfl, x->p_nb[1], x->infile, x->p_nb[0]);
	my_close(x->prev_nb[0], x->prev_nb[1], -1, -1);
	exit(code);
}

void	do_cmd(t_x x, char **env, char *commande)
{
	x.cmd = ft_split(commande, ' ');
	if (!x.cmd || !x.cmd[0])
		exit_error("commande vide", &x, 127);
	x.all_cmd = ft_env(env, x.cmd[0], x);
	if (!x.all_cmd)
	{
		exit_error(NULL, &x, 127);
	}
	execve(x.all_cmd, x.cmd, env);
	exit_error("execve", &x, 127);
}

void	redirection(t_x *x, int ac, int i)
{
	if (i == 2)
	{
		if (dup2(x->infile, 0) == -1)
			exit_error("dup2", x, 1);
	}
	else
	{
		if (dup2(x->prev_nb[0], 0) == -1)
			exit_error("dup2", x, 1);
	}
	if (i == ac - 2)
	{
		if (dup2(x->outfl, 1) == -1)
			exit_error("dup2", x, 1);
	}
	else
	{
		if (dup2(x->p_nb[1], 1) == -1)
			exit_error("dup2", x, 1);
	}
	my_close((*x).infile, (*x).outfl, -1, (*x).p_nb[1]);
	my_close((*x).prev_nb[0], (*x).prev_nb[1], -1, -1);
}

void	here_doc(char **av, t_x *x)
{
	pid_t	pid;

	if (pipe(x->fd) == -1)
		exit_error("pipe", x, 1);
	pid = fork();
	if (pid == 0)
	{
		close(x->fd[0]);
		while (1)
		{
			free(x->line);
			x->line = get_next_line(0);
			if (!x->line)
				break ;
			if (ft_strncmp(x->line, av[2], ft_strlen(av[2])) == 0
				&& x->line[ft_strlen(av[2])] == '\n')
			{
				free(x->line);
				exit(0);
			}
			write(x->fd[1], x->line, ft_strlen(x->line));
		}
		close(x->fd[1]);
		exit(0);
	}
	else
		wait_doc(x, pid);
}

int	main(int ac, char **av, char **env)
{
	t_x		x;
	int		i;

	i = 1;
	if (ac < 5 || !*env)
		return (1);
	initialisation(&x, av, ac);
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
	{
		i = 2;
		here_doc(av, &x);
	}
	malloc_pid(&x, ac, av);
	while (++i < ac - 1)
	{
		if (pipe(x.p_nb) < 0)
			exit_error("pipe", &x, 1);
		x.pid[x.n_pid] = fork();
		if (x.pid[x.n_pid] < 0)
			exit_error("fork", &x, 1);
		if (x.pid[x.n_pid] == 0)
		{
			redirection(&x, ac, i);
			do_cmd(x, env, av[i]);
			exit_error(NULL, &x, 127);
		}
		else
			next(&x);
	}
	return (my_wait(&x));
}
