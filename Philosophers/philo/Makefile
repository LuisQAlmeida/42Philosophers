# **************************************************************************** #
#                                   philo                                      #
# **************************************************************************** #

# **************************************************************************** #
#                                   Variables                                  #
# **************************************************************************** #
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g

NAME = philo

SRC_DIR = src/
INC_DIR = include

INCLUDE = -I$(INC_DIR)

CORE_FILES = main.c parse.c simulation.c
PHILO_FILES = actions.c meal_state.c monitor.c
SYNC_FILES = forks.c state.c
UTILS_FILES = time.c log.c messages.c utils.c

SRC_FILES = $(CORE_FILES) $(PHILO_FILES) $(SYNC_FILES) $(UTILS_FILES)
SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ = $(SRC:.c=.o)

# **************************************************************************** #
#                                   Rules                                      #
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
