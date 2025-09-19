CC = cc
CFLAGS = -Wall -Wextra -Werror  -O3 #-g -fsanitize=thread

SRCS = philosophers.c actions.c help_function.c main_helper.c
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