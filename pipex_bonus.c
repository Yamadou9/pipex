/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydembele <ydembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:12:51 by ydembele          #+#    #+#             */
/*   Updated: 2025/08/05 16:15:42 by ydembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*ft_strslashjoin(char const *s1, char const *s2);

void	exit_error(char *msg, t_x x, int i, int code)
{
	if (!msg)
		printf("%s: command not found\n", x.cmd[0]);
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

// void	first(char **av, t_x x, char **env)
// {
// 	x.infile = open(av[1], O_RDONLY);
// 	if (x.infile < 0)
// 		exit_error("open", x, 2);
// 	x.cmd = ft_split(av[2], ' ');
// 	if (!x.cmd)
// 		exit_error("split", x, 2);
// 	x.all_cmd = ft_env(env, x.cmd[0], x);
// 	if (!x.all_cmd)
// 		exit_error(NULL, x, 0);
// 	if (dup2(x.infile, 0) == -1 || dup2(x.p_nb[1], 1) == -1)
// 		exit_error("dup2", x, 2);
// 	close(x.infile);
// 	execve(x.all_cmd, x.cmd, env);
// 	close(x.p_nb[0]);
// 	exit_error("execve", x, 2);
// }

void	do_cmd(t_x x, char **env, int i, char *commande)
{
	x.cmd = ft_split(commande, ' ');
	if (!x.cmd)
		exit_error("split", x, 1, 1);
	x.all_cmd = ft_env(env, x.cmd[0], x);
	if (!x.all_cmd)
		exit_error(NULL, x, 1, 1);
	execve(x.all_cmd, x.cmd, env);
	exit_error("execve", x, 1, 127);
}

// void	second(char **av, t_x x, char **env, pid_t	pid)
// {
// 	if (waitpid(pid, NULL, 0) == -1)
// 		exit_error("wait", x, 1);
// 	x.outfl = open(av[4], O_WRONLY | O_TRUNC | O_CREAT, 0777);
// 	if (x.outfl < 0)
// 		exit_error("open", x, 1);
// 	x.cmd = ft_split(av[3], ' ');
// 	if (!x.cmd)
// 		exit_error("split", x, 1);
// 	x.all_cmd = ft_env(env, x.cmd[0], x);
// 	if (!x.all_cmd)
// 		exit_error(NULL, x, 1);
// 	close(x.p_nb[1]);
// 	if (dup2(x.outfl, 1) == -1 || dup2(x.p_nb[0], 0) == -1)
// 		exit_error("dup2", x, 1);
// 	close(x.outfl);
// 	execve(x.all_cmd, x.cmd, env);
// 	exit_error("execve", x, 1);
// }

void	null_function(t_x *x)
{
	(*x).all_cmd = NULL;
	(*x).cmd = NULL;
	(*x).path = NULL;
	(*x).local = NULL;
}

/*int	main(int ac, char **av, char **env)
{
	pid_t	pid;
	t_x		x;

	if (!env)
		return (0);
	if (ac != 5)
		return (0);
	if (pipe(x.p_nb) == -1)
		return (0);
	null_function(&x);
	pid = fork();
	if (pid < 0)
		return (0);
	if (pid == 0)
		first(av, x, env);
	second(av, x, env, pid);
	perror("Second");
	return (0);
}*/

int	main(int ac, char **av, char **env)
{
	t_x		x;
	int		i;
	int		n_pid;

	i = 1;
	n_pid = 0;
	if (ac < 5 || !*env)
		return (1);
	x.infile = open(av[1], O_RDONLY);
	x.outfl = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (x.infile < 0 || x.outfl < 0)
		exit_error("open", x, 1, 1);
	x.pid = malloc(sizeof(pid_t) * (ac - 3));
	if (!x.pid)
		exit_error("malloc", x, 1, 1);
	x.prev_nb[0] = -1;
	x.prev_nb[1] = -1;
	while (++i < ac - 1)
	{
		if (pipe(x.p_nb) < 0)
			exit_error("pipe", x, 1, 1);
		x.pid[n_pid] = fork();
		if (x.pid[n_pid] < 0)
			exit_error("fork", x, 1, 1);
		if (x.pid[n_pid] == 0)
		{
			if (i == 2)
				dup2(x.infile, 0);
			else
				dup2(x.prev_nb[0], 0);
			if (i == ac - 2)
				dup2(x.outfl, 1);
			else
				dup2(x.p_nb[1], 1);
			my_close(x.infile, x.outfl, x.p_nb[0], x.p_nb[1]);
			my_close(x.prev_nb[0], x.prev_nb[1], -1, -1);
			do_cmd(x, env, i, av[i]);
			exit_error(NULL, x, 1, 127);
		}
		else
		{
			my_close(x.prev_nb[0], x.prev_nb[1], x.p_nb[1], -1);
			x.prev_nb[0] = x.p_nb[0];
			x.prev_nb[1] = -1;
			n_pid++;
		}
	}
	i = -1;
	while (i++ < n_pid - 1)
		waitpid(x.pid[i], NULL, 0);
	free(x.pid);
	return (0);
}
