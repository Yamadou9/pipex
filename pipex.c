/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydembele <ydembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:55:00 by ydembele          #+#    #+#             */
/*   Updated: 2025/07/12 17:29:01 by ydembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);

void	first(char **av, int *p_nb)
{
	int		infile;
	char	**cmd;
	char	*all_cmd;

	cmd = ft_split(av[2], ' ');
	all_cmd = ft_strjoin("/bin/", cmd[0]);
	infile = open(av[1], O_RDONLY);
	if (infile < 0)
		return ;
	dup2(infile, 0);
	dup2(p_nb[1], 1);
	close(infile);
	execve(all_cmd, cmd, NULL);
	printf("bjr");
	return ;
}

void	second(char **av, int *p_nb)
{
	int		outfile;
	char	**cmd;
	char	*all_cmd;

	cmd = ft_split(av[3], ' ');
	all_cmd = ft_strjoin("/bin/", cmd[0]);
	outfile = open(av[4], O_RDWR);
	if (outfile < 0)
		return ;
	close(p_nb[1]);
	dup2(outfile, 1);
	dup2(p_nb[0], 0);
	close(outfile);
	execve(all_cmd, cmd, NULL);
	printf("bjr");
}

int	main(int ac, char **av)
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
		first(av, p_nb);
	}
	else
		second(av, p_nb);
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