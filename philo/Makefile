NAME = philo
CFILES = main.c libft_helpers.c
OFILES = $(CFILES:.c=.o)

CFLAGS = -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OFILES)
	cc $(CFLAGS) $(OFILES) -o $(NAME) -g -lpthread

%.o: %.c
	cc $(CFLAGS) -c $< -o $@ -g

clean:
	rm -f $(OFILES)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)


.PHONY: all clean fclean re
