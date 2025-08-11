/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydembele <ydembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 13:32:06 by ydembele          #+#    #+#             */
/*   Updated: 2025/05/21 18:13:06 by ydembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000000000
# endif

int		ft_len(char *s, char c);
char	*ft_njoin(char *res, char *buffer, int n);
void	ft_buffer_rest(char *tab, char	*buffer);
void	ft_metzero(char *tab);
void	*ft_memset(void *s, int c, size_t n);
char	*get_next_line(int fd);
char	*next_line(char *tab, int fd, char *buffer);
int		ft_strchr(const char *s, int c);
char	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(char *s);

#endif