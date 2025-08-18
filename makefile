NAME = pipex
BONUS = pipex_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
RM = rm -f

SRCS = ft_split.c ft_strjoin.c pipex.c pipex_utils.c

BONUS_SRCS = ft_split.c ft_strjoin.c pipex_bonus.c pipex_utils_bonus.c \
	get_next_line/get_next_line.c \
	get_next_line/get_next_line_utils.c

OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

bonus: $(BONUS)

$(BONUS): $(BONUS_OBJS)
	$(CC) $(CFLAGS) -o $(BONUS) $(BONUS_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(BONUS_OBJS)

fclean: clean
	$(RM) $(NAME) $(BONUS)

re: fclean all

.PHONY: all bonus clean fclean re
