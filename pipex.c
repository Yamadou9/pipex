/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydembele <ydembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:55:00 by ydembele          #+#    #+#             */
/*   Updated: 2025/07/15 18:15:40 by ydembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	return (free_all(local), free(path), NULL);
}

int	first(char **av, int *p_nb, char **env)
{
	int		infile;
	char	**cmd;
	char	*all_cmd;

	cmd = ft_split(av[2], ' ');
	if (!cmd)
		return (perror("split"), 0);
	all_cmd = ft_env(env, cmd[0]);
	if (!all_cmd)
		return (free_all(cmd), 0);
	infile = open(av[1], O_RDONLY);
	if (infile < 0)
		return (perror("open"), free_all(cmd), free(all_cmd), exit(1), 0);
	dup2(infile, 0);
	dup2(p_nb[1], 1);
	close(infile);
	execve(all_cmd, cmd, env);
	perror("execve");
	return (free(all_cmd), free_all(cmd), 0);
}

int	second(char **av, int *p_nb, char **env, pid_t	pid)
{
	int		outfile;
	char	**cmd;
	char	*all_cmd;

	if (waitpid(pid, NULL, 0) == -1)
	{
		close(p_nb[1]);
		return (perror("wait"), 0);
	}
	cmd = ft_split(av[3], ' ');
	if (!cmd)
		return (perror("split"), 0);
	all_cmd = ft_env(env, cmd[0]);
	if (!all_cmd)
		return (free_all(cmd), 0);
	outfile = open(av[4], O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (outfile < 0)
		return (perror("open"), free_all(cmd), free(all_cmd), 0);
	close(p_nb[1]);
	if (dup2(outfile, 1) == -1 || dup2(p_nb[0], 0) == -1)
		return (free(all_cmd), free_all(cmd), close(p_nb[0]), close(outfile), 0);
	close(outfile);
	execve(all_cmd, cmd, env);
	return (free(all_cmd), free_all(cmd), 0);
}

int	main(int ac, char **av, char **env)
{
	pid_t	pid;
	int		p_nb[2];

	if (pipe(p_nb) == -1)
		return (0);
	pid = fork();
	if (pid < 0)
		return (0);
	if (pid == 0)
	{
		if (!first(av, p_nb, env))
			return (perror("first"), 0);
	}
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