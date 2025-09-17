CC = cc
CFLAGS = -Wall -Wextra -Werror  #-g -fsanitize=thread

SRCS = philosophers.c
OBJS = $(SRCS:.c=.o)

HEADER = philosophers.h
NAME = philosophers

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HEADER) 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
.PHONY: all clean fclean re