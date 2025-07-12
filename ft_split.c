/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydembele <ydembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:49:15 by ydembele          #+#    #+#             */
/*   Updated: 2025/07/12 16:21:39 by ydembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

size_t	ft_strlen(const char *s);

void	free_all(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

size_t	ft_countword(char const *s, char c)
{
	int	set;
	int		i;
	size_t	word;

	word = 0;
	i = 0;
	set = 1;
	while (s[i])
	{
		if (set == 1 && s[i] != c)
		{
			word++;
			set = 0;
		}
		else if (s[i] == c)
			set = 1;
		i++;
	}
	return (word);
}

size_t	len_set(char const *s, int o, char c)
{
	size_t	i;

	i = 0;
	while (s[o] && s[o] == c)
				o++;
	while (s[o] && s[o] != c)
	{
		i++;
		o++;
	}
	return (i);
}

int	ft_cpy(const char *s, int i, char *res, char c)
{
	int	r;

	r = 0;
	while (s[i] != c && s[i])
		res[r++] = s[i++];
	res[r] = 0;
	return (i);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		i;
	size_t	j;

	i = 0;
	j = 0;
	res = malloc(sizeof(char *) * (ft_countword(s, c) + 1));
	if (res == NULL)
		return (NULL);
	while (j < ft_countword(s, c))
	{
		while (s[i] && s[i] == c)
			i++;
		res[j] = malloc(sizeof(char) * len_set(s, i, c) + 1);
		if (res[j] == NULL)
		{
			free_all(res);
			return (NULL);
		}
		i = ft_cpy(s, i, res[j], c);
		j++;
	}
	res[j] = NULL;
	return (res);
}
/*int		main(int ac, char **av)
{
	char	**res = ft_split("  tripouille  42  ", ' ');
	int	i = 0;
	while (i < 2)
		printf("%s\n", res[i++]);
}*/
