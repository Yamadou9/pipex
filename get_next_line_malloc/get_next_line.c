/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydembele <ydembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:58:49 by ydembele          #+#    #+#             */
/*   Updated: 2025/05/21 19:08:00 by ydembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	c = c % 256;
	if (s == NULL)
		return (0);
	while (s[i] && s[i] != c)
		i++;
	if (s[i] == c)
		return (1);
	return (0);
}

char	*ft_njoin(char *res, char *buffer, int n)
{
	char	*line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!buffer)
		return (res);
	line = malloc(sizeof(char) * (ft_len(res, 0) + n + 1));
	if (line == NULL)
		return (free(res), NULL);
	if (res != NULL)
	{
		while (res[i])
		{
			line[i] = res[i];
			i++;
		}
	}
	while (j < n)
		line[i++] = buffer[j++];
	line[i] = 0;
	return (free(res), line);
}

char	*next_line(char *tab, int fd, char *buffer)
{
	char	*res;
	int		t;

	t = 0;
	res = NULL;
	if (*tab)
	{
		res = ft_njoin(res, tab, ft_len(tab, '\n'));
		if (res == NULL)
			return (free(buffer), NULL);
		ft_buffer_rest(tab, tab);
	}
	while (!ft_strchr(res, '\n'))
	{
		t = read(fd, buffer, BUFFER_SIZE);
		if (t <= 0)
			break ;
		buffer[t] = 0;
		res = ft_njoin(res, buffer, ft_len(buffer, '\n'));
		if (res == NULL)
			return (free(buffer), NULL);
		ft_buffer_rest(tab, buffer);
	}
	return (free(buffer), res);
}

char	*get_next_line(int fd)
{
	char		*next;
	static char	*tab = NULL;
	char		*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (free(tab), tab = NULL, NULL);
	if (!tab)
	{
		tab = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!tab)
			return (NULL);
		tab[0] = '\0';
	}
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (free(tab), tab = NULL, NULL);
	next = next_line(tab, fd, buffer);
	if (!next || tab[0] == 0)
	{
		free(tab);
		tab = NULL;
		return (next);
	}
	return (next);
}

/*int	main(int ac, char **av)
{
	char *res;
	int fd = open(av[1], O_RDONLY);
	int i = 0;

	while (i < 49)
	{
		res = get_next_line(fd);
		printf("%s", res);
		free(res);
		i++;
	}
}*/
