NAME		= philo
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -rf
LINKER		= -lpthread

SRCS		= main.c utils.c simulation.c
OBJS		= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LINKER) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.SECONDARY:
.PHONY: all clean fclean re
