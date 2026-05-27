# **************************************************************************** #
# ****************************** VARIABLES: ********************************** #
# **************************************************************************** #
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g

NAME = philo

SRC = src/philo.c src/utils.c src/thread_utils.c src/parse.c src/time.c

OBJ = $(SRC:.c=.o)
	
INCLUDE = -Iinclude

# **************************************************************************** #
# ****************************** RULES: ************************************** #
# **************************************************************************** #
all: $(NAME)

$(NAME): $(OBJ)	
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
