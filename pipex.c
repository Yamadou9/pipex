/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydembele <ydembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:55:00 by ydembele          #+#    #+#             */
/*   Updated: 2025/07/19 16:38:00 by ydembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exit_error(char *msg, char **cmd, char *all_cmd, int fd1)
{
	if (msg)
		perror(msg);
	if (cmd)
		free_all(cmd);
	if (all_cmd)
		free(all_cmd);
	if (fd1 >= 0)
		close(fd1);
	exit(EXIT_FAILURE);
}

char	*ft_env(char **env, char *cmd)
{
	int		i;
	char	*path;
	char	**local;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
		{
			local = ft_split(env[i] + 5, ':');
			if (!local)
				return (0);
		}
		i++;
	}
	i = 0;
	if (!local)
		return (0);
	while (local[i])
	{
		path = ft_strjoin(ft_strjoin(local[i], "/"), cmd);
		if (!path)
			return (free(local), NULL);
		if (access(path, F_OK | X_OK) == 0)
			return (free_all(local), path);
		i++;
	}
	perror("acces");
	return (free_all(local), free(path), NULL);
}

void	first(char **av, int *p_nb, char **env)
{
	int		infile;
	char	**cmd;
	char	*all_cmd;

	infile = open(av[1], O_RDONLY);
	if (infile < 0)
		exit_error("open", NULL, NULL, -1);
	cmd = ft_split(av[2], ' ');
	if (!cmd)
		exit_error("split", NULL, NULL, infile);
	all_cmd = ft_env(env, cmd[0]);
	if (!all_cmd)
	{
		close(p_nb[0]);
		exit_error("ft_env", cmd, NULL, infile);
	}
	if (dup2(infile, 0) == -1 || dup2(p_nb[1], 1) == -1)
	{
		close(p_nb[0]);
		exit_error("dup2", cmd, all_cmd, infile);
	}
	close(infile);
	execve(all_cmd, cmd, env);
	close(p_nb[0]);
	exit_error("execve", cmd, all_cmd, infile);
}

void	second(char **av, int *p_nb, char **env, pid_t	pid)
{
	int		outfl;
	char	**cmd;
	char	*all_cmd;

	if (waitpid(pid, NULL, 0) == -1)
		exit_error("wait", NULL, NULL, p_nb[0]);
	outfl = open(av[4], O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (outfl < 0)
		exit_error("open", NULL, NULL, p_nb[0]);
	cmd = ft_split(av[3], ' ');
	if (!cmd)
	{
		close(outfl);
		exit_error("split", NULL, NULL, p_nb[0]);
	}
	all_cmd = ft_env(env, cmd[0]);
	if (!all_cmd)
	{
		close(outfl);
		exit_error("command", cmd, NULL, p_nb[0]);
	}
	close(p_nb[1]);
	if (dup2(outfl, 1) == -1 || dup2(p_nb[0], 0) == -1)
	{
		close(outfl);
		exit_error("open", cmd, all_cmd, p_nb[0]);
	}
	close(outfl);
	execve(all_cmd, cmd, env);
	exit_error("open", cmd, all_cmd, p_nb[0]);
}

int	main(int ac, char **av, char **env)
{
	pid_t	pid;
	int		p_nb[2];

	if (ac != 5)
		return (0);
	if (pipe(p_nb) == -1)
		return (0);
	pid = fork();
	if (pid < 0)
		return (0);
	if (pid == 0)
		first(av, p_nb, env);
	second(av, p_nb, env, pid);
	perror("Second");
	return (0);
}

// int      main()
// {
//   char      *argv[] = {"/bin/ls"};
//   char      *newargv[] = {"ls", "-l", NULL };
//   char      *newenviron[] = { NULL };
//   execve(argv[0], newargv, newenviron);
//   perror("execve");
//   printf("j'ai fais ls!\n");
// }