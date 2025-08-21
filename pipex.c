/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydembele <ydembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:55:00 by ydembele          #+#    #+#             */
/*   Updated: 2025/08/21 21:03:45 by ydembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_strslashjoin(char const *s1, char const *s2);

void	exit_error(char *msg, t_x x, int i, int code)
{
	if (!msg)
	{
		write(2, x.cmd[0], ft_strlen(x.cmd[0]));
		write(2, ": command not found\n", 21);
	}
	else if (msg)
		perror(msg);
	if (x.all_cmd != NULL)
		free(x.all_cmd);
	if (x.cmd != NULL)
		free_all(x.cmd);
	if (i == 1 && x.outfl >= 0)
		close(x.outfl);
	if (i == 1 && x.p_nb[1] >= 0)
		close(x.p_nb[1]);
	if (i == 2 && x.infile >= 0)
		close(x.infile);
	if (i == 2 && x.p_nb[0] >= 0)
		close(x.p_nb[0]);
	exit(code);
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

void	first(char **av, t_x x, char **env)
{
	x.infile = open(av[1], O_RDONLY);
	if (x.infile < 0)
		exit_error("open", x, 2, 1);
	x.cmd = ft_split(av[2], ' ');
	if (!x.cmd || !x.cmd[0])
		exit_error("commande vide", x, 1, 127);
	x.all_cmd = ft_env(env, x.cmd[0], x);
	if (!x.all_cmd)
		exit_error(NULL, x, 0, 127);
	if (dup2(x.infile, 0) == -1 || dup2(x.p_nb[1], 1) == -1)
		exit_error("dup2", x, 2, 1);
	close(x.infile);
	execve(x.all_cmd, x.cmd, env);
	close(x.p_nb[0]);
	exit_error("execve", x, 2, 1);
}

void	second(char **av, t_x *x, char **env, pid_t to_wait)
{
	(*x).pid2 = fork();
	if ((*x).pid2 < 0)
		return ;
	if ((*x).pid2 == 0)
	{
		(*x).outfl = open(av[4], O_WRONLY | O_TRUNC | O_CREAT, 0777);
		if ((*x).outfl < 0)
			exit_error("open", (*x), 1, 1);
		(*x).cmd = ft_split(av[3], ' ');
		if (!(*x).cmd || !(*x).cmd[0])
			exit_error("commande vide", (*x), 1, 127);
		(*x).all_cmd = ft_env(env, (*x).cmd[0], (*x));
		if (!(*x).all_cmd)
			exit_error(NULL, (*x), 1, 127);
		close((*x).p_nb[1]);
		if (dup2((*x).outfl, 1) == -1 || dup2((*x).p_nb[0], 0) == -1)
			exit_error("dup2", (*x), 1, 1);
		my_close((*x).outfl, (*x).p_nb[0], (*x).p_nb[1], -1);
		execve((*x).all_cmd, (*x).cmd, env);
		exit_error("execve", (*x), 1, 1);
	}
	my_close((*x).p_nb[0], (*x).p_nb[1], -1, -1);
	waitpid(to_wait, NULL, 0);
	my_wait(x);
	return ;
}

int	main(int ac, char **av, char **env)
{
	pid_t	pid;
	t_x		x;

	if (!*env)
	{
		write(2, "Environement manquant\n", 22);
		return (0);
	}
	if (ac != 5)
	{
		write(2, "Nombre d'argument\n", 18);
		return (0);
	}
	if (pipe(x.p_nb) == -1)
		return (0);
	null_function(&x);
	pid = fork();
	if (pid < 0)
		return (0);
	if (pid == 0)
		first(av, x, env);
	second(av, &x, env, pid);
	return (x.exit_code);
}
